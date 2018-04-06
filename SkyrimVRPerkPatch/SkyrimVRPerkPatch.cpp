// ConsoleApplication8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

using namespace std;

DWORD_PTR GetProcessBaseAddress(DWORD processID);

int main(void) {

	int nVal = 72;

	HWND hWnd = FindWindowA(0, "Skyrim VR");
	if (hWnd == 0) {
		cerr << "Could not find window." << endl;
	}
	else {
		DWORD PID;
		GetWindowThreadProcessId(hWnd, &PID);
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

		if (!hProc) {
			cerr << "Cannot open process." << endl;
		}
		else {

			DWORD_PTR BaseAddress = GetProcessBaseAddress(PID);

			int stat = WriteProcessMemory(hProc, (LPVOID)(BaseAddress + 0x53F52B + 0x3), &nVal, (DWORD)sizeof(nVal), NULL);

			if (stat > 0) {
				clog << "Memory written to process." << endl;
			}
			else {
				cerr << "Memory couldn't be written to process." << endl;
			}

			CloseHandle(hProc);

			cin.get();

		}

	}

	return 0;
}

DWORD_PTR GetProcessBaseAddress(DWORD processID)
{
	DWORD_PTR   baseAddress = 0;
	HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	HMODULE     *moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;

	if (processHandle)
	{
		if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if (moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE *)moduleArrayBytes;

					if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}

					LocalFree(moduleArrayBytes);
				}
			}
		}

		CloseHandle(processHandle);
	}

	return baseAddress;
}