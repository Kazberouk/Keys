 // SSPro60Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SSPro60.h"
#include "SSPro60Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _EXP_ORDER_OR_ "OR"
#define _EXP_ORDER_THEN_ "THEN"
#define _KEY_VERSION_NUMBER_ 10

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
RB_WORD write_pass = 0x1234;
RB_WORD overwrite_pass1 = 0x1234;
RB_WORD overwrite_pass2 = 0x1234;
char p1[] = {50,44,46,62,3,25,5,9,4};
char p2[] = {50,44,46,62,3,25,5,9,4,27};
char p3[] = {50,44,46,62,3,25,5,9,4,28};
CString status(_T("Please, enter password"));
char chars[] = "&qwertyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM";
bool PassswordEntered = FALSE;

bool IsAccess(BOOL b) { return (b != 0);}

bool cmpext(CString &str, char p[], int size){
	if(str.GetLength() != size)
		return false;
	for(int i = 0; i<size;i++)
		if(str[i] != chars[p[i]])
			return false;
	return true;
}

class CAboutDlg : public CDialog {
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_WritePass;
	CString	m_OverWritePass1;
	CString	m_OverWritePass2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
	//{{AFX_DATA_INIT(CAboutDlg)
	m_WritePass = _T("");
	m_OverWritePass1 = _T("");
	m_OverWritePass2 = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_WRITE_PAS, m_WritePass);
	DDV_MaxChars(pDX, m_WritePass, 12);
	DDX_Text(pDX, IDC_OVERWRITE_PAS1, m_OverWritePass1);
	DDV_MaxChars(pDX, m_OverWritePass1, 12);
	DDX_Text(pDX, IDC_OVERWRITE_PAS2, m_OverWritePass2);
	DDV_MaxChars(pDX, m_OverWritePass2, 12);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	return TRUE;
}

#define USE_PASSWORD

