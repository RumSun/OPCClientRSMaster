// UserInterface.cpp : implementation file
//

#include "stdafx.h"
#include "OpcClientRSMaster.h"
#include "UserInterface.h"
#include "OPCDataCallback.h"
#include "ErrorLookup.h"
#include <winuser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CUserInterface dialog




CUserInterface::CUserInterface(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInterface::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserInterface::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUserInterface, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONCONNECT, &CUserInterface::OnBtnConnectClick)
	ON_BN_CLICKED(IDC_BUTTONDISCONNECT, &CUserInterface::OnBtnDisconnectClick)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTONRUN, &CUserInterface::OnBtnRunClick)
	ON_BN_CLICKED(IDC_BUTTONWRITEANALOG, &CUserInterface::OnBtnWriteAOClick)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CUserInterface::OnBtnStopClick)
	ON_BN_CLICKED(IDC_BUTTONWRITEBITS, &CUserInterface::OnBtnWriteDOClick)
//	ON_BN_CLICKED(IDC_CHECK1, &CUserInterface::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CUserInterface message handlers

BOOL CUserInterface::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUserInterface::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUserInterface::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUserInterface::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUserInterface::OnBtnConnectClick()
{	//Disable connect button
	GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(false);


	HRESULT		hr			=	S_OK;

	// Get total item count from defines in ServerDefines.h
	DWORD		dwItemCount =	/*OPCCLIENT_MASTER_SLAVE_ITEM_COUNT +*/  // Profibus DP status items
								OPCCLIENT_ANALOG_INPUT_ITEM_COUNT +  // Analog input items
								OPCCLIENT_ANALOG_OUTPUT_ITEM_COUNT + // Analog output items
								OPCCLIENT_BITINPUT_ITEM_COUNT +      // Digital input items
								OPCCLIENT_BITOUTPUT_ITEM_COUNT ;     // Digital output items

	// Create new OPCServerMgt object and connect to OPC server defined in ServerDefines.h
	m_pOPCServerMgt = new COPCServerMgt();
	hr = m_pOPCServerMgt->ConnectOPCServer((CString)OPCCLIENT_SERVER_PROGID);

	// Check if connect succeeded
	if(SUCCEEDED(hr))
	{
		// Create new OPCGroupMgt object and add OPCGroup to the OPC server
		m_pOPCGroupMgt  = new COPCGroupMgt(	OPCCLIENT_GROUP_NAME,
											OPCCLIENT_GROUP_UPDATERATE,
											FALSE);
		hr = m_pOPCServerMgt->AddGroup(m_pOPCGroupMgt);
	
		// Check if AddGroup succeeded
		if(SUCCEEDED(hr))
		{
			// Initialize the item information arrays
			CStringArray astrItem;       // Array of ItemIDs - ItemIDs defined in ServerDefines.h
			CUIntArray anRequDatatype;   // Array of requested data types - defined in ServerDefines.h
			CUIntArray anClientHandle;   // Array of client item handles - index of the item in the array
			CUIntArray anControlIDs;     // Array of User Interface Control IDs - initialized in BuildItemArrays
			BuildItemArrays(dwItemCount, &astrItem, &anClientHandle, &anRequDatatype, &anControlIDs);
			
			CUIntArray anServerHandle;   // Array of client item handles - returned from AddItems
			CHRESULTArray ahrErrors;     // Array of result codes for each item - returned from AddItems
			
			// Add items to the group
			hr = m_pOPCGroupMgt->AddItems(  dwItemCount,    // Item count
											astrItem,       // ItemIDs
											anRequDatatype,	// Requested data types  
											anClientHandle, // Client handles
											anServerHandle, // Returned server handles
											ahrErrors);     // Returnded item results

			// Initialize array of OPCItemMgt objects
			for(UINT i = 0; i < dwItemCount; i++)
			{
				COPCItemMgt item;
				// Initialize OPCItemMgt object
				item.ItemID			= astrItem[i];
				item.ServerHandle	= anServerHandle[i];
				item.Datatype		= anRequDatatype[i];
				item.ControlID		= anControlIDs[i];
				// Add OPCItemMgt object to the array
				// Index = Client Handle
				m_aOPCItemMgt.Add(item);

				// Check item errors
				if(ahrErrors[i] != S_OK)
				{
					m_strOutput += "AddItems: " + astrItem[i]+": " + CErrorLookup::GetErrorString(ahrErrors[i]) +"\r\n";
				}
			}

			if(SUCCEEDED(hr))
			{
				// Connect data callback to receive OnDataChange events from OPC server
				hr = m_pOPCGroupMgt->ConnectCallback((CDataCallback*)this);
				if(FAILED(hr))
					m_strOutput += "ConnectCallback: " + CErrorLookup::GetErrorString(hr) +"\r\n";
				else
					m_pOPCGroupMgt->SetActive(TRUE);
			}
			else
				m_strOutput += "AddItems: " + CErrorLookup::GetErrorString(hr) +"\r\n";
		}
		else
			m_strOutput += "AddGroup: " + CErrorLookup::GetErrorString(hr) +"\r\n";			
		
		// Change button states 
/*		GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(true);
		GetDlgItem(IDC_BUTTONRUN)->EnableWindow(true);
		GetDlgItem(IDC_BUTTONWRITEANALOG)->EnableWindow(true);
		GetDlgItem(IDC_BUTTONWRITEBITS)->EnableWindow(true);
*/
		EnableWriteOutput(TRUE);
	}
	else
	{
		m_strOutput += "ConnectOPCServer: " + CErrorLookup::GetErrorString(hr) +"\r\n";
		delete m_pOPCServerMgt;
		m_pOPCServerMgt = NULL;

		//Enable connect button
		GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(true);
	}

	UpdateData(false);
	((CEdit*)GetDlgItem(IDC_EDITOUTPUT))->SetSel(0,-1);
}

