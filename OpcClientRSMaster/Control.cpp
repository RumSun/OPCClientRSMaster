/******************************************************************************

  Implementation CControl

  File:					Control.cpp

  Project:				OpcClientDPMaster

  Class:				Description:
  -----------------------------------------------------------------------------
  CControl				Implementation CControl

******************************************************************************/
#include "StdAfx.h"
#include ".\control.h"
#include "Math.h"

/* ----------------------------------------------------------------------------
    Begin Class		CControl

	constructors / destructors
-----------------------------------------------------------------------------*/
CControl::CControl(void)
{
	m_pOPCServerMgt = NULL;
	m_pOPCGroupMgt	= NULL;
	m_bOPCError		= FALSE;
	m_dwParentThreadID = GetCurrentThreadId();
	m_nByte			= 0;
	m_fAngle		= 0;
}

CControl::~CControl(void)
{
	m_aServerHandlesWrite.RemoveAll();
	m_aServerHandlesRead.RemoveAll();
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		ThreadEnter
	Description	Overridden base class method. Connects to the OPC server, adds 
				a group and adds items to the group using the defines of 
				ServerDefines.h.
-----------------------------------------------------------------------------*/
void CControl::ThreadEnter()
{
	HRESULT		hr			=	S_OK;

	// Get total item count form defines in ServerDefines.h
	DWORD		dwItemCount	=	OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT +
								OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT +
								OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT +
								OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT +
								OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT +
								OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT;
	
  	// Initialize COM
	CoInitialize(NULL);

	// Create new OPCServerMgt object and connect to OPC server defined in ServerDefines.h
	m_pOPCServerMgt = new COPCServerMgt();
	hr = m_pOPCServerMgt->ConnectOPCServer((CString)OPCCLIENT_CONTROLFUNCTION_SERVER_PROGID);

	// Check if connect succeeded
	if(SUCCEEDED(hr))
	{
		// Create new OPCGroupMgt object and add OPCGroup to the OPC server
		m_pOPCGroupMgt  = new COPCGroupMgt(	(CString)OPCCLIENT_CONTROLFUNCTION_GROUP_NAME,
											1000,
											FALSE);
		hr = m_pOPCServerMgt->AddGroup(m_pOPCGroupMgt);
	
		// Check if AddGroup succeeded
		if(SUCCEEDED(hr))
		{
			// Initialize the item information arrays
			CStringArray astrItem;       // Array of ItemIDs - ItemIDs defined in ServerDefines.h
			CUIntArray anRequDatatype;   // Array of requested data types - defined in ServerDefines.h
			CUIntArray anClientHandle;   // Array of client item handles - index of the item in the array
			BuildItemArrays(dwItemCount,&astrItem,&anClientHandle,&anRequDatatype);
			
			CUIntArray anServerHandle;   // Array of client item handles - returned from AddItems
			CHRESULTArray ahrErrors;     // Array of result codes for each item - returned from AddItems
			
			// Add items to the group
			hr = m_pOPCGroupMgt->AddItems(  dwItemCount,    // Item count
											astrItem,       // ItemIDs
											anRequDatatype,	// Requested data types  
											anClientHandle, // Client handles
											anServerHandle, // Returned server handles
											ahrErrors);     // Returnded item results
			if(SUCCEEDED(hr))
			{
				// Check for one or more invalid itemIDs
				if(hr == S_FALSE )
					PostThreadMessage(m_dwParentThreadID,WM_CONTROLTHREAD_WRONGITEMID,0,0);
				// Store the serverhandles of the items
				StoreServerHandles(anServerHandle);
			}
			else
				m_bOPCError = TRUE;
		}
		else
		{
			PostThreadMessage(m_dwParentThreadID,WM_CONTROLTHREAD_ADDGROUP_FAILED,0,0);
			m_bOPCError = TRUE;
		}
	}
	else
	{
		PostThreadMessage(m_dwParentThreadID,WM_CONTROLTHREAD_WRONGPROGID,0,0);
		m_bOPCError = TRUE;
	}

	// Initialize memory map
	m_MemoryMap.Initialize();
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		DoWork
	Description	Calls cyclic ReadMemoryMap, ControlFunction and WriteMemoryMap.
				Cycletime is defined in ServerDefines.h.
				Loop will be cancelled by the shutdown-event, set by 
				StopThread-method of the base class.
-----------------------------------------------------------------------------*/
void CControl::DoWork(void)
{
	// Get the shutdown handle from base class
	HANDLE hHandle[1];
	hHandle[0] = m_hShutdown;
	DWORD retval = -1;

	if (m_bOPCError == FALSE)
	{
		// Submit error state once
		while(!PostThreadMessage(m_dwParentThreadID,WM_CONTROLTHREAD_OPCERROR_DROPOUT,0,0)){}
		
		BOOL bLastErrorState = FALSE;
		
		// While not m_hShutdown is signaled
		while (retval != WAIT_OBJECT_0) 
		{
			/***  Read PAE ********************************/
			ReadMemoryMap();
			/**********************************************/
			if(m_bOPCError == FALSE)
				/***  Control Function ************************/
				ControlFunction();
				/**********************************************/
			if(m_bOPCError == FALSE)
				/***  Write PAA *******************************/
				WriteMemoryMap();
				/**********************************************/
			
			// Error state changed?
			if(bLastErrorState != m_bOPCError)
			{
				if(m_bOPCError == TRUE)
				{
					// Submit error state
					while(!PostThreadMessage(m_dwParentThreadID,WM_CONTROLTHREAD_OPCERROR,0,0)){}
					bLastErrorState = m_bOPCError;
				}
				else
				{
					// Submit error state
					while(!PostThreadMessage(m_dwParentThreadID,WM_CONTROLTHREAD_OPCERROR_DROPOUT,0,0)){}
					bLastErrorState = m_bOPCError;
				}
			}
			retval = MsgWaitForMultipleObjects(1, hHandle, FALSE, OPCCLIENT_THREAD_CYCLETIME, 0);
		}
	}
	else
	{
		while(!PostThreadMessage(m_dwParentThreadID,WM_CONTROLTHREAD_FINISHED,0,0)){}
	}
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		ThreadLeave
	Description	Removes the group from server. Deletes the 2 OPC-Members. 
				Any cleanup is done by themself.
-----------------------------------------------------------------------------*/
void CControl::ThreadLeave()
{
	if(m_pOPCGroupMgt != NULL)
	{
		// Remove all items from OPC group
		CUIntArray	aServerHandles;	
		CHRESULTArray aErrors;
		int nItemCount = 0;
		for( int i = 0; i < m_aServerHandlesRead.GetSize(); i++)
		{
			aServerHandles.Add(m_aServerHandlesRead[i]);
			nItemCount++;
		}
		for( int i = 0; i < m_aServerHandlesWrite.GetSize(); i++)
		{
			aServerHandles.Add(m_aServerHandlesWrite[i]);
			nItemCount++;
		}
		m_pOPCGroupMgt->RemoveItems(nItemCount,aServerHandles,aErrors);

		// Store the group-handle to remove the group from OPC server
		OPCHANDLE hGroup = m_pOPCGroupMgt->GetGroupHandle();

		// The COPCGroupMgt destructor will clean up any resources, which 
		// were allocated by member functions.
		delete m_pOPCGroupMgt;
		m_pOPCGroupMgt = NULL;
		
		// Remove the OPC group
		m_pOPCServerMgt->RemoveGroup(hGroup);
	}
	if(	m_pOPCServerMgt != NULL)
	{
		m_pOPCServerMgt->DisconnectOPCServer();
		// The COPCServerMgt destructor will clean up any resources, which 
		// were allocated by member functions.
		delete m_pOPCServerMgt;
		m_pOPCServerMgt = NULL;
	}	
	
	m_aServerHandlesWrite.RemoveAll();
	m_aServerHandlesRead.RemoveAll();
	
	CoUninitialize();
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		ControlFunction
	Description	
-----------------------------------------------------------------------------*/
void CControl::ControlFunction(void)
{
	if(m_MemoryMap.GetInBool(4,0) == FALSE)
	{
		// Set output-byte value bitwise to input byte value
		for(int i=0;i<8;i++)
			m_MemoryMap.SetOutBool(8,i,m_MemoryMap.GetInBool(4,i));
	}
	else
	{
		// Set output byte value
		m_MemoryMap.SetOutByte(8,m_nByte);
		m_nByte = m_nByte << 1;
		if(m_nByte < 128)
			m_nByte++;
	}
	
	// Set out word 0 to value of in word 0
	WORD w0 = m_MemoryMap.GetInWord(0);
	m_MemoryMap.SetOutWord(0,w0);

	// Set out word 2 to value of word 2
	WORD w2 = m_MemoryMap.GetInWord(2);
	m_MemoryMap.SetOutWord(2,w2);

	// Set out word 4 to sin. InByte sets frequency.
	DOUBLE inByte = (DOUBLE)m_MemoryMap.GetInByte(4);
	DOUBLE sinus = sin(m_fAngle += inByte/2000);
    WORD w4 = (WORD)(sinus*1000 + 1000.5);
	m_MemoryMap.SetOutWord(4,w4);

	if(m_fAngle>360)
		m_fAngle = 0;

	// Set out word 6 to average value of words 0, 2 and 4
	WORD w6 = (w0 + w2 + w4) / 3;
	m_MemoryMap.SetOutWord(6,w6);
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		ReadMemoryMap
	Description	Reads all values from OPC server and stores them in the 
				memory map (WriteInput).
-----------------------------------------------------------------------------*/
void CControl::ReadMemoryMap(void)
{
	HRESULT			hr				= S_OK;
	CWordArray		aWords;
	CByteArray		aBytes;
	CVARIANTArray	aValues;
	CHRESULTArray	aErrors;
	
	// Read from OPC server
	hr = m_pOPCGroupMgt->Read( (UINT)m_aServerHandlesRead.GetSize(),
							   m_aServerHandlesRead,
							   aValues,
							   aErrors);
	if(hr == S_OK)
	{
		// Write inputs to memory map
		m_MemoryMap.WriteInput(aValues);
		m_bOPCError = FALSE;
	}
	else
		m_bOPCError = TRUE;

	// Clean up memory
	for(int i = 0; i < aValues.GetSize(); i++)
	{
		VariantClear(&aValues[i]);
	}
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		WriteMemoryMap
	Description	Gets all values from the memory map (ReadOutput) and writes 
				them to the OPC server
-----------------------------------------------------------------------------*/
void CControl::WriteMemoryMap(void)
{
	CVARIANTArray	aValues;
	// Read outputs from memory map
	m_MemoryMap.ReadOutput(aValues);
	
	HRESULT			hr				= S_OK;
	CHRESULTArray	aErrors;;
	UINT			nItemCount		= (UINT)aValues.GetSize();

	// Write to OPC server
	hr = m_pOPCGroupMgt->Write(nItemCount,m_aServerHandlesWrite,aValues,aErrors);
	if(hr != S_OK)
		m_bOPCError = TRUE;

	// Clean up memory
	for(UINT i = 0; i < nItemCount; i++)
		hr = VariantClear(&aValues[i]);
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		BuildItemArrays
	Description	Fills the given Arrays. 
				The item-array is filled with item-IDs from ServerDefines.h.
				Order:
				1) DWORDs - input
				2) DWORDs - output
				3) WORDs - input
				4) WORDs - output
				5) BYTEs - input
				6) BYTEs - output
-----------------------------------------------------------------------------*/
void CControl::BuildItemArrays(	DWORD dwItemCount,
								CStringArray*	astrItems,
								CUIntArray*		anClientHandles,
								CUIntArray*		anRequDatatypes)
{
	//Clienthandles
	for(DWORD i=0; i< dwItemCount; i++)
		anClientHandles->Add(i);


		//Analog Input
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM1);
	anRequDatatypes->Add(VT_UI2);
	/*
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM2);
	anControlIDs->Add(IDC_EDITWORD1);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM3);
	anControlIDs->Add(IDC_EDITWORD2);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM4);
	anControlIDs->Add(IDC_EDITWORD3);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM5);
	anControlIDs->Add(IDC_EDITWORD4);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM6);
	anControlIDs->Add(IDC_EDITWORD5);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

	//Analog Output
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM1);
	anControlIDs->Add(IDC_EDITWORD6);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	//8 Bit input
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM1);
	anControlIDs->Add(IDC_STATICBITINPUT1);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);*/
	////2 x WORD Input
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_WORD_IN1);
	//anRequDatatypes->Add(VT_UI2);
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_WORD_IN2);
	//anRequDatatypes->Add(VT_UI2);
	//
	////4 x WORD Output
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_WORD_OUT1);
	//anRequDatatypes->Add(VT_UI2);
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_WORD_OUT2);
	//anRequDatatypes->Add(VT_UI2);
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_WORD_OUT3);
	//anRequDatatypes->Add(VT_UI2);
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_WORD_OUT4);
	//anRequDatatypes->Add(VT_UI2);

	////8 Bit Input / 8 Bit Output
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_BYTE_IN1);
	//anRequDatatypes->Add(VT_UI1);
	//
	//astrItems->Add((CString)OPCCLIENT_CONTROLFUNCTION_BYTE_OUT1);
	//anRequDatatypes->Add(VT_UI1);
}
/* ----------------------------------------------------------------------------
    Class		CControl
	Method		StoreServerHandles
	Description	Separates the received severhandles and stores them in the 
				member arrays. The indices depend on the order of their entries 
				in CControl::BuildItemArrays(...).
-----------------------------------------------------------------------------*/
void CControl::StoreServerHandles(const CUIntArray &anServerHandle)
{
	int i = 0;
	// input DWORDs
	for( ; i< OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT; )
		m_aServerHandlesRead.Add(anServerHandle[i++]);
	// output DWORDs
	for( ;i <OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT; )
		m_aServerHandlesWrite.Add(anServerHandle[i++]);
	// input WORDs
	for( ;i <OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT; )
		m_aServerHandlesRead.Add(anServerHandle[i++]);
	// output WORDs
	for( ;i <OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT +
			OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT; )
		m_aServerHandlesWrite.Add(anServerHandle[i++]);
	// input BYTEs
	for( ;i < OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT +
			OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT +
			OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT; )
		m_aServerHandlesRead.Add(anServerHandle[i++]);
	// output BYTEs
	for( ;i < OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT +
			OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT +
			OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
			OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT; )
		m_aServerHandlesWrite.Add(anServerHandle[i++]);
}
/* ---------------------------------------------------------------------------
    End Class		CControl
-----------------------------------------------------------------------------*/