void CAboutDlg::OnOK() {
	CDialog::OnOK();

#ifdef USE_PASSWORD
	if(cmpext(m_WritePass, p1, sizeof(p1)) && cmpext(m_OverWritePass1, p2, sizeof(p2))&& cmpext(m_OverWritePass2, p3, sizeof(p3))) {
#endif
		write_pass = 0xBD8F;
		overwrite_pass1 = 0xE31C;
		overwrite_pass2 = 0x1F3D;
		status = _T("OK");
		PassswordEntered = TRUE;

#ifdef USE_PASSWORD
	}
	else
		status = _T("Wrong password");
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CSSPro60Dlg dialog
/////////////////////////////////////////////////////////////////////////////
CSSPro60Dlg::CSSPro60Dlg(CWnd* pParent /*=NULL*/) : CDialog(CSSPro60Dlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CSSPro60Dlg)
	m_KeyNumber = 0;
	m_KeyName = _T("Not Initialized");
	COleDateTime odtDateTime = COleDateTime::GetCurrentTime();
	odtDateTime.m_dt -= 365.;
	m_ExpDate	= odtDateTime;
	m_RunsCount	= 10;
	m_LicDate	= odtDateTime;
	m_vBurnDate	= odtDateTime;
	m_UseCurrentDate = FALSE;
	m_vVersionNumber = _KEY_VERSION_NUMBER_;
	m_UseCurrentVersion = FALSE;

	m_AllFPM			= FALSE;
	m_AllFIP			= FALSE;

	m_FPM				= FALSE;
	m_FRACPRO		= FALSE;
	m_STIMPRO		= FALSE;
	m_CEMENTPRO		= FALSE;
	m_RESPRO			= FALSE;
	m_REALPRO		= FALSE;
	m_TOOLPRO		= FALSE;
	m_MAXPRO			= FALSE;
	m_WTPA			= FALSE;
	m_FIELDPRO		= FALSE;
	m_FLOPRO			= FALSE;
	m_ECOPRO			= FALSE;
	m_GRADIENT		= FALSE;
	m_GEOPRO			= FALSE;
	m_LOGPRO			= FALSE;
	m_TRANSPRO		= FALSE;
	m_EFIELDPRO		= FALSE;
	m_STIM_RAW_DATA= FALSE;
	m_COST			= FALSE;
	m_FIELDPRO_VM	= FALSE;

	m_iComboKeyList = 0;
	m_strKeyList = "";

	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSPro60Dlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSPro60Dlg)
	DDX_Control(pDX, IDC_VERSION_NUMBER, m_cVersionNumber);
	DDX_Text(pDX, IDC_KEY_NUMBER, m_KeyNumber);
	DDV_MinMaxInt(pDX, m_KeyNumber, 0, 65535);
	DDX_Text(pDX, IDC_KEY_NAME, m_KeyName);
	DDV_MaxChars(pDX, m_KeyName, 32);
	DDX_DateTimeCtrl(pDX, IDC_EXP_DATE, m_ExpDate);
	DDX_Control(pDX, IDC_EXP_COMBO, m_ExpOrderCombo);
	DDX_Text(pDX, IDC_RUNS_COUNT, m_RunsCount);
	DDV_MinMaxInt(pDX, m_RunsCount, -1, 32767);
	DDX_DateTimeCtrl(pDX, IDC_PAID_DATE, m_LicDate);
	DDX_DateTimeCtrl(pDX, IDC_BURN_DATE, m_vBurnDate);
	DDX_Control(pDX, IDC_BURN_DATE, m_cBurnDate);
	DDX_Check(pDX, IDC_SET_CURRENT_DATE, m_UseCurrentDate);
	DDX_Text(pDX, IDC_VERSION_NUMBER, m_vVersionNumber);
	DDX_Text(pDX, IDC_STATUS, status);
	DDX_Check(pDX, IDC_SET_CURRENT_VERSION, m_UseCurrentVersion);
	DDX_Control(pDX, IDC_MAKE_KEY, m_cMakeKey);
	DDX_Control(pDX, IDC_MAKE_KEY_FILE, m_cMakeKeyFile);
	DDX_Check(pDX, IDC_ALL_FPM, m_AllFPM);
	DDX_Check(pDX, IDC_ALL_FIP, m_AllFIP);
	DDX_Check(pDX, IDC_FPM, m_FPM);
	DDX_Check(pDX, IDC_FRACPRO, m_FRACPRO);
	DDX_Check(pDX, IDC_STIMPRO, m_STIMPRO);
	DDX_Check(pDX, IDC_CEMENTPRO, m_CEMENTPRO);
	DDX_Check(pDX, IDC_RESPRO, m_RESPRO);
	DDX_Check(pDX, IDC_REALPRO, m_REALPRO);
	DDX_Check(pDX, IDC_TOOLPRO, m_TOOLPRO);
	DDX_Check(pDX, IDC_MAXPRO, m_MAXPRO);
	DDX_Check(pDX, IDC_WTPA, m_WTPA);
	DDX_Check(pDX, IDC_FIELDPRO, m_FIELDPRO);
	DDX_Check(pDX, IDC_PORPRO, m_FLOPRO);
	DDX_Check(pDX, IDC_ECOPRO, m_ECOPRO);
	DDX_Check(pDX, IDC_GRADIENT, m_GRADIENT);
	DDX_Check(pDX, IDC_COST, m_COST);
	DDX_Check(pDX, IDC_STIM_RAW_DATA, m_STIM_RAW_DATA);
	DDX_Check(pDX, IDC_GEO_OP, m_GEOPRO);
	DDX_Check(pDX, IDC_REGISTERED, m_LOGPRO);
	DDX_Check(pDX, IDC_MIGRATION, m_TRANSPRO);
	DDX_Check(pDX, IDC_4PHI, m_EFIELDPRO);
	DDX_Check(pDX, IDC_FIELDPRO_VM, m_FIELDPRO_VM);
	
	DDX_Control(pDX, IDC_COMBO_LOADED_KEY, m_cComboKeyList);
	DDX_CBIndex(pDX, IDC_COMBO_LOADED_KEY, m_iComboKeyList);
	DDX_Text(pDX, IDC_KEY_LIST, m_strKeyList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSSPro60Dlg, CDialog)
	//{{AFX_MSG_MAP(CSSPro60Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MAKE_KEY					, OnMakeKey				)
	ON_BN_CLICKED(IDC_INCREASE_KEY_NUMBER	, OnIncreaseKeyNumber)
	ON_BN_CLICKED(IDC_GETINFO					, OnGetinfo				)
	ON_BN_CLICKED(IDC_LOAD_LOG					, OnLoadLog				)
	ON_BN_CLICKED(IDC_LOAD_KEY_FILE			, OnLoadKeyFile		)
	ON_BN_CLICKED(IDC_MAKE_KEY_FILE			, OnMakeKeyFile		)
	ON_BN_CLICKED(IDC_SET_CURRENT_DATE		, OnSetCurrentDate	)
	ON_BN_CLICKED(IDC_ALL_FPM					, OnAllFPM				)
	ON_BN_CLICKED(IDC_ALL_FIP					, OnAllFIP				)
	ON_BN_CLICKED(IDC_SET_CURRENT_VERSION	, OnSetCurrentVersion)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_LOADED_KEY, &CSSPro60Dlg::OnSelchangeComboLoadedKey)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSPro60Dlg message handlers

