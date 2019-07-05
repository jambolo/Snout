// Snout.h : main header file for the SNOUT application
//

#pragma once

#include "MainFrm.h"
#include "FileTree.h"
#include "DirectoryTreeCtrl.h"

#include "Misc/PathName.h"

/////////////////////////////////////////////////////////////////////////////
// CSnoutApp:
// See Snout.cpp for the implementation of this class
//

class CSnoutApp : public CWinApp, public CMainFrame::Client
{
public:
	CSnoutApp();
	~CSnoutApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSnoutApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	// Overrides CMainFrame::Client
	virtual bool IsScanning() const;

// Implementation

public:
	//{{AFX_MSG(CSnoutApp)
	afx_msg void OnFileNewScan();
	afx_msg void OnFileRescan();
	afx_msg void OnFileStop();
	afx_msg void OnFilePrint();
	afx_msg void OnViewDisplayFiles();
	afx_msg void OnViewDisplayFoldersOnly();
	afx_msg void OnViewDisplayFiles10();
	afx_msg void OnViewDisplayFiles90();
	afx_msg void OnToolsDelete();
	afx_msg void OnToolsOptions();
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	// Build the tree
	void Scan( PathName const & root );

	// Display the tree
	void Display( CDirectoryTreeCtrl::DisplayMode mode = CDirectoryTreeCtrl::DM_NONE );

	__int64 InsertFolderItem( FileTree::Node * pParent,
							  PathName const & name,
							  PathName const & path,
							  unsigned __int32 attributes );
	void InsertFileItem( FileTree::Node * pParent,
						 PathName const & name,
						 PathName const & path,
						 unsigned __int32 attributes,
						 __int64 size );

	// The root node of the tree
	FileTree::Node *	m_pRoot;
	// Automatically rescan after deleting files if true
	bool m_bRescanAfterDelete;
	// Copy deleted files to the Recycle Bin if true
	bool m_bCopyToRecycleBin;
	// Do not confirm folder deletions if true
	bool m_bConfirmFolderDeletion;
	// True while scanning
	bool m_bScanning;

	// Return a pointer to the CMainFrame window
	CMainFrame * GetMainFrameWnd()		{ return static_cast< CMainFrame * >( m_pMainWnd ); }

	// Return a pointer to the view
	CDirectoryTreeCtrl * GetView()		{ return &GetMainFrameWnd()->m_wndTreeCtrl; }
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
