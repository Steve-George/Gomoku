
// GomokuDlg.h : header file
//

#pragma once
#include "CChessManager.h"

// CGomokuDlg dialog
class CGomokuDlg : public CDialogEx
{
	
	CChessManager m_Manager;
	CFont m_FontTimer;
	CFont m_FontOver;
	int m_iTime;
	bool m_bState;
// Construction
public:
	CGomokuDlg(CWnd* pParent = nullptr);	// standard constructor
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool NewGame(int x, int y);
	bool About(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOMOKU_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGameNewgame();
};
