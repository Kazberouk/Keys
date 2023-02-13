// RESkeyUpdate.h : main header file for the RESKEYUPDATE application
//

#if !defined(AFX_RESKEYUPDATE_H__2B3C2C2D_FBA3_490B_84DB_8F8F507E7801__INCLUDED_)
#define AFX_RESKEYUPDATE_H__2B3C2C2D_FBA3_490B_84DB_8F8F507E7801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRESkeyUpdateApp:
// See RESkeyUpdate.cpp for the implementation of this class
//

class CRESkeyUpdateApp : public CWinApp
{
public:
	CRESkeyUpdateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRESkeyUpdateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRESkeyUpdateApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESKEYUPDATE_H__2B3C2C2D_FBA3_490B_84DB_8F8F507E7801__INCLUDED_)