void CUserInterface::OnBtnDisconnectClick()
{	
	//Disable disconnect button
	GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(false);
		//Disable disconnect button
	GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(false);

	HRESULT hr = S_OK;

	if(m_pOPCGroupMgt != NULL)
	{
		//store the group-handle to remove the group from OPC server
		OPCHANDLE hGroup = m_pOPCGroupMgt->GetGroupHandle();

		//remove all items from OPC group
		int				nItemCount = (int)m_aOPCItemMgt.GetSize();
		CUIntArray		aServerHandles;	
		CHRESULTArray	aErrors;
		for( int i = 0; i < nItemCount; i++)
			aServerHandles.Add(m_aOPCItemMgt[i].ServerHandle);
		m_pOPCGroupMgt->RemoveItems(nItemCount,aServerHandles,aErrors);

		//the COPCGroupMgt-destructor will clean up any resources, which 
		//were allocated by member-functions.
		delete m_pOPCGroupMgt;
		m_pOPCGroupMgt = NULL;
		
		//remove the OPC group
		m_pOPCServerMgt->RemoveGroup(hGroup);
	}
	if(	m_pOPCServerMgt != NULL)
	{
		//the COPCServerMgt-destructor will clean up any resources, which 
		//were allocated by member-functions.
		delete m_pOPCServerMgt;
		m_pOPCServerMgt = NULL;
	}
/*	if(m_pControl != NULL)
	{
		m_pControl->StopThread();
		m_pControl->Wait(10000);
		delete m_pControl;
		m_pControl = NULL;
	}*/
	m_aOPCItemMgt.RemoveAll();
	
	//mfc-control handling
	ResetControls();
	//EnableWriteOutput(FALSE);
/*	GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(false);
	//GetDlgItem(IDC_BUTTONRUN)->EnableWindow(false);
	//GetDlgItem(IDC_BUTTONSTOP)->EnableWindow(false);
	GetDlgItem(IDC_BUTTONWRITEANALOG)->EnableWindow(false);
	GetDlgItem(IDC_BUTTONWRITEBITS)->EnableWindow(false);
*/	
	m_bControlFunctionError = FALSE;

	Invalidate();
}

