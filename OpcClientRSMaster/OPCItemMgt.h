/******************************************************************************

  Declaration COPCItemMgt

  File:					COPCItemMgt.h

  Project:				OpcClientDPMaster	

  Class:                Description:
  -----------------------------------------------------------------------------
  COPCItemMgt			Class to manage an OPC item object.

******************************************************************************/
#pragma once

class COPCItemMgt
{
public:
	/** construction / destruction */
	COPCItemMgt(void);
	~COPCItemMgt(void);
	/** copy constructor and = operator ***********/
	COPCItemMgt(const COPCItemMgt &itemMgt);
	COPCItemMgt& operator=(const COPCItemMgt &itemMgt);        
	
	/** get and set methods **/
	CString GetItemID() const;
	void	SetItemID(CString strItemID);
	VARIANT GetValue() const;
	void	SetValue(VARIANT strValue);
	VARTYPE GetDatatype() const;
	void	SetDatatype(VARTYPE type);
	UINT	GetServerHandle() const;
	void	SetServerHandle(UINT nServerHandle);
	UINT	GetControlID() const;
	void	SetControlID(UINT nControlID);
	
	/** property-declaration **/
	_declspec(property(get=GetItemID, put=SetItemID)) CString ItemID;
	_declspec(property(get=GetValue, put=SetValue)) VARIANT Value;
	_declspec(property(get=GetDatatype, put=SetDatatype)) VARTYPE Datatype;
	_declspec(property(get=GetServerHandle, put=SetServerHandle)) UINT ServerHandle;
	_declspec(property(get=GetControlID, put=SetControlID)) UINT ControlID;

private:
	//members
	/** ItemID of the item ****************************************/
	CString m_ItemID;
	/** Value of the item *****************************************/
	VARIANT m_varValue;
	/** Datatype of the item **************************************/
	VARTYPE	m_vartypeDatatype;
	/** Serverhandleof the item ***********************************/
	UINT	m_nServerHandle;
	/** ControlID of the dependant control of the item ************/
	UINT	m_nControlID;
};

// definition of COPCItemArray
typedef CArray<COPCItemMgt, COPCItemMgt&> COPCItemArray;