BOOL CSSPro60Dlg::OnInitDialog() {
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL){
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()){
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_ExpOrderCombo.ResetContent();
	m_ExpOrderCombo.AddString(_EXP_ORDER_OR_);
	m_ExpOrderCombo.AddString(_EXP_ORDER_THEN_);
	m_ExpOrderCombo.SetCurSel(1);

	m_cBurnDate		.EnableWindow(!m_UseCurrentDate);
	m_cVersionNumber.EnableWindow(!m_UseCurrentVersion);
	m_cMakeKey		.EnableWindow(PassswordEntered);
	m_cMakeKeyFile	.EnableWindow(PassswordEntered);

	m_ExpDate.SetDate(COleDateTime::GetCurrentTime().GetYear()-1, 3, 31);
	m_LicDate.SetDate(COleDateTime::GetCurrentTime().GetYear()-1, 3, 31);

	ResetKeyList();

	return TRUE;
}

void CSSPro60Dlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		UpdateData();
		CAboutDlg dlgAbout;
		if(dlgAbout.DoModal() == IDOK) {
			m_cMakeKey		.EnableWindow(PassswordEntered);
			m_cMakeKeyFile	.EnableWindow(PassswordEntered);
			UpdateData(FALSE);
		}
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSSPro60Dlg::OnPaint() {
	if (IsIconic())	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags the minimized window.
HCURSOR CSSPro60Dlg::OnQueryDragIcon() {
	return (HCURSOR) m_hIcon;
}

#define _SET_DATA_(mask1)	WORD(	IsAccess(m_##mask1##)*_SSP60_##mask1##_MASK_)

void CSSPro60Dlg::SetKeyDataFromDialog(CSSPro60Data &KeyData) {
	UpdateData(TRUE);

	KeyData.ClearAll();
	KeyData.m_KeyNumber = (WORD)m_KeyNumber;

	KeyData.m_ExpDate.m_Day = 0;
	KeyData.m_ExpDate.m_Month = 0;
	KeyData.m_ExpDate.m_Year = 0;
	if(m_ExpDate.GetStatus() == COleDateTime::valid) {
		KeyData.m_ExpDate.m_Day = m_ExpDate.GetDay();
		KeyData.m_ExpDate.m_Month = m_ExpDate.GetMonth();
		KeyData.m_ExpDate.m_Year = div(m_ExpDate.GetYear(),100).rem;
	}

	KeyData.m_LicDate.m_Day = 0;
	KeyData.m_LicDate.m_Month = 0;
	KeyData.m_LicDate.m_Year = 0;
	if(m_LicDate.GetStatus() == COleDateTime::valid) {
		KeyData.m_LicDate.m_Day = m_LicDate.GetDay();
		KeyData.m_LicDate.m_Month = m_LicDate.GetMonth();
		KeyData.m_LicDate.m_Year = div(m_LicDate.GetYear(),100).rem;
	}

	if (m_UseCurrentDate) {
		KeyData.m_BurnDate.m_Day = COleDateTime::GetCurrentTime().GetDay();
		KeyData.m_BurnDate.m_Month = COleDateTime::GetCurrentTime().GetMonth();
		KeyData.m_BurnDate.m_Year = div(COleDateTime::GetCurrentTime().GetYear(),100).rem;
		m_vBurnDate	= COleDateTime::GetCurrentTime();
	}
	else {
		KeyData.m_BurnDate.m_Day = m_vBurnDate.GetDay();
		KeyData.m_BurnDate.m_Month = m_vBurnDate.GetMonth();
		KeyData.m_BurnDate.m_Year = div(m_vBurnDate.GetYear(),100).rem;
	}
	
	if (m_UseCurrentVersion) {
		KeyData.m_Updater_Version = _KEY_VERSION_NUMBER_;
		m_vVersionNumber = _KEY_VERSION_NUMBER_;
	}
	
	if (m_RunsCount != -1)
		KeyData.m_ExpCounter = (WORD) (_SSP60_COUNTER_FLAG_ | m_RunsCount);

	CString str;
	m_ExpOrderCombo.GetLBText(m_ExpOrderCombo.GetCurSel(), str);
	if (str == _EXP_ORDER_THEN_)
		KeyData.m_ExpUpdate = _SSP60_EXPIRE_ORDER_FLAG_;

	KeyData.m_Fieldpro					= WORD(_SET_DATA_(FIELDPRO	));				
	KeyData.m_FPM							= WORD(_SET_DATA_(FPM		));
	KeyData.m_Fracpro						= WORD(_SET_DATA_(FRACPRO	));
	KeyData.m_RawData_Stimpro			= WORD(_SET_DATA_(STIM_RAW_DATA)+ _SET_DATA_(STIMPRO	));
	KeyData.m_Respro_Cementpro			= WORD(_SET_DATA_(RESPRO	)	+ _SET_DATA_(CEMENTPRO	));
	KeyData.m_MaxproVM					= WORD(_SET_DATA_(MAXPRO	)	+ _SET_DATA_(FIELDPRO_VM));
	KeyData.m_PorPro_EcoPro_Cost		= WORD(_SET_DATA_(FLOPRO	)	+ _SET_DATA_(ECOPRO		)	+ _SET_DATA_(COST		));
	KeyData.m_WTPA_Gradient_Toolpro	= WORD(_SET_DATA_(WTPA		)	+ _SET_DATA_(TOOLPRO		)	+ _SET_DATA_(GRADIENT	));
	KeyData.m_TransPro					= WORD(_SET_DATA_(TRANSPRO	));
	KeyData.m_eFieldpro					= WORD(_SET_DATA_(EFIELDPRO	));
	KeyData.m_Realpro						= WORD(_SET_DATA_(REALPRO	));
	KeyData.m_GeoPro_LogPro				= WORD(_SET_DATA_(GEOPRO	)	+ _SET_DATA_(LOGPRO	));
//	KeyData.m_Word22						= WORD(_SET_DATA_(FORECAST	));

	strncpy_s(KeyData.m_KeyName, (LPCTSTR)m_KeyName, sizeof(KeyData.m_KeyName));
	KeyData.m_Updater_Version = WORD(m_vVersionNumber);
}

void CSSPro60Dlg::OnMakeKey() {
	status = _T("");

	CWaitCursor wc;
	CSSPro60Sec Key;
	
	SaveKeyStatus();

	CFile file;
	CString file_name;
	file_name.Format("%d-Write.log", m_KeyNumber);
	if (!file.Open(file_name, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary)) {
		MessageBox("Can't create log file");
		return;
	}

	if(FAILED(Key.Init())) {
		MessageBox("Error");
		return;
	}

	if(FAILED(Key.OpenKey())) {
		MessageBox("Key not found");
		return;
	}

	SetKeyDataFromDialog(Key.m_KeyData);

	file.Write("\nSSPro60\n", 9);

	if(FAILED(Key.BurnKey(write_pass, overwrite_pass1, overwrite_pass2))) {
		MessageBox("ACCESS DENIED");
		status = _T("!!! ERROR !!!");
	}
	else {
		SEC_KEY_BUFFER buff;
		Key.m_KeyData.Serialize(true, buff);
		file.Write(buff, sizeof(buff));
		status = _T("Key generated successfully");
		MessageBeep(0);
	}
	file.Close();

	ResetKeyList();
}

void CSSPro60Dlg::OnIncreaseKeyNumber() {
	if (!UpdateData())
		return;
	m_KeyNumber++;
	status = _T("");
	UpdateData(FALSE);
}

#define _GET_DATA_(var, mask) 	\
	m_##mask## = !!(KeyData.var & _SSP60_##mask##_MASK_);

void CSSPro60Dlg::LoadDialogFromKeyData(CSSPro60Data &KeyData) {
	m_KeyNumber = KeyData.m_KeyNumber;

	if((KeyData.m_ExpDate.m_Day == 0) && (KeyData.m_ExpDate.m_Month == 0))
		m_ExpDate.SetStatus(COleDateTime::null);
	else
		m_ExpDate.SetDate(KeyData.m_ExpDate.m_Year+2000, 
								KeyData.m_ExpDate.m_Month,
								KeyData.m_ExpDate.m_Day);

	if((KeyData.m_LicDate.m_Day == 0) && (KeyData.m_LicDate.m_Month == 0))
		m_LicDate.SetStatus(COleDateTime::null);
	else
		m_LicDate.SetDate(KeyData.m_LicDate.m_Year+2000, 
								KeyData.m_LicDate.m_Month,
								KeyData.m_LicDate.m_Day);

	if((KeyData.m_BurnDate.m_Day == 0) && (KeyData.m_BurnDate.m_Month == 0))
		m_vBurnDate.SetStatus(COleDateTime::null);
	else
		m_vBurnDate.SetDate(KeyData.m_BurnDate.m_Year+2000, 
								KeyData.m_BurnDate.m_Month,
								KeyData.m_BurnDate.m_Day);

	if (KeyData.m_ExpCounter & _SSP60_COUNTER_FLAG_)
		m_RunsCount = KeyData.m_ExpCounter & ~_SSP60_COUNTER_FLAG_;
	else
		m_RunsCount = -1;

	if (KeyData.m_ExpUpdate & _SSP60_EXPIRE_ORDER_FLAG_)
		m_ExpOrderCombo.SelectString(-1, _EXP_ORDER_THEN_);
	else
		m_ExpOrderCombo.SelectString(-1, _EXP_ORDER_OR_);

	_GET_DATA_(m_FPM							, FPM				);
	_GET_DATA_(m_Fracpro						, FRACPRO		);
	_GET_DATA_(m_RawData_Stimpro			, STIMPRO		);
	_GET_DATA_(m_RawData_Stimpro			, STIM_RAW_DATA);
	_GET_DATA_(m_Respro_Cementpro			, RESPRO			);
	_GET_DATA_(m_Respro_Cementpro			, CEMENTPRO		);
	_GET_DATA_(m_Realpro						, REALPRO		);
	_GET_DATA_(m_WTPA_Gradient_Toolpro	, TOOLPRO		);
	_GET_DATA_(m_WTPA_Gradient_Toolpro	, GRADIENT		);
	_GET_DATA_(m_WTPA_Gradient_Toolpro	, WTPA			);
	_GET_DATA_(m_MaxproVM					, MAXPRO			);
	_GET_DATA_(m_MaxproVM					, FIELDPRO_VM	);
	_GET_DATA_(m_Fieldpro					, FIELDPRO		);
	_GET_DATA_(m_PorPro_EcoPro_Cost		, FLOPRO			);
	_GET_DATA_(m_PorPro_EcoPro_Cost		, ECOPRO			);
	_GET_DATA_(m_PorPro_EcoPro_Cost		, COST			);
	_GET_DATA_(m_GeoPro_LogPro				, GEOPRO			);
	_GET_DATA_(m_GeoPro_LogPro				, LOGPRO			);
	_GET_DATA_(m_TransPro					, TRANSPRO		);
	_GET_DATA_(m_eFieldpro					, EFIELDPRO		);

	char name[sizeof(KeyData.m_KeyName)+1];
	memset(name, 0, sizeof(name));
	memcpy(name, KeyData.m_KeyName, sizeof(KeyData.m_KeyName));
	m_KeyName = name;
	m_vVersionNumber = KeyData.m_Updater_Version;

	if (KeyData.m_ExpUpdate & _SSP60_EXPIRE_FLAG_)
		status = "Key expired!";
}

void CSSPro60Dlg::OnGetinfo() {
	CWaitCursor wc;
	CSSPro60Sec Key;

	if(FAILED(Key.Init())) {
		MessageBox("Error");
		return;
	}

	if(FAILED(Key.OpenKey())) {
		MessageBox("Key not found");
		return;
	}
	m_UseCurrentDate = FALSE;
	m_UseCurrentVersion = FALSE;

	LoadDialogFromKeyData(Key.m_KeyData);
	m_cBurnDate.EnableWindow(!m_UseCurrentDate);
	m_cVersionNumber.EnableWindow(!m_UseCurrentVersion);

	ResetKeyList();
}

void CSSPro60Dlg::SaveKeyStatus() {
	CWaitCursor wc;
	CSSPro60Sec Key;

	Key.m_KeyData.ClearAll();
	if(FAILED(Key.Init())) {
		MessageBox("Error");
		return;
	}

	if(FAILED(Key.OpenKey())) {
		return;
	}

//	ReadKeyInfo(Key);

	if (Key.m_KeyData.m_KeyNumber<=0)
		return;
	CFile file;
	CString file_name;
	file_name.Format("%d-Read.log", Key.m_KeyData.m_KeyNumber);
	if (!file.Open(file_name, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary)) {
		MessageBox("Can't create Input log file");
		return;
	}
	file.Write("\nSSPro60\n", 9);
	SEC_KEY_BUFFER buff;
	Key.m_KeyData.Serialize(true, buff);
	file.Write(buff, sizeof(buff));
	file.Close();
}

static char BASED_CODE szFilterLog[] = "Log Files (*.LOG)|*.LOG|All Files (*.*)|*.*||";

void CSSPro60Dlg::OnLoadLog() {
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY,szFilterLog);
	if (dlg.DoModal() != IDOK)
		return;

	CString file_name = dlg.GetPathName();
	CString sig_buff;
	SEC_KEY_BUFFER buff;
	CFile log_file;
	if (!log_file.Open(file_name, CFile::modeRead|CFile::typeBinary)) {
		MessageBox("Can't open log file");
		return;
	}

	int count = log_file.Read(sig_buff.GetBuffer(9), 9);
	sig_buff.ReleaseBuffer();
	if (count != 9 && sig_buff != "\nSSPro60\n") {
		MessageBox("Wrong file format!");
		log_file.Close();
		return;
	}
	count = log_file.Read(buff, sizeof(buff));
	log_file.Close();
	if (count != sizeof(buff) ) {
		MessageBox("Wrong file format!");
		return;
	}
	CSSPro60Sec Key;
	Key.m_KeyData.Serialize(false, buff);
	LoadDialogFromKeyData(Key.m_KeyData);

	ResetKeyList();
}

