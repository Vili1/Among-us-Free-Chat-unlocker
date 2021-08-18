#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h> 
#include <vector>

/* Global Variables */
HWND hGameWindow;
DWORD pID;
DWORD offsetGameToBaseAddress;
HANDLE processHandle = NULL;
DWORD baseAddress;

bool Steam;
int vSelect = 0;
int chatValue = 1;

DWORD dwGetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID)
{
   DWORD dwModuleBaseAddress = 0;
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
   MODULEENTRY32 ModuleEntry32 = { 0 };
   ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

   if (Module32First(hSnapshot, &ModuleEntry32))
   {
     do
     {
         if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
         {
            dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
            break;
         }
     } while (Module32Next(hSnapshot, &ModuleEntry32));

   }
   CloseHandle(hSnapshot);
   return dwModuleBaseAddress;
}

void reloadFunk()
{
   std::cout << "---------------------------------------------------------------------------\n";
   for (int i = 5; i > 0; i--)
   {
      if (i == 1)
      {
         std::cout << "Auto reloading in " << i << " second\n";
         Sleep(1000);
      }
      else 
      {
         std::cout << "Auto reloading in " << i << " seconds\n";
         Sleep(1000);
      }
   }
}

int main()
{
   reload:
   system("CLS");
   hGameWindow = FindWindow(NULL, "Among Us");
   if (hGameWindow != NULL)
   {
     std::cout << "Among Us found successfully!\n";
     std::cout << "---------------------------------------------------------------------------\n";
     std::cout << "Please select the version of your game and then press Enter!\n";
     std::cout << "1 - Steam - v2021.6.30s\n";
     std::cout << "2 - Epic Games - v2021.7.20e\n";
     std::cin >> vSelect;

      switch (vSelect)
      {
         case 1: //if vSelect == 1
         Steam = true;
         break;
         case 2:
         Steam = false;
         break;
         default: //if it doesn't fit anything above
         goto reload;
         break;
      }
   }
   else
   {
      std::cout << "Unable to find Among Us, Please open Among Us!\n";
      reloadFunk();
      goto reload;
   }
 
   while(true)
   {
      Steam:
      system("CLS");
      hGameWindow = FindWindow(NULL, "Among Us");
      if (hGameWindow != NULL)
      {
         std::cout << "Among Us found successfully!\n";
         std::cout << "---------------------------------------------------------------------------\n";
      }
      else
      {
         reloadFunk();
         goto reload;
      }

      GetWindowThreadProcessId(hGameWindow, &pID);
      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
      if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL)
      {
         std::cout << "Failed to open process! Try to run the application as administrator.\n";
         reloadFunk();
         goto reload;
      }

      char moduleName[] = "GameAssembly.dll";
      if (Steam)
      {
         offsetGameToBaseAddress = 0x01BB2D8C; //steam
      }
      else
      {
        offsetGameToBaseAddress = 0x01C71C08; // epic games
      }
      DWORD gameBaseAddress = dwGetModuleBaseAddress(_T(moduleName), pID);
      std::vector<DWORD> pointsOffsets{0x5C,0x2C};

      ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + offsetGameToBaseAddress), &baseAddress, sizeof(baseAddress), NULL);
      //std::cout << "Debugginfo: Baseaddress = " << std::hex << baseAddress << std::endl;
      DWORD pointsAddress = baseAddress;
      for (int i = 0; i < pointsOffsets.size() - 1; i++)
      {
         ReadProcessMemory(processHandle, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
         //std::cout << "Debugginfo: address at offset = " << std::hex << pointsAddress << std::endl;
      }
      pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);
      //std::cout << "Debugginfo: address at final offset = " << std::hex << pointsAddress << std::endl;
      //std::cout << "---------------------------------------------------------------------------\n";

      //"UI"
      std::cout << "Free Chat Unlocker by Vili\n";
      std::cout << "---------------------------------------------------------------------------\n";
      std::cout << "If the Program doesn't work Press Delete to reload it!\n";
      if (GetAsyncKeyState(VK_DELETE))
      {
         goto reload;
      }

      //memory write 
      WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &chatValue, sizeof(int), 0);
      CloseHandle(processHandle);
      Sleep(1000);
      goto Steam;
   }
}
