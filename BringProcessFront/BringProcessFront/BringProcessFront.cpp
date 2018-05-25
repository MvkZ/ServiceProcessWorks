// BringProcessFront.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

BOOL CALLBACK EnumWindowsProc(HWND windowHandle, LPARAM lParam)  {
	DWORD searchedProcessId = (DWORD)lParam;  // This is the process ID we search for (passed from BringToForeground as lParam)
	DWORD windowProcessId = 0;
	GetWindowThreadProcessId(windowHandle, &windowProcessId); // Get process ID of the window we just found
	if (searchedProcessId == windowProcessId)  {  // Is it the process we care about?
		SetForegroundWindow(windowHandle);  // Set the found window to foreground
		return FALSE;  // Stop enumerating windows
	}
	return TRUE;  // Continue enumerating
}

void BringToForeground(DWORD processId)  {
	EnumWindows(&EnumWindowsProc, (LPARAM)processId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD procid = 8512;
	BringToForeground(procid);
	printf("the proc id : %d\n", procid);
	return 0;
}

