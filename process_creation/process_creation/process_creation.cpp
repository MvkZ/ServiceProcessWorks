// process_creation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <string>

void _tmain(int argc, TCHAR *argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hProcess = NULL;
	if (OpenProcess(PROCESS_ALL_ACCESS, FALSE, 1028))
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 1028);
		printf("Successful!!\n");
	}
	else
		printf("failed : %d\n", GetLastError());

	HANDLE hToken = NULL,phToken=NULL;

	 if(OpenProcessToken(GetCu, TOKEN_IMPERSONATE, &hToken))
		 printf("Successful!!\n");
	 else
		 printf("failed : %d\n", GetLastError());

	if (ImpersonateLoggedOnUser(hToken))
		printf("Successful!!\n");
	else
		printf("failed : %d\n",GetLastError());
	DuplicateTokenEx(phToken, NULL, NULL, (SECURITY_IMPERSONATION_LEVEL)NULL, TokenPrimary, &hToken);

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if (!CreateProcessAsUser(hToken,
		L"C:/Windows/notepad.exe",   // No module name (use command line)
		L"aa.txt",        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW | DETACHED_PROCESS,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Close process and thread handles. 
	CloseHandle(hToken);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	system("PAUSE");
}