void CUserInterface::OnDestroy()
{

		OnBtnDisconnectClick();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CUserInterface::BuildItemArrays(DWORD dwItemCount, CStringArray* astrItems, CUIntArray* anClientHandles, CUIntArray* anRequDatatypes, CUIntArray* anControlIDs)
{
	//Clienthandles
	for(DWORD i=0; i< dwItemCount; i++)
		anClientHandles->Add(i);


	//Master State /Slave State
	//astrItems->Add(OPCCLIENT_MASTER_STATE);
	//anControlIDs->Add(IDC_EDITMASTERSTATE);
	//anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

	//astrItems->Add(OPCCLIENT_SLAVE_TYPE);
	//anControlIDs->Add(IDC_EDITSLAVETYPE);
	//anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	//
	//astrItems->Add(OPCCLIENT_SLAVE_STATE);
	//anControlIDs->Add(IDC_EDITSLAVESTATE);
	//anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	//
	//astrItems->Add(OPCCLIENT_SLAVE_DIAGNOSIS);
	//anControlIDs->Add(IDC_EDITSLAVEDIAGNOSIS);
	//anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

	//Analog Input 
			// rooftop 2
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM1);
	anControlIDs->Add(IDC_EDITWORD0);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM2);
	anControlIDs->Add(IDC_EDITWORD1);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM3);
	anControlIDs->Add(IDC_EDITWORD2);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM4);
	anControlIDs->Add(IDC_EDITWORD3);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM5);
	anControlIDs->Add(IDC_EDITWORD4);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM6);
	anControlIDs->Add(IDC_EDITWORD5);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

			// rooftop 3
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM31);
	anControlIDs->Add(IDC_EDITWORD30);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM32);
	anControlIDs->Add(IDC_EDITWORD31);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM33);
	anControlIDs->Add(IDC_EDITWORD32);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM34);
	anControlIDs->Add(IDC_EDITWORD33);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM35);
	anControlIDs->Add(IDC_EDITWORD34);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM36);
	anControlIDs->Add(IDC_EDITWORD35);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

			// rooftop 4
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM41);
	anControlIDs->Add(IDC_EDITWORD40);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM42);
	anControlIDs->Add(IDC_EDITWORD41);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM43);
	anControlIDs->Add(IDC_EDITWORD42);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM44);
	anControlIDs->Add(IDC_EDITWORD43);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM45);
	anControlIDs->Add(IDC_EDITWORD44);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM46);
	anControlIDs->Add(IDC_EDITWORD45);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

			// rooftop 5
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM51);
	anControlIDs->Add(IDC_EDITWORD50);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM52);
	anControlIDs->Add(IDC_EDITWORD51);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM53);
	anControlIDs->Add(IDC_EDITWORD52);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM54);
	anControlIDs->Add(IDC_EDITWORD53);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM55);
	anControlIDs->Add(IDC_EDITWORD54);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM56);
	anControlIDs->Add(IDC_EDITWORD55);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

			// rooftop 6
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM61);
	anControlIDs->Add(IDC_EDITWORD60);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM62);
	anControlIDs->Add(IDC_EDITWORD61);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM63);
	anControlIDs->Add(IDC_EDITWORD62);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM64);
	anControlIDs->Add(IDC_EDITWORD63);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM65);
	anControlIDs->Add(IDC_EDITWORD64);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM66);
	anControlIDs->Add(IDC_EDITWORD65);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

			// rooftop 7
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM71);
	anControlIDs->Add(IDC_EDITWORD70);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM72);
	anControlIDs->Add(IDC_EDITWORD71);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM73);
	anControlIDs->Add(IDC_EDITWORD72);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM74);
	anControlIDs->Add(IDC_EDITWORD73);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM75);
	anControlIDs->Add(IDC_EDITWORD74);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM76);
	anControlIDs->Add(IDC_EDITWORD75);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);

			// rooftop 8
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM81);
	anControlIDs->Add(IDC_EDITWORD80);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM82);
	anControlIDs->Add(IDC_EDITWORD81);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM83);
	anControlIDs->Add(IDC_EDITWORD82);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM84);
	anControlIDs->Add(IDC_EDITWORD83);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM85);
	anControlIDs->Add(IDC_EDITWORD84);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	astrItems->Add(OPCCLIENT_ANALOG_INPUT_ITEM86);
	anControlIDs->Add(IDC_EDITWORD85);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);




	//Analog Output
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM1);
	anControlIDs->Add(IDC_EDITWORD6);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM31);
	anControlIDs->Add(IDC_EDITWORD36);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM41);
	anControlIDs->Add(IDC_EDITWORD46);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM51);
	anControlIDs->Add(IDC_EDITWORD56);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM61);
	anControlIDs->Add(IDC_EDITWORD66);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM71);
	anControlIDs->Add(IDC_EDITWORD76);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_ANALOG_OUTPUT_ITEM81);
	anControlIDs->Add(IDC_EDITWORD86);
	anRequDatatypes->Add(OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE);
	
	//8 Bit input
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM21);
	anControlIDs->Add(IDC_STATICBITINPUT21);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM31);
	anControlIDs->Add(IDC_STATICBITINPUT31);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM41);
	anControlIDs->Add(IDC_STATICBITINPUT41);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM51);
	anControlIDs->Add(IDC_STATICBITINPUT51);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM61);
	anControlIDs->Add(IDC_STATICBITINPUT61);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM71);
	anControlIDs->Add(IDC_STATICBITINPUT71);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITINPUT_ITEM81);
	anControlIDs->Add(IDC_STATICBITINPUT81);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);


	//8 Bit Output
