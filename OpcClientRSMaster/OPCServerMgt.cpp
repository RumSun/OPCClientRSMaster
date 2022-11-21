/******************************************************************************

  Implementation COPCServerMgt

  File:					OPCServerMgt.cpp

  Project:				OpcClient

  Class:				Description:
  -----------------------------------------------------------------------------
  COPCServerMgt			Class to manage an OPCServer object

******************************************************************************/
#include "StdAfx.h"
#include "OPCServerMgt.h"
#include <comdef.h>

/* ----------------------------------------------------------------------------
    Begin Class	COPCServerMgt
	constructors / destructors
-----------------------------------------------------------------------------*/
COPCServerMgt::COPCServerMgt()
{
	// Initialize members
	m_pIOPCServer   = NULL;
	m_bConnectState = FALSE;
}

COPCServerMgt::~COPCServerMgt(void)
{
	// Check ConnectState
	// State must be disconnected
	if(m_bConnectState == TRUE)
		DisconnectOPCServer();
}
/* ----------------------------------------------------------------------------
    Class		COPCServerMgt
	Method		ConnectOPCServer
	Description	Gets the ClassID from the given ProgID and creates an Instance 
				of the OPC server.
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCServerMgt::ConnectOPCServer(const CString &ProgID)
{
	// Check ConnectState
	// State must be disconnected
	if(m_bConnectState == TRUE)
		return E_FAIL;
	ASSERT(m_pIOPCServer == NULL);

	HRESULT hr		= S_OK;
	CLSID	clsid;
	
	USES_CONVERSION;
	// Get the CLSID of the COM object from the ProgID of the COM object (OPC Server)
	// e.g.
	// ProgID = OPC.SimaticNET
	// CLSID  = {B6EACB30-42D5-11D0-9517-0020AFAA4B3C}
	hr = CLSIDFromProgID(T2COLE((LPCTSTR)ProgID) ,&clsid);
	if(SUCCEEDED(hr))
	{
		// Create COM object with the CLSID-> connect to OPC Server
		// Returns the interface IOPCServer
		hr = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IOPCServer, (void**)&m_pIOPCServer);
		if(hr == S_OK)
		{
			// Set ConnectState to true
			m_bConnectState = TRUE;
		}
	}

	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCServerMgt
	Method		AddGroup
	Description	Adds a group according to the given COPCGroupMgt-object.
				Parameter bActive specifies the state of the group.
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCServerMgt::AddGroup(COPCGroupMgt* pOPCGroup)
{
	// Check in parameter
	ASSERT(pOPCGroup != NULL);
	if(pOPCGroup == NULL)
		return E_POINTER;

	// Check ConnectState
	// State must be connected
	if(m_bConnectState == FALSE)
		return E_FAIL;

	HRESULT		hr						= S_OK;

	// IN parameters of AddGroup call
	// BEGIN Get OPCGroup settings from passed OPCGroupMgt object
	DWORD		dwUpdateRate			= pOPCGroup->GetUpdateRate();
	BOOL		bActive					= pOPCGroup->IsActive();
	LPWSTR		strName					= pOPCGroup->AllocString(pOPCGroup->GetName());
	FLOAT		fDeadband				= pOPCGroup->GetDeadband();
	// END Get OPCGroup settings from passed OPCGroupMgt object
	LCID		lcidLanguageSettings	= LOCALE_SYSTEM_DEFAULT;

	// OUT parameters of AddGroup call
	DWORD		dwRevisedUpdateRate		= 0;
	OPCHANDLE	hOPCGroup				= NULL;
	IOPCItemMgt* pIOPCItemMgt			= NULL;

	// *****************************************************************************************
	// OPC call
	// Add OPCGroup object to the OPCServer object
	hr = m_pIOPCServer->AddGroup(strName,	           	    // Group name
								bActive,                   	// Active state
								dwUpdateRate,           	// Requested update rate
								1,                      	// Client handle group
								NULL,                   	// Time Bias - Difference to UTC in minutes
								0,                   	    // Percent Deadband
								lcidLanguageSettings,   	// LocaleID
								&hOPCGroup,             	// Server handle group
								&dwRevisedUpdateRate,   	// Revised update rate
								IID_IOPCItemMgt,			// Requested Interface
								(LPUNKNOWN*)&pIOPCItemMgt);	// Returned interface pointer
	// *****************************************************************************************
	
	// Check the result of the AddGroup call
	// Macro SUCCEEDED is used to check if the return code of AddGroup was a succeeded code
	// Possible return codes:
	// S_OK -> The operation succeeded	(->SUCCEEDED(hr) is true)
	// OPC_S_UNSUPPORTEDRATE -> Server does not support specified rate,  
	//							server returns the rate that it can support 
	//							in the revised update rate		(->SUCCEEDED(hr) is true)
	// E_FAIL -> The operation failed	(->SUCCEEDED(hr) is false)
	// E_INVALIDARG -> An argument to the function was invalid	(->SUCCEEDED(hr) is false)
	// OPC_E_DUPLICATENAME -> Duplicate name not allowed		(->SUCCEEDED(hr) is false)
	if(SUCCEEDED(hr))
	{
		// Initialize OPCGroupMgt object with returned IOPCItemMgt interface pointer,
		// group handle and revised update rate.
		pOPCGroup->Initialize(pIOPCItemMgt, dwRevisedUpdateRate, hOPCGroup);
		// Release local reference to IOPCItemMgt interface
		// OPCGroupMgt object has incremented the reference count
		pIOPCItemMgt->Release();
		pIOPCItemMgt = NULL;
	}

	// Free the allocated memory for the string strName
	CoTaskMemFree(strName);

	return hr;
}
/* ----------------------------------------------------------------------------
    Class		COPCServerMgt
	Method		RemoveGroup
	Description	
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCServerMgt::RemoveGroup(OPCHANDLE hOPCGroup)
{
	// Check ConnectState
	// State must be connected
	if(m_bConnectState == FALSE)
		return E_FAIL;
	
	// Removes the OPCGoup object from the OPCServer object with the handle of the group
	return m_pIOPCServer->RemoveGroup(hOPCGroup, FALSE);
}
/* ----------------------------------------------------------------------------
    Class		COPCServerMgt
	Method		DisconnectOPCServer
	Description	Releases the Instance of the OPC server
-----------------------------------------------------------------------------*/
STDMETHODIMP COPCServerMgt::DisconnectOPCServer()
{
	// Check ConnectState
	// State must be connected
	if(m_bConnectState == FALSE)
		return E_FAIL;

	if(m_pIOPCServer)
	{
		// Release the interface IOPCServer
		// This release call to the interface pointer 
		// disconnects the OPC client from the OPC server
		OPCSERVERSTATUS* status = NULL;
		m_pIOPCServer->Release();
		m_pIOPCServer = NULL;
		m_bConnectState = FALSE;
	}
	else
	{
		ASSERT(TRUE);
	}

	return S_OK;
}
/* ---------------------------------------------------------------------------
    End Class		COPCServerMgt
-----------------------------------------------------------------------------*/