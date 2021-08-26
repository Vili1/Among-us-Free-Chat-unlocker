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

	//PatternScan
	void* chatAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x8b\x40\x00\xc3\xcc\xcc\x80\x3d", "xx?xxxxx");

	//Nop the instructions
	if (chatAddr != nullptr)
	{
		NopEx(hProcess, chatAddr, 3);
		std::cout << "The chat has been unlocked successfully!\n";
		std::cout << "---------------------------------------------------------------------------\n";
		system("pause");
	}

	return  0;
}
