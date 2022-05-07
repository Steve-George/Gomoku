
// GomokuDlg.cpp : implementation file
//
#include "afx.h"
#include "pch.h"
#include "windows.h"
#include "framework.h"
#include "Gomoku.h"
#include "GomokuDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGomokuDlg dialog


CGomokuDlg::CGomokuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GOMOKU_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGomokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGomokuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_GAME_NEWGAME, &CGomokuDlg::OnGameNewgame)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGomokuDlg message handlers

BOOL CGomokuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here
	SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOZORDER | SWP_NOMOVE);
	m_FontTimer.CreatePointFont(250, _T("Segoe UI Semibold"), NULL);
	m_FontOver.CreatePointFont(1000, _T("微软雅黑"), NULL);
	m_bState = false;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGomokuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGomokuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CPaintDC dc(this);
		CBitmap bmp;
		BITMAP bm;
		bmp.LoadBitmap(IDB_BITMAP_BK);
		bmp.GetObject(sizeof(BITMAP), &bm);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap* pOldBitmap = MemDC.SelectObject(&bmp);
		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		m_Manager.Show(&dc);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGomokuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGomokuDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (NewGame(point.x, point.y))
		return;
	if (About(point.x, point.y))
		return;
	if (!m_bState)
	{
		AfxMessageBox(_T("请选择“开始”按钮开始新的游戏，按Esc键退出游戏！"));
		return;
	}
	int r = m_Manager.Add(point.x, point.y);
	if (r == 0)
	{
		CClientDC dc(this);
		m_Manager.Show(&dc);
		if (m_Manager.GameOver())
		{
			KillTimer(1);
			CString csTemp;
			if (m_Manager.GetWinner() == WHITE)
				csTemp.Format(_T("白方胜！"));
			else
				csTemp.Format(_T("黑方胜！"));
			m_bState = false;
			CClientDC dc(this);
			CFont* pOldFont = dc.SelectObject(&m_FontOver);
			int OldBkMode = dc.GetBkMode();
			COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
			dc.SetBkMode(TRANSPARENT);
			OldColor = dc.SetTextColor(NewColor1);
			dc.TextOut(158, 208, csTemp);
			dc.SetTextColor(NewColor2);
			dc.TextOut(150,200, csTemp);
			dc.SetTextColor(OldColor);
			dc.SetBkMode(OldBkMode);
			dc.SelectObject(pOldFont);
		}
	}
	if (r == 1)
		AfxMessageBox(_T("请在棋盘交叉点落子！"));
	else if(r==2)
		AfxMessageBox(_T("不可以重复落子！"));
	CDialogEx::OnLButtonUp(nFlags, point);
}

bool CGomokuDlg::NewGame(int x, int y)
{
	int x0 = 35, y0 = 150, x1 = 200, y1 = 185;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1))
	{
		m_Manager.NewGame();
		Invalidate();
		m_iTime = 0;
		SetTimer(1, 1000, NULL);
		m_bState = true;
		return true;
	}
	return false;
}

bool CGomokuDlg::About(int x, int y)
{
	int x0 = 35, y0 = 70, x1 = 200, y1 = 95;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1))
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return true;
	}
	return false;
}

void CGomokuDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			CClientDC dc(this);
			CFont* pOldFont;
			pOldFont = dc.SelectObject(&m_FontTimer);
			m_iTime++;
			CString csTemp;
			csTemp.Format(_T("%04d "), m_iTime);
			COLORREF OldColor, NewColor = RGB(150, 50, 50);
			OldColor = dc.SetTextColor(NewColor);
			//dc.SetBkMode(TRANSPARENT);
			dc.TextOut(725, 7, csTemp);
			dc.SetTextColor(OldColor);
			dc.SelectObject(pOldFont);
			break;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CGomokuDlg::OnGameNewgame()
{
	// TODO: Add your command handler code here
	NewGame(50, 150);
}