/*	astrItems->Add(OPCCLIENT_BITOUTPUT_ITEM1);
	anControlIDs->Add(IDC_CHECK1);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITOUTPUT_ITEM31);
	anControlIDs->Add(IDC_CHECK31);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITOUTPUT_ITEM41);
	anControlIDs->Add(IDC_CHECK41);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITOUTPUT_ITEM51);
	anControlIDs->Add(IDC_CHECK51);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITOUTPUT_ITEM61);
	anControlIDs->Add(IDC_CHECK61);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITOUTPUT_ITEM71);
	anControlIDs->Add(IDC_CHECK71);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);
	astrItems->Add(OPCCLIENT_BITOUTPUT_ITEM81);
	anControlIDs->Add(IDC_CHECK81);
	anRequDatatypes->Add(OPCCLIENT_BIT_REQUESTEDDATATYPE);*/
}

STDMETHODIMP CUserInterface::DataChange(HRESULT hrMasterquality, UINT dwCount, const CUIntArray & aClientHandles, const CVARIANTArray& aValues, const CUIntArray& aQualities, HRESULT* pErrors)
{
	// Iterate all valid values and set it to the controls
	// The client handle is the array index of the OPCItemMgt array
    for(UINT i=0; i < dwCount; i++)
	{
		// Get the ControlID for the item
		// Access with Client Handle
		DWORD dwControlID = m_aOPCItemMgt[aClientHandles[i]].ControlID;
			
		if(pErrors[i] == S_OK)
		{
			// Get the requested data type
			VARTYPE vt = aValues[i].vt;
			
			if(vt == VT_BSTR)
			{
				// Data Type is string -> Control is a text box
				// Write value to the text box with the control ID from OPCItemMgt
				if(aQualities[i] != OPC_QUALITY_GOOD)
					SetDlgItemText(dwControlID,CErrorLookup::GetQuality(aQualities[i]));
				else
					SetDlgItemText(dwControlID,(CString)aValues[i].bstrVal);
			}
			else if(vt == VT_BOOL)
			{
				SetDlgItemText(dwControlID,aValues[i].boolVal?"On":"Off");
				//((CButton*)GetDlgItem(dwControlID))->SetCheck(aValues[i].boolVal);
				// Invalidate the control to get a repaint
				//GetDlgItem(dwControlID)->Invalidate();
			}
			// Store the new value in OPCItemMgt object 
			VARIANT tmp;
			VariantInit(&tmp);
			HRESULT hr = VariantCopy(&tmp,&((VARIANT)aValues[i]));
			// Clear old value
			VariantClear(&(m_aOPCItemMgt[aClientHandles[i]].Value));
			// Set new value
			m_aOPCItemMgt[aClientHandles[i]].Value = tmp;
		}
		else
		{
			if(IsTextBox(dwControlID) == TRUE)
				SetDlgItemText(dwControlID,"OPC_ERROR");

			m_strOutput += "DataChange Error: " + CErrorLookup::GetErrorString(pErrors[i]) + "\r\n";
			UpdateData(false);
			((CEdit*)GetDlgItem(IDC_EDITOUTPUT))->SetSel(0,-1);
		}
	}
	// The client must always return S_OK
	return S_OK;
}