static char BASED_CODE szFilterKey[] = "Key Files (*.KEY)|*.KEY|All Files (*.*)|*.*||";

void CSSPro60Dlg::OnLoadKeyFile() {
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY,szFilterKey);
	if (dlg.DoModal() != IDOK)
		return;

	m_strKeysFile = dlg.GetPathName();
	CSSPro60Sec KeyToLoad;
	if (!bLoadKeyLicenseInfoStarSec(&KeyToLoad, -1, m_strKeysFile)) {
		AfxMessageBox("License information for current security key was not found.", MB_OK );
		return;
	}
	LoadDialogFromKeyData(KeyToLoad.m_KeyData);

	CArray<int, int> arrKeys;
	int  nKeysNumber = nGetKeysListStarSec(m_strKeysFile, arrKeys);

	if (nKeysNumber <= 0) {
		ResetKeyList();
		return;
	}

	m_cComboKeyList.ResetContent();
	m_strKeyList.Empty();
	CString strKeyTextLine;
	int iIndex;
	for (int i=0; i<nKeysNumber; i++) {
		strKeyTextLine.Format("%4d", arrKeys[i]);
		m_strKeyList += strKeyTextLine;
		if (i%12 == 11)
			m_strKeyList += "\r\n";
		else if (i < nKeysNumber-1)
			m_strKeyList += ", ";
		iIndex = m_cComboKeyList.AddString(strKeyTextLine.GetBuffer(0));
		m_cComboKeyList.SetItemData(iIndex, arrKeys[i]);
	}
	m_iComboKeyList = 0;
	if (nKeysNumber > 1) {
		m_cComboKeyList.EnableWindow(TRUE);
	}
	else if (nKeysNumber == 1)
		m_cComboKeyList.EnableWindow(FALSE);
		
	UpdateData(FALSE);
}

