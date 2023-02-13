// RESkeyUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RESkeyUpdate.h"
#include "RESkeyUpdateDlg.h"
#include "../Utils.32/StarlingPro/SSPro60Sec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void GetVersionInfoProgramTime(SYSTEMTIME &version_time)
{
//	version_time.wYear = 2004;
//	version_time.wMonth = 6;
//	version_time.wDay = 23;
	DWORD   dwVerInfoSize;		
	LPSTR   lpVersion;		
	DWORD   dwVerHnd=0;			
	UINT    uVersionLen;
	WORD    wRootLen;
	BOOL    bRetCode;
	char    szFullPath[256];
	char    szGetName[256];
	CString ProgramLine = "";
	CString Version, FileDesc;

	GetModuleFileName(AfxGetApp()->m_hInstance, szFullPath, sizeof(szFullPath));

	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	if (dwVerInfoSize)
	{
		LPSTR   lpstrVffInfo;
		HANDLE  hMem;
		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpstrVffInfo  = (LPSTR) GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpstrVffInfo);

		lstrcpy(szGetName, "\\StringFileInfo\\040904b0\\");
		wRootLen = (WORD)lstrlen(szGetName); // Save this position

		szGetName[wRootLen] = (char)0;
		lstrcat (szGetName, "FileDescription");
		uVersionLen   = 0;
		lpVersion     = NULL;
		bRetCode      = VerQueryValue((LPVOID)lpstrVffInfo,
				(LPSTR)szGetName,	(LPVOID*)&lpVersion,	(UINT *)&uVersionLen);

		if (bRetCode && uVersionLen && lpVersion)
			FileDesc = lpVersion;
		else
			FileDesc = "";

		lstrcpy(szGetName, "\\StringFileInfo\\040904b0\\");

		szGetName[wRootLen] = (char)0;
		lstrcat (szGetName, "ProductVersion");
		UINT	uVersionLen		= 0;
		LPSTR	lpVersion= NULL; // String pointer to 'version' text
		BOOL bRetCode	= VerQueryValue((LPVOID)lpstrVffInfo,
			(LPSTR)szGetName,	(LPVOID*)&lpVersion, (UINT *)&uVersionLen);
		if ( !bRetCode || !uVersionLen || !lpVersion)
			return ;

		COleDateTime odtRevision_Date;
		odtRevision_Date.ParseDateTime(__DATE__, VAR_DATEVALUEONLY, LANG_ENGLISH);
		version_time.wYear = (WORD)odtRevision_Date.GetYear();
		version_time.wMonth = (WORD)odtRevision_Date.GetMonth();
		version_time.wDay = (WORD)odtRevision_Date.GetDay();

		odtRevision_Date.ParseDateTime(__TIME__, VAR_TIMEVALUEONLY, LANG_ENGLISH);
		version_time.wHour = (WORD)odtRevision_Date.GetHour();
		version_time.wMinute = (WORD)odtRevision_Date.GetMinute();
		version_time.wSecond = (WORD)odtRevision_Date.GetSecond();
			
		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
}

#define _CREATE_LOG_
//#define _IGNORE_CONDITIONS_

//////////////////////////////////////////////////////////////////////////////
//Updater for security keys
/*
enum eKeyUpdaterAccess
{
	KUAnone = 0,
	KUAperm = 1,
	KUAtemp = 2,
};

bool IsPermanent(eKeyUpdaterAccess mode){ return mode == KUAperm;}
bool IsAccess(eKeyUpdaterAccess mode){ return (mode == KUAtemp) || IsPermanent(mode);}

enum eKeyUpdaterMode
{
	KUMreset_all = 0,//reset all data on the key
	KUMreset_exp = 1,//reset only expire flag
	KUMreset_none = 2,
};
*/
enum eKeyUpdaterRunCounterMode
{
	KURor = 0,
	KURthen = 1,
	KURnone = 2,
};
static unsigned short UpdaterKeyNumber = 0; // Introduced to keep all defines in one file without rewriting the organization, only a trick for compiling

