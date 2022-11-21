// UserInterface.h : header file
//

#pragma once

#include "OPCServerMgt.h"
#include "OPCGroupMgt.h"
#include "OPCItemMgt.h"
#include "DataCallback.h"
//#include "Control.h"
#include "afxwin.h"

// CUserInterface dialog
class CUserInterface : public CDialog, public CDataCallback
{
// Construction
public:
	CUserInterface(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OPCCLIENTRSMASTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnConnectClick();


private:
	// Private data
	/** Pointer to a OPCServerMgt object *********************************/
	COPCServerMgt*	m_pOPCServerMgt;
	/** Pointer to a OPCGroupMgt object **********************************/
	COPCGroupMgt*	m_pOPCGroupMgt;
	/** Array of OPCItemMgt objects  *************************************/
	COPCItemArray	m_aOPCItemMgt;
	/** String to display messages to the User  **************************/
	CString			m_strOutput;
	/** CBrushes for control painting  ***********************************/
	CBrush			m_brushRed;
	CBrush			m_brushGreen;

	/** State of the Controlthread ***************************************/
	BOOL			m_bControlFunctionError;
	/** Pointer to CCOntrolClass/Workerthread ****************************/
//	CControl*		m_pControl;
public:
	afx_msg void OnBtnDisconnectClick();
	afx_msg void OnDestroy();
private:
	void BuildItemArrays(DWORD dwItemCount, CStringArray* astrItems, CUIntArray* anClientHandles, CUIntArray* anRequDatatypes, CUIntArray* anControlIDs);
public:
	STDMETHODIMP DataChange(HRESULT hrMasterquality,
		UINT dwCount, 
		const CUIntArray & aClientHandles, 
		const CVARIANTArray& aValues, 
		const CUIntArray& aQualities, 
		HRESULT* pErrors);
private:
	void ResetControls(void);
	BOOL IsTextBox(DWORD dwControlID);
public:
	afx_msg void OnBtnRunClick();
	afx_msg void OnBtnWriteAOClick();
private:
	CString CheckErrors(CHRESULTArray& aErrors);
	void EnableWriteOutput(bool enable);
public:
	afx_msg void OnBtnStopClick();
	afx_msg void OnBtnWriteDOClick();
//	afx_msg void OnBnClickedCheck1();
};
