#pragma once
#include <Windows.h>
#include <TlHelp32.h>

//Get Process ID From an executable name using toolhelp32Snapshot
DWORD GetProcID(wchar_t* exeName);

//Get ModuleEntry from module name, using toolhelp32snapshot
MODULEENTRY32 GetModule(DWORD dwProcID, wchar_t* moduleName);