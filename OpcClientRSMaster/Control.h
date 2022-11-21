/******************************************************************************

  Declaration CControl

  File:					Control.h

  Project:				OpcClientDPMaster

  Class:				Description:
  -----------------------------------------------------------------------------
  CControl				Class to control a process image

******************************************************************************/
#pragma once
#include "Thread.h"
#include "OPCServerMgt.h"
#include "OPCGroupMgt.h"
#include "OPCItemMgt.h"
#include "MemoryMap.h"

class CControl : public CThread
{
public:
	/** 
	* Constructor.
	*/
	CControl(void);
	/** 
	* Destructor.
	*/
	~CControl(void);

private:
	/** Pointer to the COPCServerMgt-object  ******************************/
	COPCServerMgt*	m_pOPCServerMgt;
	/** Pointer to the COPCGroupMgt-object  *******************************/
	COPCGroupMgt*	m_pOPCGroupMgt;
	/** Array of serverhandles. Contains all handles, needed to read ******/
	CUIntArray		m_aServerHandlesRead;
	/** Array of serverhandles. Contains all handles, needed to write *****/
	CUIntArray		m_aServerHandlesWrite;
	/** Memorymap to manage the process image *****************************/
	CMemoryMap		m_MemoryMap;
	/** Flag to check occured errors **************************************/
	BOOL			m_bOPCError;
	/** ThreadID of the Mainthread to send error-messages******************/
	DWORD			m_dwParentThreadID;
	/** for running light *************************************************/
	BYTE			m_nByte;
	/** for sinus function*************************************************/
	DOUBLE			m_fAngle;
	/** 
	*  Overridden init-method. Called once before DoWork() .
	*/
	void ThreadEnter(void);
	/** 
	*  Overridden work-method. Called once.  
	*/
	void DoWork(void);
	/** 
	*  Overridden init-method. Called once after DoWork(). 
	*/
	void ThreadLeave(void);
	/** 
	*  Reads values from OPC and updates memory map.
	*/
	void ReadMemoryMap(void);
	/** 
	*  Controlling.
	*/
	void ControlFunction(void);
	/** 
	*  Get values from memory map and write to OPC.
	*/
	void WriteMemoryMap(void);
	/** 
	*  Gets Arrays, as defined in ServerDefines.h.
	*/
	void BuildItemArrays(	DWORD dwItemCount,
							CStringArray*	astrItems,
							CUIntArray*		anClientHandles,
							CUIntArray*		anRequDatatypes);
	/** 
	*  Separates and stores the serverhandles.
	*/
	void StoreServerHandles(const CUIntArray &snServerHandle);
};

