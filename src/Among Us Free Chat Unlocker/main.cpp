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

	void* petAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x2C\x00\x74\x04\xB0\x01\x5D", "xxxxxxx");
	if (petAddr != nullptr)
	{
		NopEx(hProcess, petAddr, 2);
	}

	void* hatAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x50\x00\x74\x04\xB0\x01\x5D", "xxxxxxx");
	if (hatAddr != nullptr)
	{
		NopEx(hProcess, hatAddr, 2);
	}

	void* skinAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x00\x00\x00\x00\x74\x04\xB0\x01\x5D", "xxxxxxxxx");
	if (skinAddr != nullptr)
	{
		NopEx(hProcess, skinAddr, 2);
	}

	void* visiorAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x40\x00\x74\x04\xB0\x01\x5D\xC3\x56", "xxxxxxxxx");
	if (visiorAddr != nullptr)
	{
		NopEx(hProcess, visiorAddr, 2);
	}

	void* namepAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x30\x00\x74\x04\xB0\x01\x5D", "xxxxxxx");
	if (namepAddr != nullptr)
	{
		NopEx(hProcess, namepAddr, 2);
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
