// StartStop_Service.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include<stdio.h>
#include<Windows.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SC_HANDLE serviceDbHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE serviceHandle = OpenService(serviceDbHandle, L"New Service", SC_MANAGER_ALL_ACCESS);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	SERVICE_STATUS_PROCESS status;
	DWORD bytesNeeded;
	QueryServiceStatusEx(serviceHandle, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);

	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		BOOL b = ControlService(serviceHandle, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&status);
		if (b)
		{
			std::cout << "Service stopped." << std::endl;
		}
		else { std::cout << "Service failed to stop." << std::endl; }
	}
	else {
		BOOL b = StartService(serviceHandle, NULL, NULL); 
		if (b) 
		{ std::cout << "Service started." << std::endl;
		}
		else { std::cout << "Service failed to start." << std::endl; } 
	}
	CloseServiceHandle(serviceHandle); 
	CloseServiceHandle(serviceDbHandle);
	return 0; 
}