// User_process_creation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Windows.h>
#include<stdio.h>
#include<NTSecAPI.h>
#include <Sddl.h>

void printTokenUserSid(HANDLE hToken)
{
	PTOKEN_USER ptu = NULL;
	DWORD dwSize = 0;
	if (!GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize)
		&& ERROR_INSUFFICIENT_BUFFER != GetLastError())
	{
		return;
	}
	if (NULL != (ptu = (PTOKEN_USER)LocalAlloc(LPTR, dwSize)))
	{
		LPTSTR StringSid = NULL;
		if (!GetTokenInformation(hToken, TokenUser, ptu, dwSize, &dwSize))
		{
			LocalFree((HLOCAL)ptu);
			return;
		}
		if (ConvertSidToStringSid(ptu->User.Sid, &StringSid))
		{
			_tprintf(_T("%s\n"), StringSid);
			LocalFree((HLOCAL)StringSid);
			LocalFree((HLOCAL)ptu);
			return;
		}
		LocalFree((HLOCAL)ptu);
	}
	return;
}


int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb=sizeof(STARTUPINFOW);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	CreateProcessWithLogonW(L"TestUser", NULL, L"Laptop@123", 0, L"C:\\windows\\notepad.exe", NULL, 0, NULL, NULL, &si, &pi);

	HANDLE hToken = NULL;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		printf("Token is successfully Obtained\n");
		printTokenUserSid(hToken);
	}
	else
	{
		printf("Something failed, Error Code: %d \n", GetLastError());
	}

	CloseHandle(hToken);
	return 0;
}