void CUserInterface::ResetControls(void)
{
	((CEdit*)GetDlgItem(IDC_EDITWORD0))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDITWORD1))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDITWORD2))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDITWORD3))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDITWORD4))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDITWORD5))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDITWORD6))->SetWindowText("");
	for(int i=1017;i<1017+42;i++)
		((CEdit*)GetDlgItem(i))->SetWindowText("");

	((CEdit*)GetDlgItem(IDC_STATICBITINPUT21))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_STATICBITINPUT31))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_STATICBITINPUT41))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_STATICBITINPUT51))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_STATICBITINPUT61))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_STATICBITINPUT71))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_STATICBITINPUT81))->SetWindowText("");

	//((CEdit*)GetDlgItem(IDC_EDITSLAVEDIAGNOSIS))->SetWindowText("");
	//((CEdit*)GetDlgItem(IDC_EDITSLAVESTATE))->SetWindowText("");
	//((CEdit*)GetDlgItem(IDC_EDITSLAVETYPE))->SetWindowText("");
	//((CEdit*)GetDlgItem(IDC_EDITMASTERSTATE))->SetWindowText("");

/*	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
	for(int i=1065;i<1065+6;i++)
		((CButton*)GetDlgItem(i))->SetCheck(0);*/
	UpdateData(FALSE);
}

BOOL CUserInterface::IsTextBox(DWORD dwControlID)
{
	//check if the error pertains a corresponding textbox
	if(	/*dwControlID == IDC_EDITMASTERSTATE || dwControlID == IDC_EDITSLAVESTATE ||
		dwControlID == IDC_EDITSLAVETYPE || dwControlID == IDC_EDITSLAVEDIAGNOSIS ||*/
		dwControlID == IDC_EDITWORD0 || dwControlID == IDC_EDITWORD1 ||
		dwControlID == IDC_EDITWORD2 || dwControlID == IDC_EDITWORD3 ||
		dwControlID == IDC_EDITWORD4 || dwControlID == IDC_EDITWORD5||
		dwControlID == IDC_EDITWORD6 ||
		
		dwControlID == IDC_EDITWORD30 || dwControlID == IDC_EDITWORD31 ||
		dwControlID == IDC_EDITWORD32 || dwControlID == IDC_EDITWORD33 ||
		dwControlID == IDC_EDITWORD34 || dwControlID == IDC_EDITWORD35||
		dwControlID == IDC_EDITWORD36 ||
		
		dwControlID == IDC_EDITWORD40 || dwControlID == IDC_EDITWORD41 ||
		dwControlID == IDC_EDITWORD42 || dwControlID == IDC_EDITWORD43 ||
		dwControlID == IDC_EDITWORD44 || dwControlID == IDC_EDITWORD45||
		dwControlID == IDC_EDITWORD46 ||
		
		dwControlID == IDC_EDITWORD50 || dwControlID == IDC_EDITWORD51 ||
		dwControlID == IDC_EDITWORD52 || dwControlID == IDC_EDITWORD53 ||
		dwControlID == IDC_EDITWORD54 || dwControlID == IDC_EDITWORD55||
		dwControlID == IDC_EDITWORD56 ||
		
		dwControlID == IDC_EDITWORD60 || dwControlID == IDC_EDITWORD61 ||
		dwControlID == IDC_EDITWORD62 || dwControlID == IDC_EDITWORD63 ||
		dwControlID == IDC_EDITWORD64 || dwControlID == IDC_EDITWORD65||
		dwControlID == IDC_EDITWORD66 ||
		
		dwControlID == IDC_EDITWORD70 || dwControlID == IDC_EDITWORD71 ||
		dwControlID == IDC_EDITWORD72 || dwControlID == IDC_EDITWORD73 ||
		dwControlID == IDC_EDITWORD74 || dwControlID == IDC_EDITWORD75||
		dwControlID == IDC_EDITWORD76 ||
		
		dwControlID == IDC_EDITWORD80 || dwControlID == IDC_EDITWORD81 ||
		dwControlID == IDC_EDITWORD82 || dwControlID == IDC_EDITWORD83 ||
		dwControlID == IDC_EDITWORD84 || dwControlID == IDC_EDITWORD85||
		dwControlID == IDC_EDITWORD86
/*
		dwControlID == IDC_STATICBITINPUT21 || dwControlID == IDC_STATICBITINPUT31 || 
		dwControlID == IDC_STATICBITINPUT41 || dwControlID == IDC_STATICBITINPUT51 || 
		dwControlID == IDC_STATICBITINPUT61 || dwControlID == IDC_STATICBITINPUT71 || 
		dwControlID == IDC_STATICBITINPUT81*/
		)
		return TRUE;
	else
		return FALSE;
}

