/******************************************************************************

  Declaration COPCGroupMgt

  File:					OPCGroupMgt.h

  Project:				OpcClient

  Class:				Description:
  -----------------------------------------------------------------------------
  COPCServerMgt			Class to manage an OPCGroup object

******************************************************************************/

#pragma once
#include "OPCDataCallback.h"

class COPCGroupMgt
{
public:
	/** 
	* construction / destruction
	*/
	COPCGroupMgt(CString strName, UINT nUpdateRate, BOOL bActive, FLOAT fDeadband = 0);
	~COPCGroupMgt(void);

	/** 
	* Initializes the OPCGroupMgt object
	*/
	STDMETHODIMP Initialize(IOPCItemMgt* pIOPCItemMgt, 
							DWORD dwRevisedUpdateRate,
							OPCHANDLE hOPCGroup);
	/** 
	* Adds items to the group
	*/
	STDMETHODIMP AddItems(	UINT nCount,
							const CStringArray &astrItems,
							const CUIntArray &aRequestedDatatypes,
							const CUIntArray &anClientHandles,
							CUIntArray &anServerHandles, 
							CHRESULTArray &ahrErrors);
	/** 
	* Remove items from the group
	*/
	STDMETHODIMP RemoveItems(	UINT nItemCount, 
								const CUIntArray &anServerHandles, 
								CHRESULTArray &ahrErrors);
	/** 
	* Reads item-values synchronous from OPC server. 
	*/
	STDMETHODIMP Read(	UINT nItemCount,
						const CUIntArray &anServerHandles, 
						CVARIANTArray &avarValues,
						CHRESULTArray &ahrErrors);
	/** 
	* Writes item-values synchronous to OPC server
	*/
	STDMETHODIMP Write(	UINT nCount,
						const CUIntArray &anServerHandles,
						const CVARIANTArray &avarValues,
						CHRESULTArray &ahrErrors);
	/** 
	* Connects to DataChange event of the OPC server
	*/
	STDMETHODIMP ConnectCallback(CDataCallback* pDataCallback);
	/** 
	* Disconnects from DataChange of the OPC server
	*/
	STDMETHODIMP DisconnectCallback(void);
	/**
	* Sets the active state of this OPC group
	*/
	STDMETHODIMP SetActive(BOOL bActive);
	/** Get wide character string from CString ********************/
	static WCHAR*	AllocString(const CString & strIn);

	//Get-methods
	UINT		GetUpdateRate(void);
	CString		GetName(void);
	BOOL		IsActive(void);
	OPCHANDLE	GetGroupHandle(void);
	FLOAT		GetDeadband(void);

private:
	/** Prohibit use of copy constructor and = operator ***********/
	COPCGroupMgt(const COPCGroupMgt& groupMgt){};
	COPCGroupMgt& operator=(const COPCGroupMgt& groupMgt){};

	/** Name of the group *****************************************/
	CString				m_strName;
	/** Update rate of the group **********************************/
	UINT				m_nUpdateRate;
	/** Active state of the group *********************************/
	BOOL				m_bActive;
	/** Deadband of the group *************************************/
	FLOAT				m_fDeadband;
	/** OPC server handle of the group ****************************/
	OPCHANDLE			m_hOPCGroup;
	/** Cookie for the callback connection ************************/
	DWORD				m_dwCookie;
	/** Interface pointer IOPCItemMgt of OPCGroup object **********/
	IOPCItemMgt*		m_pIOPCItemMgt;
	/** Interface pointer IOPCGroupStateMgt of OPCGroup object ****/
   	IOPCGroupStateMgt*	m_pIOPCGroupStateMgt;
	/** Interface pointer IOPCSyncIO of OPCGroup object ***********/
	IOPCSyncIO*			m_pIOPCSyncIO;
};
