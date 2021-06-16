#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h> 
#include <vector>


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


int main()
{
    reload:
    system("CLS");
    HWND hGameWindow = FindWindow(NULL, "Among Us");
    if (hGameWindow != NULL)
    {
        std::cout << "Among Us found successfully!" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
    }
    else
    {
        std::cout << "Unable to find Among Us, Please open Among Us!" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 5 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 4 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 3 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 2 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 1 seconds!" << std::endl;
        
        goto reload;
    }
    DWORD pID;
    GetWindowThreadProcessId(hGameWindow, &pID);
    HANDLE processHandle = NULL;
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL)
    {
        std::cout << "Failed to open process!" << std::endl;
        system("pause");
        return 0;
    }

    char moduleName[] = "GameAssembly.dll";
    DWORD gameBaseAddress = dwGetModuleBaseAddress(_T(moduleName), pID);
    DWORD offsetGameToBaseAddress = 0x01D22F60;
    std::vector<DWORD> pointsOffsets{0x5C,0x2C};
    DWORD baseAddress;

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
    //std::cout << "---------------------------------------------------------------------------" << std::endl;

    //"UI"
    std::cout << "Free Chat Unlocker by Vili " << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    int chatValue = 1;

    //memory write 
    WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &chatValue, sizeof(int), 0);
    Sleep(1000);
    goto reload;

}
