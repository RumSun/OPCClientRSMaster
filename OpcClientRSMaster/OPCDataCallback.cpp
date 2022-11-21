/******************************************************************************

  Implementation COPCDataCallback

  File:					OPCDataCallback.cpp

  Project:				OpcClient

  Class:                Description:
  -----------------------------------------------------------------------------
  COPCDataCallback		Implementation COPCDataCallback	

******************************************************************************/
#include "StdAfx.h"
#include ".\opcdatacallback.h"

/* ----------------------------------------------------------------------------
    Begin Class		COPCDataCallback
-----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
    Class		COPCDataCallback
	Method		OnDataChange
	Description	Encapsulates the received values and sends them to the UI.
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCDataCallback::OnDataChange(DWORD		dwTransid,		
											OPCHANDLE	hGroup,		
											HRESULT		hrMasterquality,
											HRESULT		hrMastererror,
											DWORD		dwCount,
											OPCHANDLE*	phClientItems,
											VARIANT*	pvValues,
											WORD*		pwQualities,
											FILETIME*	pftTimeStamps,
											HRESULT*	pErrors)
{
	HRESULT		  hr = S_OK;
	CVARIANTArray aValues;
	CUIntArray	  aClientHandles;
	CUIntArray	  aQualities;
	HRESULT h;
	for(UINT i=0; i<dwCount; i++)
	{
		if(pErrors[i] != S_OK)
			h = pErrors[i];
		aValues.Add(pvValues[i]);
		aClientHandles.Add(phClientItems[i]);
		aQualities.Add(pwQualities[i]);
	}
	hr = m_pDataCallback->DataChange(hrMasterquality,
										(UINT)dwCount,
										aClientHandles,
										aValues,
										aQualities,
										pErrors);
	return hr;
}

/* ----------------------------------------------------------------------------
    End Class		COPCDataCallback
	-----------------------------------------------------------------------------*/
