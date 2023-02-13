// SSPro60.h : main header file for the SSPRO60 application
//

#if !defined(AFX_SSPRO60_H__AC776035_3022_11D5_B522_00C0F6B1F5A5__INCLUDED_)
#define AFX_SSPRO60_H__AC776035_3022_11D5_B522_00C0F6B1F5A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSSPro60App:
// See SSPro60.cpp for the implementation of this class
//

class CSSPro60App : public CWinApp
{
public:
	CSSPro60App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSPro60App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSSPro60App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSPRO60_H__AC776035_3022_11D5_B522_00C0F6B1F5A5__INCLUDED_)
