// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryTreeCtrl.h"

class CMainFrame : public CFrameWnd
{
	
public:

	class Client
	{
	public:
		virtual bool IsScanning() const = 0;
	};

	CMainFrame( Client * pClient );
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
//	CStatusBar m_wndStatusBar;
	CToolBar m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnUpdateFileNewScan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileRescan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewDisplayFiles(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDisplayFoldersOnly(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDisplayFiles10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDisplayFiles90(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	Client * m_pApp;

public:
	CDirectoryTreeCtrl m_wndTreeCtrl;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
