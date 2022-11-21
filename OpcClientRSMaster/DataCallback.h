/******************************************************************************

  Declaration IDataCallback

  File:					DataCallback.h

  Project:				OpcClient	

  Class:                Description:
  -----------------------------------------------------------------------------
  IDataCallback			Interfacdeclaration

******************************************************************************/
#pragma once

class CDataCallback
{
public:
	virtual STDMETHODIMP DataChange(HRESULT				hrMasterquality,
									UINT				dwCount,
									const CUIntArray   &aClientHandles,
									const CVARIANTArray&aValues,
									const CUIntArray   &aQualities,
									HRESULT*			pErrors) = 0;
};
