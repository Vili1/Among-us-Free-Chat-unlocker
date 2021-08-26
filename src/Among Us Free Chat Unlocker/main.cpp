#include <Windows.h>
#include <iostream>
#include "proctools.h"
#include "patscan.h"
#include "memhack.h"

int main()
{
	//Get Process ID by enumerating the processes using tlhelp32snapshot
	DWORD processID = GetProcID(L"Among Us.exe");
	//Get handle by OpenProcess
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);

	//PatternScan
	void* chatAddr = PatternScanExModule(hProcess, L"Among Us.exe", L"GameAssembly.dll", "\x8b\x40\x00\xc3\xcc\xcc\x80\x3d", "xx?xxxxx");

	//Nop the instructions
	if (chatAddr != nullptr)
	{
		NopEx(hProcess, chatAddr, 3);
		std::cout << "Unlocked successfully!\n";
		system("pause");
	}

	return  0;
}