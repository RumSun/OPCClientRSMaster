/******************************************************************************

  Declaration COPCDataCallback

  File:					OPCDataCallback.h

  Project:				OpcClient	

  Class:                Description:
  -----------------------------------------------------------------------------
  COPCDataCallback		The callback object for the callbacks of the
						OPC server.	

******************************************************************************/
#pragma once
#if !defined(OPCCALLBACK_H)
#define OPCCALLBACK_H

#include <comdef.h>
#include "DataCallback.h"
                   
class COPCDataCallback :
	public CComObjectRoot,
	public IOPCDataCallback
{
public:
	COPCDataCallback()
	{
		m_pDataCallback = 0;
	};
	~COPCDataCallback() {};

BEGIN_COM_MAP(COPCDataCallback)
	COM_INTERFACE_ENTRY(IOPCDataCallback)
END_COM_MAP()

#pragma warning( disable : 4100 )
// IOPCDataCallback 
	STDMETHODIMP  OnDataChange( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
        /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
        /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
        /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
        /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
    STDMETHODIMP  OnReadComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
        /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
        /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
        /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
        /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
	{
		return E_NOTIMPL;
	};
    STDMETHODIMP  OnWriteComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMastererr,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE __RPC_FAR *pClienthandles,
        /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
	{
		return E_NOTIMPL;
	};
    STDMETHODIMP  OnCancelComplete(/* [in] */ DWORD dwTransid,/* [in] */ OPCHANDLE hGroup) 
	{
		return E_NOTIMPL;
	};

STDMETHODIMP  Initialize(CDataCallback* pDataCallback) 
{
	m_pDataCallback = pDataCallback;
	return S_OK;
};


protected:
	// Member functions
	CDataCallback* m_pDataCallback;
};

#endif // OPCCALLBACK_H

