#pragma once
#include <Windows.h>

//internal patch
void Patch(void* dst, void* src, unsigned int size);

//external
void PatchEx(HANDLE hProcess, void* dst, void* src, unsigned int size);

//Internal Nop
void Nop(HANDLE hProcess, void* dest, unsigned int size);

//External
void NopEx(HANDLE hProcess, void* dst, unsigned int size);