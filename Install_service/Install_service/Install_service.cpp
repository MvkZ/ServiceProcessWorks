// Install_service.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include<stdio.h>
#include<Windows.h>
#include<TlHelp32.h>
#include<comdef.h>
#include<Psapi.h>
#pragma warning(disable : 4996)
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SC_HANDLE serviceDbHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE serviceHandle = OpenService(serviceDbHandle, L"Themes", SC_MANAGER_ALL_ACCESS);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	SERVICE_STATUS_PROCESS status;
	DWORD bytesNeeded;
	QueryServiceStatusEx(serviceHandle, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);


	while (true)
	{
		Sleep(250);//reduce cpu usage
		CHAR __name[MAX_PATH];//name buffer
		HWND hwnd;//window handle
		DWORD pid;//process pid
		hwnd = FindWindow(NULL, NULL);//find any window
		PROCESSENTRY32 entry;//process structure containing info about processes
		entry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//get processes
		if (hwnd != 0)
		{
			GetWindowThreadProcessId(hwnd, &pid);//get found window pid
		}
		if (Process32First(snapshot, &entry) == TRUE)//start listing processes
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				const WCHAR* wc = entry.szExeFile;
				_bstr_t b(wc);
				const char* c = b;
				if (_stricmp(c, "explorer.exe") == 0)
				{
					if (pid != entry.th32ProcessID)//if found window pid is explorers one, skip it
					{
						HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);//open processusing PROCESS_ALL_ACCESS to get handle
						if (hProcess != NULL)
						{
							wchar_t wtext[20];
							mbstowcs(wtext, __name, strlen(__name) + 1);//Plus null
							LPWSTR ptr = wtext;
							GetModuleFileNameEx(hProcess, NULL, ptr, MAX_PATH);//get executable path
							cout << "Found: " << __name << endl;
						}
					}
				}
			}
		}
	}

	if (status.dwCurrentState == SERVICE_RUNNING)
	{// Stop it
		BOOL b = ControlService(serviceHandle, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&status);
		if (b)
		{
			std::cout << "Service stopped." << std::endl;
		}
		else { std::cout << "Service failed to stop." << std::endl; }
	}
	else {// Start it 
		BOOL b = StartService(serviceHandle, NULL, NULL); 
		if (b) 
		{ std::cout << "Service started." << std::endl;
		CreateProcess(L"C:/Windows/notepad.exe",   // No module name (use command line)
			L"aa.txt",        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW | DETACHED_PROCESS,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi);           // Pointer to PROCESS_INFORMATION structure;
		}
		else { std::cout << "Service failed to start." << std::endl; } 
	}
	CloseServiceHandle(serviceHandle); 
	CloseServiceHandle(serviceDbHandle);
	return 0; 
}