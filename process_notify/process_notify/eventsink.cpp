#include "eventsink.h"
#include "stdafx.h"

ULONG EventSink::AddRef()
{
    return InterlockedIncrement(&m_lRef);
}

ULONG EventSink::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRef);
    if(lRef == 0)
        delete this;
    return lRef;
}

HRESULT EventSink::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
    {
        *ppv = (IWbemObjectSink *) this;
        AddRef();
        return WBEM_S_NO_ERROR;
    }
    else return E_NOINTERFACE;
}


HRESULT EventSink::Indicate(long lObjectCount,
    IWbemClassObject **apObjArray)
{
	HRESULT hres = S_OK;
	_variant_t vtProp;

    for (int i = 0; i < lObjectCount; i++)
    {
        bool CreateorDel = false;
        _variant_t cn;
        hres = apObjArray[i]->Get(_bstr_t(L"__Class"), 0, &cn, 0, 0);
        if (SUCCEEDED(hres))
        {
            wstring LClassStr(cn.bstrVal);
            if (0 == LClassStr.compare(L"__InstanceCreationEvent") )
            {
                wcout << "Creation" << endl;
                CreateorDel = true;
            }
			if (0 == LClassStr.compare(L"__InstanceDeletionEvent") )
            {
                wcout << "Deletion" << endl;
                CreateorDel = true;
            }
		}
	 if (CreateorDel)
        {
            hres = apObjArray[i]->Get(_bstr_t(L"TargetInstance"), 0, &vtProp, 0, 0);
            if (!FAILED(hres))
            {
                IUnknown* str = vtProp;
                hres = str->QueryInterface( IID_IWbemClassObject, reinterpret_cast< void** >( &apObjArray[i] ) );
                if ( SUCCEEDED( hres ) )
                {
                    _variant_t cn;
                    hres = apObjArray[i]->Get( L"Name", 0, &cn, NULL, NULL );
                    if ( SUCCEEDED( hres ) )
                    {

                        if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
                            wcout << "Name : " << ((cn.vt==VT_NULL) ? "NULL" : "EMPTY") << endl;
                        else
                            wcout << "Name : " << cn.bstrVal << endl;
                    }
                    VariantClear(&cn);              
                    VariantClear(&cn);
                }
            }
            VariantClear(&vtProp);
        }

    }
    return WBEM_S_NO_ERROR;
}

//HRESULT EventSink::Indicate(long lObjectCount,
//    IWbemClassObject **apObjArray)
//{
// HRESULT hres = S_OK;
//
//    for (int i = 0; i < lObjectCount; i++)
//    {
//        printf("Close Event occurred\n");
//    }
//
//    return WBEM_S_NO_ERROR;
//}

HRESULT EventSink::SetStatus(
            /* [in] */ LONG lFlags,
            /* [in] */ HRESULT hResult,
            /* [in] */ BSTR strParam,
            /* [in] */ IWbemClassObject __RPC_FAR *pObjParam
        )
{
    if(lFlags == WBEM_STATUS_COMPLETE)
    {
        printf("Call complete. hResult = 0x%X\n", hResult);
    }
    else if(lFlags == WBEM_STATUS_PROGRESS)
    {
        printf("Call in progress.\n");
    }

    return WBEM_S_NO_ERROR;
}    // end of EventSink.cpp
