#pragma once
#include "game.h"

uintptr_t getProcID(const wchar_t* procName);
uintptr_t getModuleBaseAddress(uintptr_t const procID, const wchar_t* moduleName);

template <class val>
val readMem(uintptr_t const address)
{
	val value;
	ReadProcessMemory(game.hProcess, (BYTE*)address, &value, sizeof(value), nullptr);
	return value;
}

template <class val>
void writeMem(uintptr_t const address, val value)
{
	WriteProcessMemory(game.hProcess, (BYTE*)address, &value, sizeof(value), nullptr);
}