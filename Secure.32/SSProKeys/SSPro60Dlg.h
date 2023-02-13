// SSPro60Dlg.h : header file
//
#pragma once

#include "../Utils.32/StarlingPro/SSPro60Sec.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CSSPro60Dlg dialog

class CSSPro60Dlg : public CDialog
{
// Construction
public:
	CSSPro60Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSSPro60Dlg)
	enum { IDD = IDD_SSPRO60_DIALOG };
	int				m_KeyNumber;
	CString			m_KeyName;
	COleDateTime	m_ExpDate;
	CComboBox		m_ExpOrderCombo;
	int				m_RunsCount;
	COleDateTime	m_LicDate;

	COleDateTime	m_vBurnDate;
	CDateTimeCtrl	m_cBurnDate;
	int				m_UseCurrentDate;

	int				m_vVersionNumber;
	CEdit			m_cVersionNumber;
	BOOL			m_UseCurrentVersion;

	int				m_AllFPM;
	int				m_AllFIP;
	CButton			m_cMakeKey;
	CButton			m_cMakeKeyFile;
	CComboBox		m_cComboKeyList;
	int				m_iComboKeyList;
	CString			m_strKeyList;

	int	m_FRACPRO;
	int	m_STIMPRO;
	int	m_CEMENTPRO;
	int	m_RESPRO;
	int	m_REALPRO;
	int	m_TOOLPRO;
	int	m_MAXPRO;
	int	m_WTPA;
	int	m_FRAC_SCREEN;

	int	m_FIELDPRO;
	int	m_FLOPRO;
	int	m_ECOPRO;
	int	m_LOGPRO;

	int	m_VM;
	//}}AFX_DATA

	CString			m_strKeysFile;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSPro60Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	void SetKeyDataFromDialog(CSSPro60Data &KeyData);
	void LoadDialogFromKeyData(CSSPro60Data &KeyData);
	void ResetKeyList();
	void SaveKeyStatus();

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSSPro60Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMakeKey();
	afx_msg void OnIncreaseKeyNumber();
	afx_msg void OnGetinfo();
	afx_msg void OnLoadLog();
	afx_msg void OnLoadKeyFile();
	afx_msg void OnMakeKeyFile();
	afx_msg void OnSetCurrentDate();
	afx_msg void OnAllFPM();
	afx_msg void OnAllFIP();
	afx_msg void OnSetCurrentVersion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeComboLoadedKey();
};