void CSSPro60Dlg::ResetKeyList() {
	m_cComboKeyList.EnableWindow(FALSE);
	m_cComboKeyList.ResetContent();
	m_strKeyList.Empty();
	m_cComboKeyList.ResetContent();
	m_strKeyList.Empty();
	m_iComboKeyList = 0;
	m_strKeysFile = "";
	UpdateData(FALSE);
}

void CSSPro60Dlg::OnMakeKeyFile() {
	CWaitCursor wc;

	CSSPro60Sec KeyToStore;
	SetKeyDataFromDialog(KeyToStore.m_KeyData);

	CFileDialog dlg(FALSE, ".KEY", NULL, OFN_HIDEREADONLY, szFilterKey);
	if (dlg.DoModal() != IDOK)
		return;
	CString file_name = dlg.GetPathName();

	if (!bStoreKeyLicenseInfoStarSec(&KeyToStore, KeyToStore.m_KeyData.m_KeyNumber, file_name, FALSE)) {
		AfxMessageBox("Key File was not stored.", MB_OK );
		status = _T("!!! ERROR !!!");
	}
	else {
		status = _T("");
	}
	UpdateData(FALSE);
}

void CSSPro60Dlg::OnSelchangeComboLoadedKey() {
	UpdateData(TRUE);

	int iKeyNumber = m_cComboKeyList.GetItemData(m_iComboKeyList);

	CSSPro60Sec KeyToLoad;
	if (!bLoadKeyLicenseInfoStarSec(&KeyToLoad, iKeyNumber, m_strKeysFile)) {
		AfxMessageBox("License information for current security key was not loaded.", MB_OK );
		ResetKeyList();
		return;
	}
	status = _T("");
	LoadDialogFromKeyData(KeyToLoad.m_KeyData);
	UpdateData(FALSE);
}

void CSSPro60Dlg::OnSetCurrentDate() {
	UpdateData();
	m_cBurnDate.EnableWindow(!m_UseCurrentDate);
}
void CSSPro60Dlg::OnSetCurrentVersion() {
	UpdateData();
	m_cVersionNumber.EnableWindow(!m_UseCurrentVersion);
}

void CSSPro60Dlg::OnAllFPM() {
	UpdateData();
	m_FPM			= 
	m_FRACPRO	= 
	m_STIMPRO	= 
	m_CEMENTPRO	= 
	m_RESPRO		= 
	m_REALPRO	= 
	m_TOOLPRO	= 
	m_MAXPRO		= 
	m_WTPA		= 
	m_STIM_RAW_DATA = m_AllFPM;
	UpdateData(FALSE);
}

void CSSPro60Dlg::OnAllFIP() {
	UpdateData();
	m_FIELDPRO	= 
	m_FLOPRO		= 
	m_ECOPRO		= 
	m_GRADIENT	= 
	m_GEOPRO		= 
	m_LOGPRO		= 
	m_TRANSPRO	= 
	m_COST		=
	m_EFIELDPRO	= m_AllFIP;
	UpdateData(FALSE);
}

