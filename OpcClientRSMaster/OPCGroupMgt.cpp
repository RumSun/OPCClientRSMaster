/******************************************************************************

  Implementation COPCGroupMgt

  File:					OPCGroupMgt.cpp

  Project:				OpcClient

  Class:				Description:
  -----------------------------------------------------------------------------
  COPCServerMgt			Class to manage an OPCGroup object

******************************************************************************/

#include "StdAfx.h"
#include "OPCGroupMgt.h"

/* ----------------------------------------------------------------------------
    Begin Class		COPCServerMgt
	constructors / destructors
-----------------------------------------------------------------------------*/
COPCGroupMgt::COPCGroupMgt(CString strName, UINT nUpdateRate, BOOL bActive, FLOAT fDeadband)
{
	// Initialize members
	m_strName				= strName;
	m_nUpdateRate			= nUpdateRate;
	m_bActive				= bActive;
	m_fDeadband				= fDeadband;
	m_hOPCGroup				= NULL;
	m_dwCookie				= NULL;
	m_pIOPCItemMgt			= NULL;
	m_pIOPCGroupStateMgt	= NULL;
	m_pIOPCSyncIO			= NULL;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		Destructor
	Description	Cleans up any allocated memory and releases all COM-objects.
-----------------------------------------------------------------------------*/
COPCGroupMgt::~COPCGroupMgt(void)
{
	// Release all interface pointer for this OPCGroup object
	if(m_pIOPCItemMgt != NULL)
	{
		// Disconnect data callback
		DisconnectCallback();
		// Release interface IOPCItemMgt
		m_pIOPCItemMgt->Release();
		m_pIOPCItemMgt = NULL;
	}
	if(m_pIOPCSyncIO != NULL)
	{
		// Release interface IOPCSyncIO
		m_pIOPCSyncIO->Release();
		m_pIOPCSyncIO = NULL;
	}
	if(m_pIOPCGroupStateMgt != NULL)
	{
		// Release interface IOPCGroupStateMgt
		m_pIOPCGroupStateMgt->Release();
		m_pIOPCGroupStateMgt = NULL;
	}
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		Initialize
	Description	Stores OPC server and group-handle to be able to remove the 
				group. Stores the given IOPCItemMgt-pointer and gets all other
				neede Interface-pointer using QueryInterface.
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::Initialize(IOPCItemMgt * pIOPCItemMgt, 
                                      DWORD         dwRevisedUpdateRate, 
                                      OPCHANDLE     hOPCGroup)
{
	ASSERT(pIOPCItemMgt != NULL);
	
	HRESULT	hr		= S_OK;

	m_nUpdateRate	= (UINT)dwRevisedUpdateRate;
	m_hOPCGroup		= hOPCGroup;

	// Add a reference to the given IOPCItemMgt pointer and save it to the member
	// This interface will be used for adding and removing items
	pIOPCItemMgt->AddRef();
	m_pIOPCItemMgt = pIOPCItemMgt;

	// Get IOPCGroupStateMgt interface pointer 
	// This interface will be used for group managment e.g. activate/deactivate group
	hr = m_pIOPCItemMgt->QueryInterface(IID_IOPCGroupStateMgt, (void**)&m_pIOPCGroupStateMgt);
	// Get IOPCSyncIO interface pointer 
	// This interface will be used for synchronous read/write operations
	hr = m_pIOPCItemMgt->QueryInterface(IID_IOPCSyncIO, (void**)&m_pIOPCSyncIO);

	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		AddItems
	Description	Adds all given items using the given client-handles to the group.
				Stores the received server-handles in the out-parameter 
				aServerHandles (needed for writing).
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::AddItems(UINT                 nItemCount,
									const CStringArray & astrItems, 
									const CUIntArray   & aRequestedDatatypes,
									const CUIntArray   & anClientHandles,
									CUIntArray         & anServerHandles,
									CHRESULTArray      & ahrErrors)	
{
	HRESULT			hr				= S_OK;
	DWORD			i				= 0;

	// IN parameter of AddItems call
	OPCITEMDEF*		pOPCItemDef		= NULL;

	// OUT parameters of AddItems call
	OPCITEMRESULT*	pOPCItemResult	= NULL;
	HRESULT*		pErrors			= NULL;			
	
	// Init IN parameter
	pOPCItemDef = new OPCITEMDEF[nItemCount];
	// Fill OPCITEMDEF array
	for (i = 0; i < nItemCount; i++)
	{
		pOPCItemDef[i].szAccessPath        = L"";
		pOPCItemDef[i].szItemID            = AllocString(astrItems[i]);
		pOPCItemDef[i].bActive             = TRUE;
		pOPCItemDef[i].hClient             = (OPCHANDLE)anClientHandles[i];
		pOPCItemDef[i].dwBlobSize          = 0;
		pOPCItemDef[i].pBlob               = NULL;
		pOPCItemDef[i].vtRequestedDataType = aRequestedDatatypes[i];
	}

	// *****************************************************************************************
	// OPC call
	// Add items to group
	hr = m_pIOPCItemMgt->AddItems(	nItemCount,			// Item count
									pOPCItemDef,		// OPCITEMDEF array
									&pOPCItemResult,	// Returned OPCITEMRESULT array
									&pErrors);			// Returned HRESULT array
	// *****************************************************************************************

	// Check the result of the AddItems call
	// Macro SUCCEEDED is used to check if the return code of AddGroup was a succeeded code
	// Possible return codes:
	// S_OK		-> The operation succeeded	(->SUCCEEDED(hr) is true)
	// S_FALSE	-> The operation completed with one or more errors.
	//				Refer to individual error returns for failure analysis
	//				(->SUCCEEDED(hr) is true)
	// E_FAIL	-> The operation failed		(->SUCCEEDED(hr) is false)
	// E_INVALIDARG -> An argument to the function was invalid	(->SUCCEEDED(hr) is false)
	if (SUCCEEDED(hr))
	{
		// Copy the received server-handles and the HRESULTs to the OUT parameters of the method
		for(i=0; i<nItemCount; i++)
		{
			anServerHandles.Add(pOPCItemResult[i].hServer);		
			ahrErrors.Add(pErrors[i]);
		}
  	}

	// Free memory of IN and OUT parameters
	if (pOPCItemDef)      
	{
		for (i = 0; i < nItemCount; i++)
			if(pOPCItemDef[i].szItemID!= NULL)
				CoTaskMemFree(pOPCItemDef[i].szItemID);
		delete [] pOPCItemDef;
	}
	if(pErrors != NULL)
	{
		// pErrors is the array of error codes for each item returned by the server
		// The memory was allocated as COM memory by the server
		// This is neccesary if process A allocates the memory and process B frees the memory
		// CoTaskMemFree frees COM memory
		CoTaskMemFree(pErrors);
	}
	if(pOPCItemResult!= NULL)
	{
		// pOPCItemResult is the array of result for each item returned by the server
		// The memory was allocated as COM memory by the server
		// This is neccesary if process A allocates the memory and process B frees the memory
		// CoTaskMemFree frees COM memory
		CoTaskMemFree(pOPCItemResult);
	}

	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		RemoveItems
	Description	Removes the items, identified by the given serverhandles from 
				the OPC group.
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::RemoveItems(UINT nItemCount,
									   const CUIntArray &anServerHandles, 
									   CHRESULTArray &ahrErrors)
{
	HRESULT		hr				= S_OK;
	// IN parameter of RemoveItems call
	OPCHANDLE*	phServerHandles = NULL;
	// OUT parameters of RemoveItems call
	HRESULT*	pErrors			= NULL;
	
	// Init IN parameter
	phServerHandles = new OPCHANDLE[nItemCount];
	// Fill item  handle array
	for(UINT i = 0; i < nItemCount; i++)
	{
		phServerHandles[i] = anServerHandles[i];
	}

	// *****************************************************************************************
	// OPC call
	// Remove items from group
	hr = m_pIOPCItemMgt->RemoveItems( nItemCount,		// Item count
									  phServerHandles,	// Item  handle array -> Items to remove
									  &pErrors);		// Returned HRESULT array
	// *****************************************************************************************

	// Check the result of the RemoveItems call
	if (SUCCEEDED(hr))	
	{
		// Copy the received HRESULTs to the OUT parameter of the method
		for(UINT i=0; i< nItemCount; i++)
			ahrErrors.Add(pErrors[i]);
	}
	
	// Free memory of IN and OUT parameters
	if(phServerHandles != NULL)
	{
		delete [] phServerHandles;
	}
	if(pErrors != NULL)
	{
		// pErrors is the array of error codes for each item returned by the server
		// The memory was allocated as COM memory by the server
		// This is neccesary if process A allocates the memory and process B frees the memory
		// CoTaskMemFree frees COM memory
		CoTaskMemFree(pErrors);
	}
	
	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		Read
	Description	
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::Read(UINT nItemCount,
								const CUIntArray &anServerHandles, 
								CVARIANTArray &avarValues,
								CHRESULTArray &ahrErrors)
{
	HRESULT			hr				= S_OK;
	// IN parameter of Read call
	DWORD*			dwServerHandles = NULL;
	// OUT parameters of Read call
	OPCITEMSTATE*	itemStates		= NULL;
	HRESULT*		pErrors			= NULL;
	
	// Init IN parameter
	dwServerHandles = new DWORD[nItemCount];
	// Fill list of server item handles
	for(UINT i = 0; i < nItemCount; i++)
	{
		dwServerHandles[i] = anServerHandles[i];
	}
	
	// *****************************************************************************************
	// OPC call
	// Read value, quality, timestamp of items
	hr = m_pIOPCSyncIO->Read( OPC_DS_DEVICE,	// The ‘data source’; OPC_DS_CACHE or OPC_DS_DEVICE
							  nItemCount,		// The number of items to be read
							  dwServerHandles,	// The list of server item handles for the items to be read
							  &itemStates,		// Array of structures in which the item values are returned
							  &pErrors);		// Returned HRESULT array
	// *****************************************************************************************

	// Check the result of the Read call
	if(SUCCEEDED(hr))
	{
		// Copy the received values and the HRESULTs to the OUT parameters of the method
		for(UINT i=0;i<nItemCount;i++)
		{
			// Copy values
			VARIANT tmpvar;
			VariantInit(&tmpvar);
			// Check error code of each item
			// Copy value only if return code is SUCCEEDED
			if(SUCCEEDED(pErrors[i]))
			{
				// copy value
				HRESULT hr = VariantCopy(&tmpvar, &itemStates[i].vDataValue);
            }
			avarValues.Add(tmpvar);

			// Copy HRESULT for each item
			ahrErrors.Add(pErrors[i]);
		}
	}

	// Free memory of IN and OUT parameters
	if(dwServerHandles)
	{
		delete dwServerHandles;
	}
	if(pErrors != NULL)
	{
		// pErrors is the array of error codes for each item returned by the server
		// The memory was allocated as COM memory by the server
		// This is neccesary if process A allocates the memory and process B frees the memory
		// CoTaskMemFree frees COM memory
		CoTaskMemFree(pErrors);
	}
	if(itemStates != NULL)
	{
		// ItemState contains the value as VARIANT
		// The variant in each ItemState must be freed 
		for(UINT i=0;i<nItemCount;i++)
		{
			// VariantClear frees the memory allocated by the VARIANT
			VariantClear(&itemStates[i].vDataValue);
		}
		// CoTaskMemFree frees COM memory for the ItemState array
		CoTaskMemFree(itemStates);
	}

	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		Write
	Description	Writes the given values according to the items, specified by 
				the given server-handle-array.
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::Write(UINT nItemCount,
								 const CUIntArray &anServerHandles,
								 const CVARIANTArray &avarValues,
								 CHRESULTArray &ahrErrors)
{
	HRESULT		hr				= S_OK;
	// IN parameters of Write call
	OPCHANDLE*	phServerHandles = NULL;
	VARIANT*	pItemValues		= NULL;
	// OUT parameter of Write call
	HRESULT*	pErrors			= NULL;
	
	// Init IN parameter
	phServerHandles = new OPCHANDLE[nItemCount];
	pItemValues		= new VARIANT[nItemCount];
	// Fill list of server item handles and list of values to write
	for (UINT i = 0; i < nItemCount; i++)
	{
		pItemValues[i] = avarValues[i];
		phServerHandles[i] = anServerHandles[i];
	}

	// *****************************************************************************************
	// OPC call
	// Write values to the items defined by the item server handles
	hr = m_pIOPCSyncIO->Write( nItemCount,		// The number of items to be written
							   phServerHandles, // The list of server item handles for the items to be written
							   pItemValues,		// List of values to be written to the items
							   &pErrors);		// Returned HRESULT array
	// *****************************************************************************************

	// Check the result of the Write call
	if(SUCCEEDED(hr))
	{
		// Copy the received HRESULTs to the OUT parameter of the method
		for(UINT i=0; i< nItemCount; i++)
		{
			ahrErrors.Add(pErrors[i]);
		}
	}

	// Free memory of IN and OUT parameters
	if(pItemValues != NULL)
		delete [] pItemValues;
	if(phServerHandles != NULL)
		delete [] phServerHandles;
	if(pErrors != NULL)
	{
		// pErrors is the array of error codes for each item returned by the server
		// The memory was allocated as COM memory by the server
		// This is neccesary if process A allocates the memory and process B frees the memory
		// CoTaskMemFree frees COM memory
		CoTaskMemFree(pErrors);
	}

	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		ConnectCallback
	Description	TODO
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::ConnectCallback(CDataCallback* pDataCallback)
{
	HRESULT hr = S_OK;
	
	// Create Callback object
	CComObject<COPCDataCallback>* pIOPCDataCallback = NULL;
	CComObject<COPCDataCallback>::CreateInstance(&pIOPCDataCallback);

	// Init Callback object with pointer of the target object
	pIOPCDataCallback->Initialize(pDataCallback);

	// Advise callback connection
	hr = AtlAdvise(m_pIOPCItemMgt,pIOPCDataCallback,IID_IOPCDataCallback,&m_dwCookie);
	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		DisconnectCallback
	Description	TODO
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::DisconnectCallback(void)
{
	HRESULT hr = S_OK;
	
	// Unadvise callback connection
	hr = AtlUnadvise(m_pIOPCItemMgt,IID_IOPCDataCallback,m_dwCookie);
	
	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		SetActive
	Description	Sets the state of the group. 
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCGroupMgt::SetActive(BOOL bActive = true)
{
	ASSERT(m_pIOPCGroupStateMgt != NULL);	
	
	HRESULT	hr						= S_OK;
	DWORD	dwRevisedUpdateRate		= 0;

	// *****************************************************************************************
	// OPC call
	// Set group active state to the given state
	hr = m_pIOPCGroupStateMgt->SetState(NULL,
										&dwRevisedUpdateRate, 
										&bActive,
										NULL,
										NULL,
										NULL,
										NULL);
	// *****************************************************************************************

	// Check the result of the SetState call
	if(SUCCEEDED(hr))
	{
		m_nUpdateRate = (UINT)dwRevisedUpdateRate;
		m_bActive = bActive;
	}

	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetUpdateRate
-----------------------------------------------------------------------------*/
UINT COPCGroupMgt::GetUpdateRate()
{
	return m_nUpdateRate;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetGroupHandle
-----------------------------------------------------------------------------*/
OPCHANDLE COPCGroupMgt::GetGroupHandle(void)
{
	return m_hOPCGroup;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetName
-----------------------------------------------------------------------------*/
CString COPCGroupMgt::GetName(void)
{
	return m_strName;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		IsActive
-----------------------------------------------------------------------------*/
BOOL COPCGroupMgt::IsActive(void)
{
	return m_bActive;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetDeadband
-----------------------------------------------------------------------------*/
FLOAT COPCGroupMgt::GetDeadband(void)
{
	return m_fDeadband;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		AllocString
	Description	Converts a given CString in a WCHAR*.
				Do not forget to free the memory, allocated by this	function.
				Free memory with CoTaskMemFree
-----------------------------------------------------------------------------*/
WCHAR* COPCGroupMgt::AllocString(const CString & strIn)
{
   WCHAR  * strTemp = NULL;
   DWORD       dwLength = strIn.GetLength();
    
   if( dwLength > 0 )
   {
      DWORD dwTemp = sizeof(WCHAR)*(dwLength+1);
      if ( (strTemp = (WCHAR *) CoTaskMemAlloc(dwTemp) ) != NULL )
      {
         for(DWORD i=0; i<dwLength; i++)
         {
            strTemp[i] = (WCHAR)strIn.GetAt(i);
         }
         strTemp[dwLength] = 0;
      }
   }

   return strTemp;
}

/* ---------------------------------------------------------------------------
    End Class		COPCGroupMgt
-----------------------------------------------------------------------------*/


