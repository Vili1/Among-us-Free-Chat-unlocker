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
	//currently that opcode isnt presesnt and its being handled with a jump instruction
	void* chatAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x75\x0C\xC7\x41\x10\x01\x00\x00\x00", "xxxxxxxxx");
	
	//patch the instruction
	if (chatAddr != nullptr)
	{
		// patches a jump
		byte patchChat[2] = {0x75, 0x00};
		PatchEx(hProcess, chatAddr, patchChat, sizeof(patchChat));
		std::cout << "The chat has been unlocked successfully!\n";
		std::cout << "---------------------------------------------------------------------------\n";
		system("pause");
	}

	
	return  0;
}