void CUserInterface::OnBtnRunClick()
{
/*	GetDlgItem(IDC_BUTTONRUN)->EnableWindow(false);

	if(m_pControl == NULL)
	{
		m_pControl = new CControl();
		m_pControl->StartThread();
	}*/
	//EnableWriteOutput(FALSE);
}

void CUserInterface::OnBtnWriteAOClick()
{
	HRESULT			hr				=	S_OK;
	UINT			nOffset			=	/*OPCCLIENT_MASTER_SLAVE_ITEM_COUNT +*/
										OPCCLIENT_ANALOG_INPUT_ITEM_COUNT;
	UINT			nItemCount		=	OPCCLIENT_ANALOG_OUTPUT_ITEM_COUNT;
	CVARIANTArray	aItemValues;
	CUIntArray		anServerHandles;
	CHRESULTArray	aErrors;
	
	//get the values to write as CVARIANTArray
	for(UINT i = nOffset; i < nItemCount + nOffset; i++)
	{
		//get item from item-array
		COPCItemMgt item = m_aOPCItemMgt[i];
		CString	strtmp;
		//get text from CEdit-control
		GetDlgItemText(item.ControlID,strtmp);
		VARIANT vartmp;
		VariantInit(&vartmp);
		vartmp.vt = item.Datatype;
		vartmp.bstrVal = strtmp.AllocSysString();
		//fill value-array
		aItemValues.Add(vartmp);
		//copy the serverhandles of the items to write
		anServerHandles.Add(item.ServerHandle);
	}

	//write the new values to OPC server
	hr = m_pOPCGroupMgt->Write(nItemCount, anServerHandles, aItemValues, aErrors);
	
	//free allocated memory
	for(UINT i = 0; i < nItemCount; i++)
		VariantClear(&aItemValues[i]);

	if(hr != S_OK)
	{
		m_strOutput = "Write: " + CErrorLookup::GetErrorString(hr) +"\r\n";
		if(SUCCEEDED(hr))
			m_strOutput += CheckErrors(aErrors);
	}
	else
	{
		m_strOutput = "Items successfully written.";
	}
	
	UpdateData(false);
}

