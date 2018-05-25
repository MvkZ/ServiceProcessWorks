// wmi_example.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#define _WIN32_DCOM
#include <iostream>
#include <Windows.h>
#include <WbemIdl.h>
#pragma comment(lib, "wbemuuid.lib")
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	 IWbemLocator *pLoc = 0;
	 HRESULT hr;
     hr = CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hr)) 
	{ 
		cout << "Failed to initialize COM library. Error code = 0x"<< hex << hr << endl; 
		return hr;
	}

	hr =  CoInitializeSecurity(
    NULL,                        // Security descriptor    
    -1,                          // COM negotiates authentication service
    NULL,                        // Authentication services
    NULL,                        // Reserved
    RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication level for proxies
    RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation level for proxies
    NULL,                        // Authentication info
    EOAC_NONE,                   // Additional capabilities of the client or server
    NULL);                       // Reserved

	 hr = CoCreateInstance(CLSID_WbemLocator, 0, 
        CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &pLoc);

	 if (FAILED(hr))
     {
        cout << "Failed to create IWbemLocator object. Err code = 0x"
             << hex << hr << endl;
        CoUninitialize();
     }

	 IWbemServices *pSvc = 0;

	 hr = pLoc->ConnectServer(BSTR(L"ROOT\\cimv2"),NULL,NULL,0,NULL,0,0,&pSvc);

	 if (FAILED(hr))
	 {
        cout << "Could not connect. Error code = 0x" 
             << hex << hr << endl;
        pLoc->Release();
        CoUninitialize();
     }

	 cout << "Connected to WMI" << endl;

	 IEnumWbemClassObject *enumerator = 0;

	 hr = pSvc->ExecQuery(L"WQL",L"select * from Win32_Process",WBEM_FLAG_FORWARD_ONLY,NULL,&enumerator);

	ULONG uObjectCount = 0;
    IWbemClassObject *pclsObj;
    hr = enumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uObjectCount);
	ULONG uReturn;
	BSTR MethodName = SysAllocString(L"GetOwner");
	BSTR ClassName = SysAllocString(L"Win32_Process");


	while (enumerator)
	{
		HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;
		VARIANT vtProcName;
		VARIANT vtDomain;
		VARIANT vtUsername;

		// Get the value of the Name property
		hr = pclsObj->Get(L"Name", 0, &vtProcName, 0, 0);
		if (FAILED(hr))
		{
			printf("Failed to get the process's name\n");
			pclsObj->Release();
			continue;
		}

		// Get the PATH to the object in question
		// the result in vtProp is similar to '\\name_of_computer\ROOT\CIMV2:Win32_Process.Handle="pid_of_process"'
		hr = pclsObj->Get(L"__PATH", 0, &vtProp, 0, 0);
		if (FAILED(hr))
		{
			printf("Failed to get the path to the object\n");
			pclsObj->Release();
			continue;
		}

		// Execute Method against the object defined by the __PATH variable
		hr = pSvc->ExecMethod(vtProp.bstrVal, MethodName, 0, NULL, NULL, &pclsObj, NULL);
		if (FAILED(hr))
		{
			wprintf(L"Failed to execute the method against %s\n", vtProp.bstrVal);
			pclsObj->Release();
			continue;
		}

		// extract the results
		hr = pclsObj->Get(L"User", 0, &vtUsername, NULL, 0);
		if (FAILED(hr))
		{
			printf("Failed to get the owner's name\n");
			pclsObj->Release();
			continue;
		}

		pclsObj->Get(L"Domain", 0, &vtDomain, NULL, 0);
		if (FAILED(hr))
		{
			printf("Failed to get the owner's domain\n");
			pclsObj->Release();
			continue;
		}

		// print the output to screen
		wprintf(L"Process: %s. Domain\\User: %s\\%s\n", V_BSTR(&vtProcName), V_BSTR(&vtDomain), V_BSTR(&vtUsername));

		// release/cleanup resources we used this go around
		VariantClear(&vtProcName);
		VariantClear(&vtProp);
		VariantClear(&vtDomain);
		VariantClear(&vtUsername);
		pclsObj->Release();

	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	enumerator->Release();
	CoUninitialize();
	return 0;
}