class CKeyUpdater
{
public:
	WORD	m_KeyNumber;
	CString m_KeyName;
	long	m_validYear;
	long	m_validMonth;
	long	m_validDay;
	long	m_licYear;
	long	m_licMonth;
	long	m_licDay;
	long	m_ExpCounter;
//	eKeyUpdaterMode m_UpdaterMode;
	eKeyUpdaterRunCounterMode m_RunCounterMode;
	long*	m_pModules;
	CKeyUpdater(WORD key_num, /*eKeyUpdaterMode mode, */CString key_name,
					long valid_year, long valid_month, long valid_day, 
					long lic_year, long lic_month, long lic_day, 
					eKeyUpdaterRunCounterMode run_counter, long counter, long* pmodules):
					m_KeyNumber(key_num), /*m_UpdaterMode(mode),*/ m_KeyName(key_name), 
					m_validYear(valid_year), m_validMonth(valid_month), m_validDay(valid_day), 
					m_licYear(lic_year), m_licMonth(lic_month), m_licDay(lic_day), 
					m_ExpCounter(counter), m_RunCounterMode(run_counter)
	{
		ASSERT(pmodules);
		m_pModules = pmodules;
		m_pNext = m_pHead;
		m_pHead = this;
	};
	CKeyUpdater* m_pNext;
static CKeyUpdater* m_pHead;

static HRESULT CheckKey(WORD key_num)
{
	CKeyUpdater* pKey = m_pHead;
	while(NULL != pKey)
	{
		if (pKey->m_KeyNumber == key_num)
			break;
		else
			pKey = pKey->m_pNext;
	}
	if (NULL == pKey)//key not found
		return E_FAIL;
	return S_OK;
}

#define GET_DATA(mask) \
	WORD(_SSP60_##mask##_MASK_)

#define _UPDATE_DATA_(mem_var, mask)																\
	case SKM_##mask##:																				\
		KeyData.##mem_var = (WORD)((KeyData.##mem_var & ~GET_DATA(mask)) | GET_DATA(mask));	\
		break;

static HRESULT ModifyKey(CSSPro60Data& KeyData) {
	//find key
	CKeyUpdater* pKey = m_pHead;
	while(NULL != pKey) {
		if (pKey->m_KeyNumber == KeyData.m_KeyNumber)
			break;
		else
			pKey = pKey->m_pNext;
	}
	if (NULL == pKey)//key not found
		return E_FAIL;
	if (pKey->m_KeyName.IsEmpty())
		pKey->m_KeyName = KeyData.m_KeyName;
	KeyData.ClearAll();

	if (pKey->m_validMonth && pKey->m_validDay) {
		KeyData.m_ExpDate.m_Year	= pKey->m_validYear;
		KeyData.m_ExpDate.m_Month	= pKey->m_validMonth;
		KeyData.m_ExpDate.m_Day		= pKey->m_validDay;
	}

	if (pKey->m_licMonth && pKey->m_licDay)
	{
		KeyData.m_LicDate.m_Year	= pKey->m_licYear;
		KeyData.m_LicDate.m_Month	= pKey->m_licMonth;
		KeyData.m_LicDate.m_Day		= pKey->m_licDay;
	}

	KeyData.m_KeyNumber = pKey->m_KeyNumber;
	memcpy(KeyData.m_KeyName, pKey->m_KeyName.GetBuffer(_KEY_NAME_LENGTHW_SSP60_*2), _KEY_NAME_LENGTHW_SSP60_*2);
	pKey->m_KeyName.ReleaseBuffer();

	if (pKey->m_RunCounterMode == KURthen)
		KeyData.m_ExpUpdate |= _SSP60_EXPIRE_ORDER_FLAG_;
	else
		KeyData.m_ExpUpdate &= ~_SSP60_EXPIRE_ORDER_FLAG_;

	if (pKey->m_ExpCounter>0x7FF)
		pKey->m_ExpCounter=0x7FF;

	KeyData.m_ExpCounter = WORD(pKey->m_ExpCounter | _SSP60_COUNTER_FLAG_);

	KeyData.m_Updater_Version = UpdaterKeyNumber;

	SYSTEMTIME program_time;
	GetVersionInfoProgramTime(program_time);
	KeyData.m_BurnDate.m_Year = div(program_time.wYear,100).rem;
	KeyData.m_BurnDate.m_Month = program_time.wMonth;
	KeyData.m_BurnDate.m_Day = program_time.wDay;

	int index = 0;
//	eKeyUpdaterAccess module_access;
	while(SKM_UNKNOWN != pKey->m_pModules[index])
	{
//		module_access = (eKeyUpdaterAccess)pKey->m_pModules[index+1];
		switch(pKey->m_pModules[index])
		{
			_UPDATE_DATA_(m_FPM							, FPM				);
			_UPDATE_DATA_(m_Fracpro						, FRACPRO		);
			_UPDATE_DATA_(m_RawData_Stimpro			, STIMPRO		);
			_UPDATE_DATA_(m_RawData_Stimpro			, STIM_RAW_DATA);
			_UPDATE_DATA_(m_Respro_Cementpro			, RESPRO			);
			_UPDATE_DATA_(m_Respro_Cementpro			, CEMENTPRO		);
			_UPDATE_DATA_(m_Realpro						, REALPRO		);
			_UPDATE_DATA_(m_WTPA_Gradient_Toolpro	, TOOLPRO		);
			_UPDATE_DATA_(m_WTPA_Gradient_Toolpro	, GRADIENT		);
			_UPDATE_DATA_(m_WTPA_Gradient_Toolpro	, WTPA			);
			_UPDATE_DATA_(m_MaxproVM					, MAXPRO			);
			_UPDATE_DATA_(m_Fieldpro					, FIELDPRO		);
			_UPDATE_DATA_(m_PorPro_EcoPro_Cost		, FLOPRO			);
			_UPDATE_DATA_(m_PorPro_EcoPro_Cost		, ECOPRO			);
			_UPDATE_DATA_(m_PorPro_EcoPro_Cost		, COST			);
			_UPDATE_DATA_(m_GeoPro_LogPro				, GEOPRO			);
			_UPDATE_DATA_(m_GeoPro_LogPro				, LOGPRO			);
			_UPDATE_DATA_(m_TransPro					, TRANSPRO		);
			_UPDATE_DATA_(m_eFieldpro					, EFIELDPRO		);
			_UPDATE_DATA_(m_MaxproVM					, FIELDPRO_VM	);
		}
		index++;
	}

	return S_OK;
}

};

CKeyUpdater* CKeyUpdater::m_pHead = NULL;

#define KEY_UPDATE(number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, modules)								\
static long g_Key##number[] = {/modules/, SKM_UNKNOWN};																											\
static CKeyUpdater g_KeyUpdater##number(number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, g_Key##number);

#define CLIENT_NEW_SECTION	;

#include "UpdateKeys.h"
#undef KEY_UPDATE

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESkeyUpdateDlg dialog

CRESkeyUpdateDlg::CRESkeyUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRESkeyUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRESkeyUpdateDlg)
	m_KeyName = _T("");
	m_KeyNumber = _T("");
	m_KeysList = _T("");
	m_Message = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	UpdaterKeyNumber = UPDATER_VERSION_NUMBER;
	m_bOutputExists = false;

	char PathBuffer[_MAX_PATH];
	HMODULE	Module = AfxGetApp()->m_hInstance;
	if(GetModuleFileName(Module, PathBuffer, _MAX_PATH) != (DWORD)0)
	{
		m_sFilePath = PathBuffer;
		char Drive[_MAX_DRIVE];
		char Dir[_MAX_DIR];
		char Fname[_MAX_FNAME];
		char Ext[_MAX_EXT];
		_splitpath_s(m_sFilePath.GetBuffer(0), Drive, Dir, Fname, Ext);
		m_sFilePath = Drive;
		m_sFilePath += Dir;
	}
}

void CRESkeyUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESkeyUpdateDlg)
	DDX_Text(pDX, IDC_KEY_NAME, m_KeyName);
	DDX_Text(pDX, IDC_KEY_NUMBER, m_KeyNumber);
	DDX_Text(pDX, IDC_EDIT_KEYS, m_KeysList);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_Message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRESkeyUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CRESkeyUpdateDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnUpdate)
	ON_BN_CLICKED(ID_SendReport, OnSendReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESkeyUpdateDlg message handlers

BOOL CRESkeyUpdateDlg::OnInitDialog()
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
	
	CString StrBuffer;
	m_KeysList = _T("");
	CKeyUpdater* pKey = CKeyUpdater::m_pHead;
	while(NULL != pKey)
	{
		StrBuffer.Format("%04d ", pKey->m_KeyNumber);
		m_KeysList = StrBuffer + m_KeysList;
		pKey = pKey->m_pNext;
	}
	m_KeysList = _T("A list of the keys that can be updated: \r\n") + m_KeysList;

	m_Message = _T("Below is a list of the keys that this updater can reburn. You can attach each key listed one at a time to the computer, and click ""Update"" to reburn that key.  If the key has already been burned with this burner, it cannot be burned twice. If the key number is not listed, this reburner will not affect it.\r\n");
	m_Message += _T("The program will generate a report of the reburn. If you have any further questions or problems regarding the reburn, please press the ""Send Report"" option which will start your default mail application and include that reburn report in an e-mail to info@resnet.com, where you can describe your problem or question.");

	GetKeyInfo();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRESkeyUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRESkeyUpdateDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRESkeyUpdateDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRESkeyUpdateDlg::OnUpdate() 
{
  	CWaitCursor wc;
	CSSPro60Sec Key1, Key2;
	SEC_KEY_BUFFER Buf1, Buf2;
	memset(Buf1, 0, sizeof(Buf1));
	memset(Buf2, 0, sizeof(Buf2));

	LPCSTR MsgErrorNotInitKey1 = "Key before updating cannot be initialized. Error #106.";
	LPCSTR MsgErrorNotFoundKey1 =	"Key before updating not found. Error #107.";
	LPCSTR MsgErrorNotInitKey2 = 	"Key after updating cannot be initialized. Error #112."; 
	LPCSTR MsgErrorNotFoundKey2 =	"Key after updating not found. Error #113.";
	LPCSTR MsgErrorKeyAccessDenied = "Key Access Denied. Error #109.";
	LPCSTR MsgErrorBefferNotCorrect = "Errors while updating the key. Error #108.";
	LPCSTR MsgErrorNoErrors = "Key updated successfully. No errors.";
	LPCSTR ErrMsg = MsgErrorNoErrors;

	bool bError = false;

	if(FAILED(Key1.Init())) {
		ErrMsg = MsgErrorNotInitKey1;
		bError = true;
	}
	if(!bError && FAILED(Key1.OpenKey())) {
		ErrMsg = MsgErrorNotFoundKey1;
		bError = true;
	}
	if (!bError) {
		HRESULT hr = E_FAIL;
		if (SUCCEEDED(CKeyUpdater::ModifyKey(Key1.m_KeyData))) {
			hr = Key1.BurnKey();
			m_bOutputExists = true;
		}
		if(FAILED(hr)) {
			ErrMsg = MsgErrorKeyAccessDenied;
			bError = true;
		}

		if(!bError && FAILED(Key2.Init())) {
			ErrMsg = MsgErrorNotInitKey2;
			bError = true;
		}
		if(!bError && FAILED(Key2.OpenKey())) {
			ErrMsg = MsgErrorNotFoundKey2;
			bError = true;
		}

		Key1.m_KeyData.Serialize(true, Buf1);
		Key2.m_KeyData.Serialize(true, Buf2);

		if (!bError) {
			if (memcmp(Buf1, Buf2,sizeof(SEC_KEY_BUFFER))) {
				ErrMsg = MsgErrorBefferNotCorrect;
				bError = true;
			}
			else {
				MessageBox("Key updated successfully.");
				GetDlgItem(IDOK)->EnableWindow(FALSE);
			}
#ifdef _CREATE_LOG_
			CTime t = CTime::GetCurrentTime();
			m_sFileName_3.Format("%04d_KeyWrite_%04d-%02d-%02d_%02d-%02d.log", Key2.m_KeyData.m_KeyNumber,
				t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
			m_sFileName_3 = m_sFilePath+m_sFileName_3;
			Key2.CreateLogFile(m_sFileName_3.GetBuffer(0));
#endif
		}
	}
	if (bError)
		MessageBox(ErrMsg);
#ifdef _CREATE_LOG_
	if (m_sFileName_2.GetLength()>0) {
		CFile file;
		if (!file.Open(m_sFileName_2, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary)) {
			MessageBox("Updater log file cannot be created.");
			m_sFileName_2 = "";
		}
		else {
			file.SeekToEnd();
			file.Write(ErrMsg, strlen(ErrMsg));
			file.Write("\r\n", 2);
			file.Close();
		}
	}
#endif
}

bool IsDateOlder (SYSTEMTIME ref_time, SYSTEMTIME new_time) {
	if(div(new_time.wYear,100).rem > div(ref_time.wYear,100).rem)
		return false;
	if((div(new_time.wYear,100).rem == div(ref_time.wYear,100).rem) &&
		(new_time.wMonth > ref_time.wMonth))
		return false;
	if((div(new_time.wYear,100).rem == div(ref_time.wYear,100).rem) &&
		(new_time.wMonth == ref_time.wMonth) &&
		(new_time.wDay >= ref_time.wDay))
		return false;
	return true;
}

void CRESkeyUpdateDlg::GetKeyInfo()
{
  	CWaitCursor wc;
	LPCSTR MsgErrorNotInitKey = "Key cannot be initialized. Error #110.";
	LPCSTR MsgErrorNotFoundKey = "Key not found. Error #111.";
	LPCSTR MsgErrorWrongKey = "Program cannot update this key because key number is not correct. Error #101.";
	LPCSTR MsgErrorWrongVersion = "Program cannot update this key because current version of key updater was already used for this key. Error #102.";
	LPCSTR MsgErrorWrongDate = "Program cannot update this key because key updater was already used for this key. Error #103.";
	LPCSTR MsgErrorDateMess = "Program cannot update this key because of problems with the date. Error #104.";
	LPCSTR MsgErrorNoErrors = "Key information loading. No errors.";
	LPCSTR ErrMsg = MsgErrorNoErrors;

	BOOL bInitKey = true;
	BOOL bOpenKey = true;

	CSSPro60Sec Key;
	if(FAILED(Key.Init())) { 
		ErrMsg = MsgErrorNotInitKey;
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		bInitKey = false;
	}
	if(bInitKey && FAILED(Key.OpenKey())) {
		ErrMsg = MsgErrorNotFoundKey;
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		bOpenKey = false;
	}

	BOOL bCorrectKeyNumber = SUCCEEDED(CKeyUpdater::CheckKey(Key.m_KeyData.m_KeyNumber));
//	BOOL bNotUsedKey_Based_on_Version = Key.m_KeyData.m_Updater_Version<UPDATER_VERSION_NUMBER || Key.m_KeyData.m_Updater_Version==2005;
	BOOL bNotUsedKey_Based_on_Version = Key.m_KeyData.m_Updater_Version<UPDATER_VERSION_NUMBER;

	SYSTEMTIME current_time, key_burn_time, program_time;
	GetSystemTime(&current_time);
	key_burn_time.wYear = WORD(Key.m_KeyData.m_BurnDate.m_Year+2000);
	key_burn_time.wMonth = Key.m_KeyData.m_BurnDate.m_Month;
	key_burn_time.wDay = Key.m_KeyData.m_BurnDate.m_Day;
	BOOL bNotUsedKey_Based_on_DateMess = !IsDateOlder (key_burn_time, current_time);

	GetVersionInfoProgramTime(program_time);
	BOOL bNotUsedKey_Based_on_Date = !IsDateOlder (key_burn_time, program_time);

	BOOL bCanBeRun =	bInitKey && bOpenKey &&
							bNotUsedKey_Based_on_Version && bCorrectKeyNumber && 
							bNotUsedKey_Based_on_Date && bNotUsedKey_Based_on_DateMess;
	if (!bCanBeRun) {
		if (!bInitKey)
			ErrMsg = MsgErrorNotInitKey;
		else if (!bOpenKey)
			ErrMsg = MsgErrorNotFoundKey;
		else if (!bCorrectKeyNumber)
			ErrMsg = MsgErrorWrongKey;
		else if (!bNotUsedKey_Based_on_Version)
			ErrMsg = MsgErrorWrongVersion;
		else if (!bNotUsedKey_Based_on_Date)
			ErrMsg = MsgErrorWrongDate;
		else if (!bNotUsedKey_Based_on_DateMess)
			ErrMsg = MsgErrorDateMess;
		MessageBox(ErrMsg);
	}

#ifdef _IGNORE_CONDITIONS_
	bCanBeRun =	TRUE;
#endif

	m_KeyNumber.Format("%d", Key.m_KeyData.m_KeyNumber);
	char name[sizeof(Key.m_KeyData.m_KeyName)+1];
	memset(name, 0, sizeof(name));
	memcpy(name, Key.m_KeyData.m_KeyName, sizeof(Key.m_KeyData.m_KeyName));
	m_KeyName.Format("%s", name);

	if (bCanBeRun)
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	else
		GetDlgItem(IDOK)->EnableWindow(FALSE);

#ifdef _CREATE_LOG_
	CTime t = CTime::GetCurrentTime();
	m_sFileName_1.Format("%04d_KeyRead_%04d-%02d-%02d_%02d-%02d.log", Key.m_KeyData.m_KeyNumber,
		t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
	m_sFileName_1 = m_sFilePath+m_sFileName_1;
	Key.CreateLogFile(m_sFileName_1.GetBuffer(0));

	m_sFileName_2.Format("%02d_Updater_%04d-%02d-%02d.log", UPDATER_VERSION_NUMBER, 
		t.GetYear(),t.GetMonth(),t.GetDay());
	m_sFileName_2 = m_sFilePath+m_sFileName_2;
	CFile file;

	if (!file.Open(m_sFileName_2, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary)) {
		MessageBox("Updater log file cannot be created.");
		m_sFileName_2 = "";
	}
	else {
		file.SeekToEnd();
		CString StrBuffer;

		StrBuffer.Format("\r\nCurrent time: %04d/%02d/%02d %02d:%02d:%02d\r\n", 
			t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
		file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());

		StrBuffer.Format("Program made: %04d/%02d/%02d %02d:%02d:%02d\r\n",
			program_time.wYear,program_time.wMonth,program_time.wDay,program_time.wHour,program_time.wMinute,program_time.wSecond);
		file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());

		StrBuffer.Format("Updater version: %02d\r\n", UPDATER_VERSION_NUMBER);
		file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());

		StrBuffer.Format("Key to be updated: %04d\r\n", Key.m_KeyData.m_KeyNumber);
		file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());

		StrBuffer = "Keys allowed: ";
		file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());

		CKeyUpdater* pKey = CKeyUpdater::m_pHead;
		while(NULL != pKey)
		{
			StrBuffer.Format("%04d ", pKey->m_KeyNumber);
			file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());
			pKey = pKey->m_pNext;
		}

		if (IsAdminStarSec())
			StrBuffer = "\r\nAdministrator rights\r\n";
		else
			StrBuffer = "\r\nUser rights\r\n";
		file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());

		StrBuffer.Format("%s\r\n", ErrMsg);
		file.Write(StrBuffer.GetBuffer(0), StrBuffer.GetLength());

		file.Close();
	}
