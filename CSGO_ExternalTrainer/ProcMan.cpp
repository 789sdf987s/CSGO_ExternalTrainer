#include "stdafx.h"
#include "ProcMan.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

uintptr_t getProcID(const wchar_t* procName)
{
	// Creating snapshot of the process currently running in the system
	auto const hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	uintptr_t procID = 0;

	// Check if creating a snapshot failed
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to take snapshot \n";
		return false;
	}
	else
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32);

		while (Process32Next(hSnap, &procEntry))
		{
			// Display the name of the exe file
			std::wcout << procEntry.szExeFile << "\n";
			// Comparing processes names
			if (!_wcsicmp(procEntry.szExeFile, procName))
			{
				std::wcout << "Found process! " << procEntry.szExeFile << " With proc ID " << procEntry.th32ProcessID << "\n";
				procID = procEntry.th32ProcessID;
			}
		}
		// Checking if procID is valid
		if (procID == 0) 
		{
			std::cout << "Couldn't find process in the snapshot \n";
			return false;
		}
	}
	CloseHandle(hSnap);
	return procID;
}

uintptr_t getModuleBaseAddress(uintptr_t const procID, const wchar_t* moduleName)
{
	// Creating snapshot of the modules from the specified process
	auto const hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	uintptr_t moduleAddr = 0;

	if (hSnap == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to take snapshot of the modules \n";
		return false;
	}
	else
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);

		while (Module32Next(hSnap, &moduleEntry))
		{
			std::wcout << moduleEntry.szModule << "\n";
			if (!_wcsicmp(moduleEntry.szModule, moduleName))
			{
				std::cout << "Found module " << moduleEntry.szModule << "\n";
				moduleAddr = (uintptr_t)moduleEntry.modBaseAddr;
			}
		}
		if (moduleAddr == 0)
		{
			std::cout << "Couldn't find module in the snapshot \n";
			return false;
		}
	}
	CloseHandle(hSnap);
	return moduleAddr;
}