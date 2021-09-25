#include <Windows.h>
#include <iostream>
#include "proctools.h"
#include "patscan.h"
#include "memhack.h"

int main()
{
	//Get Process ID by enumerating the processes using tlhelp32snapshot
	DWORD processID = GetProcID(L"Among Us.exe");
	if (processID != NULL)
	{
		std::cout << "Among Us found successfully!\n";
		std::cout << "---------------------------------------------------------------------------\n";
	}
	else
	{
		std::cout << "Please make sure that Among Us is running!\n";
		std::cout << "---------------------------------------------------------------------------\n";
		system("pause");
		return  0;
	}
	//Get handle by OpenProcess
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
	if (hProcess == INVALID_HANDLE_VALUE || hProcess == NULL)
	{
		std::cout << "Try to run the application as administrator.\n";
		std::cout << "---------------------------------------------------------------------------\n";
		system("pause");
		return  0;
	}

	//PatternScan for the opcode that writes 2 to the chat value if its set to quick chat
	void* onlineAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x89\x41\x2C\xC7\x45\x0C\x00\x00\x00\x00\xC7", "xxxxxxxxxxx");

	//patch the instruction
	if (onlineAddr != nullptr)
	{
		// patches the instruction to write 2 in the play as guest value which tricks the game to let you change your name
		byte patchOnline[3] = { 0x89, 0x41, 0x38 };
		PatchEx(hProcess, onlineAddr, patchOnline, sizeof(patchOnline));
		std::cout << "The name changer has been unlocked successfully!\n";
		std::cout << "---------------------------------------------------------------------------\n";
		//system("pause");
	}

	//PatternScan for the chat compare opcode 
	void* chatAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x83\x78\x2C\x00\x75\x2E", "xxx?xx");

	//patch the instruction
	if (chatAddr != nullptr)
	{
		// patches the instruction that compares the chat mode to compare to 2 which is the restricted chat
		byte patchChat[4] = { 0x83, 0x78, 0x2C, 0x02 };
		PatchEx(hProcess, chatAddr, patchChat, sizeof(patchChat));
		std::cout << "The chat has been unlocked successfully!\n";
		std::cout << "---------------------------------------------------------------------------\n";
		system("pause");
	}
	
	return  0;
}