#endif

	UpdateData(FALSE);
}

#pragma warning(disable: 4228)
#include <mapi.h>
#pragma warning(default: 4228)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

void FillFileDesc(LPCSTR FilePath, MapiFileDesc *fileDesc, int &Pointer, LPSTR szTempName, LPSTR szTitle, LPSTR szName, LPSTR szExt) {
	if (FilePath && strlen(FilePath)>0) {
		lstrcpyn(szTempName, FilePath, _MAX_PATH);
		_splitpath_s(szTempName, NULL, 0, NULL, 0, szName, _MAX_FNAME, szExt, _MAX_EXT);
		strcpy_s(szTitle, _MAX_PATH, szName);
		strcat_s(szTitle, _MAX_PATH, szExt);
		fileDesc[Pointer].nPosition = (ULONG)-1;
		fileDesc[Pointer].lpszPathName = szTempName;
		fileDesc[Pointer].lpszFileName = szTitle;
		Pointer++;
	}
}

void SendMailWithAttachments(LPCTSTR lpszMailAddr, LPCTSTR lpszFile1, LPCTSTR lpszFile2, LPCTSTR lpszFile3, LPCTSTR lpszSubject,
									 LPCTSTR lpszNoteText)
{
	CWaitCursor wait;

	HINSTANCE hInstMail = ::LoadLibraryA("MAPI32.DLL");
	if (hInstMail == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_MAPI_LOAD);
		return;
	}
	ASSERT(hInstMail != NULL);

	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)lpfnSendMail = GetProcAddress(hInstMail, "MAPISendMail");
	if (lpfnSendMail == NULL)
	{
		AfxMessageBox(AFX_IDP_INVALID_MAPI_DLL);
		return;
	}
	ASSERT(lpfnSendMail != NULL);

	MapiFileDesc fileDesc[3];
	memset(&fileDesc[0], 0, sizeof(fileDesc[0]));
	memset(&fileDesc[1], 0, sizeof(fileDesc[1]));
	memset(&fileDesc[2], 0, sizeof(fileDesc[2]));
	int Pointer = 0;
		
	TCHAR szTempName1[_MAX_PATH], szTitle1[_MAX_PATH], szName1[_MAX_FNAME], szExt1[_MAX_EXT];
	TCHAR szTempName2[_MAX_PATH], szTitle2[_MAX_PATH], szName2[_MAX_FNAME], szExt2[_MAX_EXT];
	TCHAR szTempName3[_MAX_PATH], szTitle3[_MAX_PATH], szName3[_MAX_FNAME], szExt3[_MAX_EXT];

	FillFileDesc(lpszFile1, fileDesc, Pointer, szTempName1, szTitle1, szName1, szExt1);
	FillFileDesc(lpszFile2, fileDesc, Pointer, szTempName2, szTitle2, szName2, szExt2);
	FillFileDesc(lpszFile3, fileDesc, Pointer, szTempName3, szTitle3, szName3, szExt3);

	MapiMessage message;
	memset(&message, 0, sizeof(message));
	message.nFileCount = Pointer;
	message.lpFiles = fileDesc;

	TCHAR szSubject[80];
	if (!lpszSubject)
		lstrcpyn(szSubject, AfxGetAppName(), _countof(szSubject));
	else
		lstrcpyn(szSubject, lpszSubject, _countof(szSubject));
	message.lpszSubject = szSubject;
	if (lpszNoteText != NULL)
	{
		CString Str = lpszNoteText;
		message.lpszNoteText = Str.GetBuffer(0);
	}
	message.nRecipCount = 1;
	message.flFlags = MAPI_RECEIPT_REQUESTED;

	MapiRecipDesc RecipDesc;
	memset(&RecipDesc, 0, sizeof(RecipDesc));
	RecipDesc.ulRecipClass = MAPI_TO;
	TCHAR szAddress[101];
	strcpy_s(szAddress, _T("SMTP:"));
	strcat_s(szAddress, 100, lpszMailAddr);
	RecipDesc.lpszAddress = szAddress;
	RecipDesc.lpszName = _T("RES Technical Support");
	message.lpRecips = &RecipDesc;

	// prepare for modal dialog box
	AfxGetApp()->EnableModeless(FALSE);
	HWND hWndTop;
	CWnd* pParentWnd = CWnd::GetSafeOwner(NULL, &hWndTop);

	// some extra precautions are required to use MAPISendMail as it
	// tends to enable the parent window in between dialogs (after
	// the login dialog, but before the send note dialog).
	pParentWnd->SetCapture();
	::SetFocus(NULL);
	pParentWnd->m_nFlags |= WF_STAYDISABLED;

	int nError = lpfnSendMail(0, (ULONG)pParentWnd->GetSafeHwnd(),
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

	// after returning from the MAPISendMail call, the window must
	// be re-enabled and focus returned to the frame to undo the workaround
	// done before the MAPI call.
	::ReleaseCapture();
	pParentWnd->m_nFlags &= ~WF_STAYDISABLED;

	pParentWnd->EnableWindow(TRUE);
	::SetActiveWindow(NULL);
	pParentWnd->SetActiveWindow();
	pParentWnd->SetFocus();
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
	AfxGetApp()->EnableModeless(TRUE);

	if (nError != SUCCESS_SUCCESS &&
		nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
	{
		AfxMessageBox(AFX_IDP_FAILED_MAPI_SEND);
	}

	::FreeLibrary(hInstMail);
}


void CRESkeyUpdateDlg::OnSendReport() 
{
	CWaitCursor wait;
	CSSPro60Sec Key;

	Key.Init();
	Key.OpenKey();
	CTime t = CTime::GetCurrentTime();
	CString SubjectText;
	SubjectText.Format("Key Updater Report %04d %04d/%02d/%02d %02d:%02d", Key.m_KeyData.m_KeyNumber,
		t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());

	LPSTR pThirdFile;
	if (m_bOutputExists)
		pThirdFile = m_sFileName_3.GetBuffer(0);
	else
		pThirdFile = NULL;

	SendMailWithAttachments("info@resnet.com", m_sFileName_1.GetBuffer(0), m_sFileName_2.GetBuffer(0), pThirdFile, SubjectText.GetBuffer(0), NULL);
}
