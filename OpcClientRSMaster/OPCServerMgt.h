/******************************************************************************

  Declaration COPCServerMgt

  File:					OPCServerMgt.h

  Project:				OpcClient

  Class:				Description:
  -----------------------------------------------------------------------------
  COPCServerMgt			Class to manage an OPCServer object

******************************************************************************/

#pragma once
#include "OPCGroupMgt.h"

class COPCServerMgt
{
public:
	/** 
	* construction / destruction
	*/
	COPCServerMgt();
	~COPCServerMgt(void);
	
	/** 
	* Connects to the Opc server
	*/
	STDMETHODIMP ConnectOPCServer(const CString &ProgID);
	/** 
	* Disconnects from OPC server
	*/
	STDMETHODIMP DisconnectOPCServer(void);
 	/** 
	* Creates a OPCGroup object in the OPCServer object
	*/
	STDMETHODIMP AddGroup(COPCGroupMgt* pOPCGroup);				
	/** 
	* Removes a OPCGroup object from the OPC server
	*/
	STDMETHODIMP RemoveGroup(OPCHANDLE hOPCGroup);

private:
	/** Prohibit use of copy constructor and = operator ***********/
	COPCServerMgt(const COPCServerMgt& serverMgt){};
	COPCServerMgt& operator=(const COPCServerMgt& serverMgt){};

	/** Interface pointer IOPCServer of OPCServer object **********/
	IOPCServer * m_pIOPCServer;
	/** Connect state of the OPC server ***************************/
	BOOL         m_bConnectState;
};
