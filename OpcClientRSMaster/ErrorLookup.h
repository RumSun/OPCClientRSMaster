/******************************************************************************

  Declaration CErrorLookup

  File:					ErrorLookup.h

  Project:				OpcClient	

  Class:                Description:
  -----------------------------------------------------------------------------
  CErrorLookup			Class to resolve HRESULT, OPCSERVERSTATE and OPCQuality 
						defines

******************************************************************************/

#pragma once

class CErrorLookup
{
public:
	/** 
	*  resolves OPC-HRESULTS to a human readable string
	*/
	static CString GetErrorString(HRESULT hr);
	/** 
	*  resolves OPC-SERVERSTATE to a human readable string
	*/
	static CString GetServerState(OPCSERVERSTATE state);
	/** 
	*  resolves OPC-QUALITY to a human readable string
	*/
	static CString GetQuality(WORD quality);
};
