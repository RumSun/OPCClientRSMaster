/******************************************************************************

  Declaration CThread

  File:					Thread.h

  Project:				OpcClient	

  Class:                Description:
  -----------------------------------------------------------------------------
  CThread				Thread base class

******************************************************************************/
#pragma once

class CThread
{
public:
	virtual ~CThread(void);

	/** 
	 * Start Thread for Cyclic Read.
     */
	virtual void StartThread();
    /** 
	 * Stop Thread for Cyclic Read.
     */
	virtual void StopThread();
	/**
	 * Waits for shutdown-ready event
	 */
	void Wait(DWORD dwMilliseconds);
	/** 
	 * Get Run State of Thread.
     */
	inline BOOL GetThreadRunState() const { return m_bThreadIsRunning; };
protected:
	/** Shutdown handle  ******************************/
	HANDLE	m_hShutdown;
	/** 
	 * Called once before DoWork()
     */
	virtual void ThreadEnter(){};
	/** 
	 * Called once
     */
	virtual void DoWork() = 0;
	/** 
	 * Called once after DoWork()
     */
	virtual void ThreadLeave(){};

    /** 
	 * Constructor.
     */
	CThread();
private:
	/** Thread handle ***************************************/
	HANDLE	m_hThread;
	/** Shutdown-ready handle  ******************************/
	HANDLE	m_hShutdownReady;
	/** State of this Thread  *******************************/
	BOOL	m_bThreadIsRunning;
    /** 
	 * Starts Main Function of Thread.
     */
	static DWORD WINAPI  run( void* pv );

    /** 
	 * Main Function of Thread.
     */
	DWORD CThread::ThreadMain();
};
