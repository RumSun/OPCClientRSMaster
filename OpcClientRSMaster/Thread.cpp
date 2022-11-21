/******************************************************************************

  Implementation CThread

  File:					Thread.cpp

  Project:				OpcClient

  Class:                Description:
  -----------------------------------------------------------------------------
  CThread				Implementation CThread

******************************************************************************/
#include "StdAfx.h"
#include ".\thread.h"
/* ----------------------------------------------------------------------------
    Begin Class		CThread

	constructors / destructors
-----------------------------------------------------------------------------*/
CThread::CThread(void)
{
	m_bThreadIsRunning	= FALSE;
	m_hThread			= NULL;
	m_hShutdown			= NULL;
	m_hShutdownReady	= NULL;
}

CThread::~CThread(void)
{
	if(m_hShutdown != NULL)
	{
		CloseHandle(m_hShutdown);
		m_hShutdown = NULL;
	}
	if(m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if(m_hShutdownReady != NULL)
	{
		CloseHandle(m_hShutdownReady);
		m_hShutdownReady = NULL;
	}
}
/* ----------------------------------------------------------------------------
    Class		CThread
	Method		StartThread
	Description	Creates the thread.
				Creates events to shutdown the thread. If there is a message loop 
				(e.g. while(WaitFor..))in the DoWork-method of the derived 
				class implemented the Handle m_hShutdown must be checked. 
-----------------------------------------------------------------------------*/
void CThread::StartThread(void)
{
	if(m_bThreadIsRunning == TRUE)
		return;

	if(m_hShutdown != NULL)
	{
		CloseHandle(m_hShutdown);
		m_hShutdown = NULL;
	}
	if(m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if(m_hShutdownReady != NULL)
	{
		CloseHandle(m_hShutdownReady);
		m_hShutdownReady = NULL;
	}

	DWORD id;
	m_hShutdown		 = CreateEvent(NULL, FALSE, FALSE, NULL);	// Used to signal Shutdown!
	m_hShutdownReady = CreateEvent(NULL, FALSE, FALSE, NULL);	// Used to signal Shutdown Ready!
	
	m_hThread = CreateThread(NULL,0, run, (void*)this ,  NULL, &id );
	m_bThreadIsRunning = TRUE;
}
/* ----------------------------------------------------------------------------
    Class		CThread
	Method		StopThread
	Description	Sets the shutdown-event to stop the Thread. If there is a message 
				loop (e.g. while(WaitFor..))in the DoWork-method of the derived 
				class implemented the Handle m_hShutdown must be checked.
-----------------------------------------------------------------------------*/
void CThread::StopThread(void)
{
	if(m_bThreadIsRunning == FALSE)
		return;
	// Send Message
	SetEvent(m_hShutdown);
}
/* ----------------------------------------------------------------------------
    Class		CThread
	Method		Wait
	Description	Waits for shutdown-ready event, signaled by ThreadMain.
-----------------------------------------------------------------------------*/
void CThread::Wait(DWORD dwMilliseconds)
{
	if(m_bThreadIsRunning == FALSE)
		return;
	
	WaitForSingleObject(m_hShutdownReady, dwMilliseconds);
}
/* ----------------------------------------------------------------------------
    Class		CThread
	Method		run
	Description	
-----------------------------------------------------------------------------*/
DWORD CThread::run(void* pv)
{
	return ((CThread*)pv)->ThreadMain();
}
/* ----------------------------------------------------------------------------
    Class		CThread
	Method		ThreadMain
	Description	The main method of the Thread. Calls sequential ThreadEnter,
				DoWork and ThreadLeave. Finally sets the shutdown-ready-event.
-----------------------------------------------------------------------------*/
DWORD CThread::ThreadMain()
{
	m_bThreadIsRunning = TRUE;
	
	ThreadEnter();
	
	DoWork();

	ThreadLeave();

	m_bThreadIsRunning = FALSE;
	
	if(m_hShutdownReady)	
		SetEvent(m_hShutdownReady);
	
	return 0;
}
/* ---------------------------------------------------------------------------
    End Class		CThread
-----------------------------------------------------------------------------*/