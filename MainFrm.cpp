// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#include "MainFrm.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEWSCAN, OnUpdateFileNewScan)
	ON_UPDATE_COMMAND_UI(ID_FILE_RESCAN, OnUpdateFileRescan)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP, OnUpdateFileStop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYFILES, OnUpdateViewDisplayFiles)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYFOLDERSONLY, OnUpdateViewDisplayFoldersOnly)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYFILES10, OnUpdateViewDisplayFiles10)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYFILES90, OnUpdateViewDisplayFiles90)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame( Client * pClient )
{
	m_pApp = pClient;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a tree control to occupy the client area of the frame
	if ( !m_wndTreeCtrl.Create( CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST ) )
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

//	if (!m_wndStatusBar.Create(this) ||
//		!m_wndStatusBar.SetIndicators(indicators,
//		  sizeof(indicators)/sizeof(UINT)))
//	{
//		TRACE0("Failed to create status bar\n");
//		return -1;      // fail to create
//	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndTreeCtrl.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndTreeCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnUpdateFileNewScan(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( !m_pApp->IsScanning() );
}

void CMainFrame::OnUpdateFileRescan(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( !m_pApp->IsScanning() );
}

void CMainFrame::OnUpdateFileStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( m_pApp->IsScanning() );
}

void CMainFrame::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	// TODO: Enable once printing works
	pCmdUI->Enable( false );
//	pCmdUI->Enable( !m_pApp->IsScanning() );
}

void CMainFrame::OnUpdateViewDisplayFiles(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_wndTreeCtrl.GetDisplayMode() == CDirectoryTreeCtrl::DM_FILES );
	pCmdUI->Enable( !m_pApp->IsScanning() );
}

void CMainFrame::OnUpdateViewDisplayFoldersOnly(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_wndTreeCtrl.GetDisplayMode() == CDirectoryTreeCtrl::DM_FOLDERS );
	pCmdUI->Enable( !m_pApp->IsScanning() );
}

void CMainFrame::OnUpdateViewDisplayFiles10(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_wndTreeCtrl.GetDisplayMode() == CDirectoryTreeCtrl::DM_10 );
	pCmdUI->Enable( !m_pApp->IsScanning() );
}

void CMainFrame::OnUpdateViewDisplayFiles90(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_wndTreeCtrl.GetDisplayMode() == CDirectoryTreeCtrl::DM_90 );
	pCmdUI->Enable( !m_pApp->IsScanning() );
}
