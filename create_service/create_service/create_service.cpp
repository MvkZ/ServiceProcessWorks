// create_service.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<iostream>
#include<Windows.h>

using namespace std;

BOOL SvcInstall()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR szPath[MAX_PATH];

	if (!GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		printf("Cannot install service (%d)\n", GetLastError());
		return false;
	}

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return false;
	}

	// Create the service

	schService = CreateService(
		schSCManager,              // SCM database 
		L"CppWindowsService",                   // name of service 
		L"CppWindowsService Sample Service",                   // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_DEMAND_START,      // start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		szPath,                    // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,                      // no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 

	if (schService == NULL)
	{
		printf("CreateService failed (%d)\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return false;
	}
	else
	{
		printf("Service installed successfully\n");

	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (SvcInstall())
		cout << "Service successfully Started\n";
	else
		cout << "Service Failed\n";
	return 0;
}
