// regkeyboxDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "SysRegKey.h"
#include "SysRegKeyService.h"
#include "SysService.h"
#include "SysSecurity.h"

#include <bitset>

// Following is for a registration key
#define SECTIONS			 4
#define CHARSPERSECTION		 4
#define BITSPERCHAR			 5
#define TOTALCHARS CHARSPERSECTION*SECTIONS
#define KEYLENGTH SECTIONS*(CHARSPERSECTION+1) // including delimiter
#define BITSTREAMLENGTH BITSPERCHAR*TOTALCHARS
#define BYTESTREAMLENGTH BITSTREAMLENGTH/8 // should be an integer!
typedef unsigned char byte;
typedef std::bitset<BITSTREAMLENGTH> bitstream;

// CregkeyboxDlg dialog
class CregkeyboxDlg : public CDialog
{
// Construction
public:
	CregkeyboxDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_REGKEYBOX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CFile f;
	CFileException e;
	char* pFileName;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnNMOutofmemoryHotkey1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio3();
	CButton somegroup;
	CEdit port;
	afx_msg void OnBnClickedRadio4();
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	bool Trust(CString&); // check the registration key
	BOOL smoosh;
	CString sPort;
	CString sRN;
	
private:
	inline byte getbyte(bitstream& bs,int i); // trust() internal
	inline byte getbit(bitstream& bs,int i); // trust() internal

public:
	CEdit cRN;
};
