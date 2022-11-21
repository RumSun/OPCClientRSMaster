/******************************************************************************

  Implementation COPCItemMgt

  File:					COPCItemMgt.cpp

  Project:				OpcClientDPMaster	

  Class:                Description:
  -----------------------------------------------------------------------------
  COPCItemMgt			Implementation COPCItemMgt

******************************************************************************/
#include "StdAfx.h"
#include ".\opcitemmgt.h"
/* ----------------------------------------------------------------------------
    Begin Class		COPCServerMgt

	constructors / destructors
-----------------------------------------------------------------------------*/
COPCItemMgt::COPCItemMgt(void)
{
	VariantInit(&Value);
}

COPCItemMgt::~COPCItemMgt(void)
{
	VariantClear(&Value);
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		copyconstructor
-----------------------------------------------------------------------------*/
COPCItemMgt::COPCItemMgt(const COPCItemMgt &itemMgt)
{
	m_ItemID			= itemMgt.ItemID;
	m_vartypeDatatype	= itemMgt.Datatype;
	m_nServerHandle		= itemMgt.ServerHandle;
	m_nControlID		= itemMgt.ControlID;
	VariantCopy(&m_varValue,&itemMgt.Value);
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		 = operator
-----------------------------------------------------------------------------*/
COPCItemMgt& COPCItemMgt::operator=(const COPCItemMgt &itemMgt)
{
	ItemID				= itemMgt.ItemID;
	Datatype			= itemMgt.Datatype;
	ServerHandle		= itemMgt.ServerHandle;
	ControlID			= itemMgt.ControlID;
	VariantCopy(&Value,&itemMgt.Value);

	return *this;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetItemID
-----------------------------------------------------------------------------*/
CString COPCItemMgt::GetItemID()const
{	
	return m_ItemID;	
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		SetItemID
-----------------------------------------------------------------------------*/
void COPCItemMgt::SetItemID(CString strItemID)
{
	m_ItemID = strItemID;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetValue
-----------------------------------------------------------------------------*/
VARIANT COPCItemMgt::GetValue()const
{	
	return m_varValue;	
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		SetValue
-----------------------------------------------------------------------------*/
void COPCItemMgt::SetValue(VARIANT varValue)
{
	m_varValue = varValue;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetDatatype
-----------------------------------------------------------------------------*/
VARTYPE COPCItemMgt::GetDatatype()const
{
	return m_vartypeDatatype;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		SetDatatype
-----------------------------------------------------------------------------*/
void COPCItemMgt::SetDatatype(VARTYPE type)
{
	m_vartypeDatatype = type;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetServerHandle
-----------------------------------------------------------------------------*/
UINT COPCItemMgt::GetServerHandle()const
{ 
	return m_nServerHandle;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		SetServerHandle
-----------------------------------------------------------------------------*/
void COPCItemMgt::SetServerHandle(UINT nServerHandle)
{ 
	m_nServerHandle = nServerHandle;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		GetControlID
-----------------------------------------------------------------------------*/
UINT COPCItemMgt::GetControlID()const
{ 
	return m_nControlID;
}
/* ----------------------------------------------------------------------------
    Class		COPCGroupMgt
	Method		SetControlID
-----------------------------------------------------------------------------*/

void COPCItemMgt::SetControlID(UINT nControlID)
{ 
	m_nControlID = nControlID;
}
/* ---------------------------------------------------------------------------
    End Class		COPCServerMgt
-----------------------------------------------------------------------------*/