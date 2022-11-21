/******************************************************************************

  Implementation CErrorLookup

  File:					ErrorLookup.cpp

  Project:				OpcClientDPSlave

  Class:				Description:
  -----------------------------------------------------------------------------
  CErrorLookup			Implementation CErrorLookup

******************************************************************************/
#include "StdAfx.h"
#include "ErrorLookup.h"

/* ----------------------------------------------------------------------------
    Begin Class		CErrorLookup
-----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
    Class		CErrorLookup
	Method		GetErrorString
-----------------------------------------------------------------------------*/
CString CErrorLookup::GetErrorString(HRESULT hr)
{
	CString retval;
	switch(hr)
	{
		case S_OK:					retval = "S_OK";break;
		case S_FALSE:				retval = "S_FALSE";break;
		case REGDB_E_WRITEREGDB:	retval = "REGDB_E_WRITEREGDB";break;
		case CO_E_CLASSSTRING:		retval = "CO_E_CLASSSTRING. The progID string was improperly formatted.";break;
		case E_NOINTERFACE:			retval = "E_NOINTERFACE";break;
		case E_FAIL:				retval = "E_FAIL";break;
		case E_OUTOFMEMORY:			retval = "E_OUTOFMEMORY";break;
		case E_INVALIDARG:			retval = "E_INVALIDARG";break;
		case E_NOTIMPL:				retval = "E_NOTIMPL";break;
		case OPC_E_INVALIDHANDLE:	retval = "OPC_E_INVALIDHANDLE";break;
		case OPC_E_UNKNOWNPATH:		retval = "OPC_E_UNKNOWNPATH";break;
		case OPC_E_UNKNOWNITEMID:	retval = "OPC_E_UNKNOWNITEMID";	break;
		case OPC_E_INVALIDITEMID:	retval = "OPC_E_INVALIDITEMID"; break;
		case OPC_S_UNSUPPORTEDRATE:	retval = "OPC_S_UNSUPPORTEDRATE";break;
		case OPC_E_PUBLIC:			retval = "OPC_E_PUBLIC";break;
		case OPC_E_INVALIDFILTER:	retval = "OPC_E_INVALIDFILTER";break;
		case DISP_E_TYPEMISMATCH:	retval = "DISP_E_TYPEMISMATCH";break;
		case OPC_E_BADTYPE:			retval = "OPC_E_BADTYPE";break;
		case OPC_S_CLAMP:			retval = "OPC_S_CLAMP";break;
		case OPC_E_RANGE:			retval = "OPC_E_RANGE";break;
		case OPC_E_BADRIGHTS:		retval = "OPC_E_BADRIGHTS";break;
		case DISP_E_OVERFLOW:		retval = "DISP_E_OVERFLOW";break;
		case OPC_E_DUPLICATENAME:	retval = "OPC_E_DUPLICATENAME";break;
		case 0x800706ba:			retval = "RPC server unavailable";break; 
		case 0x80040154:			retval = "Class not registered.";break;
		case RPC_S_SERVER_UNAVAILABLE: retval = "RPC_S_SERVER_UNAVAILABLE";break;
		default:					retval.Format("Errorcode 0x%X not defined.",hr);
	}
	return retval;
}
/* ----------------------------------------------------------------------------
    Class		CErrorLookup
	Method		GetQuality
-----------------------------------------------------------------------------*/
CString CErrorLookup::GetQuality(WORD quality)
{
	CString retval;
	switch(quality)
	{
		case OPC_QUALITY_BAD:						retval = "OPC_QUALITY_BAD";break;
		case OPC_QUALITY_UNCERTAIN:					retval = "OPC_QUALITY_UNCERTAIN";break;
		case OPC_QUALITY_GOOD:						retval = "OPC_QUALITY_GOOD";break;
		case OPC_QUALITY_CONFIG_ERROR:				retval = "OPC_QUALITY_CONFIG_ERROR";break;
		case OPC_QUALITY_NOT_CONNECTED:				retval = "OPC_QUALITY_NOT_CONNECTED";break;
		case OPC_QUALITY_DEVICE_FAILURE:			retval = "OPC_QUALITY_DEVICE_FAILURE";break;
		case OPC_QUALITY_SENSOR_FAILURE:			retval = "OPC_QUALITY_SENSOR_FAILURE";break;
		case OPC_QUALITY_LAST_KNOWN:				retval = "OPC_QUALITY_LAST_KNOWN";break;
		case OPC_QUALITY_COMM_FAILURE:				retval = "OPC_QUALITY_COMM_FAILURE";break;
		case OPC_QUALITY_OUT_OF_SERVICE:			retval = "OPC_QUALITY_OUT_OF_SERVICE";break;
		case OPC_QUALITY_WAITING_FOR_INITIAL_DATA:	retval = "OPC_QUALITY_WAITING_FOR_INITIAL_DATA";break;
		case OPC_QUALITY_LAST_USABLE:				retval = "OPC_QUALITY_LAST_USABLE";break;
		case OPC_QUALITY_SENSOR_CAL:				retval = "OPC_QUALITY_SENSOR_CAL";break;
		case OPC_QUALITY_EGU_EXCEEDED:				retval = "OPC_QUALITY_EGU_EXCEEDED";break;
		case OPC_QUALITY_SUB_NORMAL:				retval = "OPC_QUALITY_SUB_NORMAL";break;
		case OPC_QUALITY_LOCAL_OVERRIDE:			retval = "OPC_QUALITY_LOCAL_OVERRIDE";break;
		default:									retval.Format("Qualitycode 0x%X not defined.",quality);break;
	}
	return retval;
}
/* ----------------------------------------------------------------------------
    Class		CErrorLookup
	Method		GetServerState
-----------------------------------------------------------------------------*/
CString CErrorLookup::GetServerState(OPCSERVERSTATE state)
{
	CString retval;
	switch(state)
	{
		case E_INVALIDARG:			retval = "E_INVALIDARG";break;
		case OPC_STATUS_RUNNING:	retval = "OPC_STATUS_RUNNING";break;
		case OPC_STATUS_FAILED:		retval = "OPC_STATUS_FAILED";break;
		case OPC_STATUS_COMM_FAULT:	retval = "OPC_STATUS_COMM_FAULT";break;
		case OPC_STATUS_NOCONFIG:	retval = "OPC_STATUS_NOCONFIG";break;
		case OPC_STATUS_SUSPENDED:	retval = "OPC_STATUS_SUSPENDED2";break;
		case OPC_STATUS_TEST:		retval = "OPC_STATUS_TEST";break;
		default:					retval.Format("Serverstate 0x%X not defined.",state);break;
	}
	return retval;
}
/* ----------------------------------------------------------------------------
    End Class		CErrorLookup
-----------------------------------------------------------------------------*/
