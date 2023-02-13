// LicenseGenerator.h : main header file for the LicenseGenerator application
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLicenseGeneratorApp:
// See LicenseGenerator.cpp for the implementation of this class
//

class CLicenseGeneratorApp : public CWinApp
{
public:
	CLicenseGeneratorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLicenseGeneratorApp)
	public:
	virtual BOOL InitInstance();
	int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLicenseGeneratorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

