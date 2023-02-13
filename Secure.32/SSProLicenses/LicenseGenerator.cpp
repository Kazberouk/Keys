// LicenseGenerator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LicenseGenerator.h"
#include "LicenseGeneratorDlg.h"
#include "ProCom.Src/LibraryLTMInfoImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "..\Common\Interfaces\RegScreenHelpr.h"
#include "Interfaces/RegScreenHelpr.h"

DECLARE_SCREENS_CHAIN


/////////////////////////////////////////////////////////////////////////////
// CLicenseGeneratorApp

BEGIN_MESSAGE_MAP(CLicenseGeneratorApp, CWinApp)
	//{{AFX_MSG_MAP(CLicenseGeneratorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseGeneratorApp construction

CLicenseGeneratorApp::CLicenseGeneratorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLicenseGeneratorApp object

CLicenseGeneratorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLicenseGeneratorApp initialization

void GenerateSingleLicenseFile();
void GenerateMultipleLicenseFiles();
void GenerateLicenseFilesReport();

BOOL CLicenseGeneratorApp::InitInstance() {
	REGISTER_SCREENS

	AfxEnableControlContainer();

	GenerateSingleLicenseFile();
	GenerateMultipleLicenseFiles();
//	GenerateLicenseFilesReport();

	return FALSE;
}

int CLicenseGeneratorApp::ExitInstance() {
	FreeLibraryLTMInfoHandler(true);
	return CWinApp::ExitInstance();
}