CString CUserInterface::CheckErrors(CHRESULTArray& aErrors)
{
	// Check for errors
	CString retval = "";
	if(aErrors.GetSize() > 0)
	{
		for(int i=0; i < aErrors.GetSize(); i++)
		{
			if(aErrors[i] != S_OK)
			{
				CString tmp;
				tmp.Format("Writing item %d: ",i+1);
				retval += tmp + CErrorLookup::GetErrorString(aErrors[i]) +"\r\n";
			}
		}
	}
	return retval;
}

void CUserInterface::EnableWriteOutput(bool enable)
{

	((CEdit*)GetDlgItem(IDC_EDITWORD6))->SetReadOnly(!enable);
	((CEdit*)GetDlgItem(IDC_EDITWORD36))->SetReadOnly(!enable);
	((CEdit*)GetDlgItem(IDC_EDITWORD46))->SetReadOnly(!enable);
	((CEdit*)GetDlgItem(IDC_EDITWORD56))->SetReadOnly(!enable);
	((CEdit*)GetDlgItem(IDC_EDITWORD66))->SetReadOnly(!enable);
	((CEdit*)GetDlgItem(IDC_EDITWORD76))->SetReadOnly(!enable);
	((CEdit*)GetDlgItem(IDC_EDITWORD86))->SetReadOnly(!enable);
/*
	GetDlgItem(IDC_CHECK1)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK31)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK41)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK51)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK61)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK71)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK81)->EnableWindow(enable);
*/
	GetDlgItem(IDC_BUTTONSTOP)->EnableWindow(!enable);
	GetDlgItem(IDC_BUTTONRUN)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTONWRITEANALOG)->EnableWindow(enable);
//	GetDlgItem(IDC_BUTTONWRITEBITS)->EnableWindow(enable);

	//GetDlgItem(IDC_STATICMASTERQUALITY)->Invalidate();
}

void CUserInterface::OnBtnStopClick()
{
	GetDlgItem(IDC_BUTTONSTOP)->EnableWindow(false);

/*	if(m_pControl != NULL)
	{
		m_pControl->StopThread();
		m_pControl->Wait(INFINITE);
		delete m_pControl;
		m_pControl = NULL;
	}*/
	EnableWriteOutput(TRUE);
}

void CUserInterface::OnBtnWriteDOClick()
{
	HRESULT			hr				=	S_OK;
	UINT			nOffset			=	/*OPCCLIENT_MASTER_SLAVE_ITEM_COUNT +*/
										OPCCLIENT_ANALOG_INPUT_ITEM_COUNT +
										OPCCLIENT_ANALOG_OUTPUT_ITEM_COUNT +
										OPCCLIENT_BITINPUT_ITEM_COUNT;
	UINT			nItemCount		=	OPCCLIENT_BITOUTPUT_ITEM_COUNT;
	CVARIANTArray	aItemValues;
	CUIntArray		anServerHandles;
	CHRESULTArray	aErrors;
	
	//get the values to write as CVARIANTArray
	for(UINT i = nOffset; i < nItemCount + nOffset; i++)
	{
		//get item from item-array
		COPCItemMgt item = m_aOPCItemMgt[i];

		//get state from CButton
		BOOL value = (((CButton*)GetDlgItem(item.ControlID))->GetState() & 0x03) * (-1);
		VARIANT vartmp;
		VariantInit(&vartmp);
		vartmp.vt = item.Datatype;
		vartmp.boolVal = value;
	
		//fill value-array
		aItemValues.Add(vartmp);
		//copy the serverhandles of the items to write
		anServerHandles.Add(item.ServerHandle);
	}

	//write the new values to OPC server
	hr = m_pOPCGroupMgt->Write(nItemCount, anServerHandles, aItemValues, aErrors);
	
	//free allocated memory
	for(UINT i = 0; i < nItemCount; i++)
		VariantClear(&aItemValues[i]);

	if(hr != S_OK)
	{
		m_strOutput = "Write: " + CErrorLookup::GetErrorString(hr) +"\r\n";
		if(SUCCEEDED(hr))
			m_strOutput += CheckErrors(aErrors);
	}
	else
		m_strOutput = "Items successfully written.";
	
	UpdateData(false);
}


