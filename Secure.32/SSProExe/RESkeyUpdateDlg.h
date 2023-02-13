// RESkeyUpdateDlg.h : header file
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRESkeyUpdateDlg dialog

class CRESkeyUpdateDlg : public CDialog
{
// Construction
public:
	CRESkeyUpdateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRESkeyUpdateDlg)
	enum { IDD = IDD_RESKEYUPDATE_DIALOG };
	CString	m_KeyName;
	CString	m_KeyNumber;
	CString	m_KeysList;
	CString	m_Message;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRESkeyUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	CString m_sFileName_1, m_sFileName_2, m_sFileName_3, 	m_sFilePath;
	bool m_bOutputExists;
protected:
	void GetKeyInfo();
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRESkeyUpdateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUpdate();
	afx_msg void OnSendReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

