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

	//PatternScan for the chat compare opcode 
	void* chatAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x83\xB8\xA0\x00\x00\x00\x00\x75\x31", "xxxxxx?xx");
	
	//patch the instruction
	if (chatAddr != nullptr)
	{
		// patches the instruction that compares the chat mode to compare to 2 which is the restricted chat
		byte patchChat[7] = { 0x83, 0xB8, 0xA0, 0x00, 0x00, 0x00, 0x02 };
		PatchEx(hProcess, chatAddr, patchChat, sizeof(patchChat));
		std::cout << "The chat has been unlocked successfully!\n";
		std::cout << "---------------------------------------------------------------------------\n";
		system("pause");
	}

	
	return  0;
}
