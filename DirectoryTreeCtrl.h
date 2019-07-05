#pragma once

// DirectoryTreeCtrl.h : header file
//

#include "FileTree.h"

/////////////////////////////////////////////////////////////////////////////
// CDirectoryTreeCtrl window

class CDirectoryTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CDirectoryTreeCtrl();

	BOOL Create( RECT const & rect, CWnd * pParentWnd, UINT nID );

// Attributes
public:

	enum DisplayMode
	{
		DM_NONE,			// Invalid option
		DM_FOLDERS,			// Folders only
		DM_FILES,			// Folders and files
		DM_10,				// Files and nodes that are 10% or more of the contents of the containing node
		DM_90				// All files and nodes except the smallest 10% of the contents of the containing node
	};

	enum NodeStringFormatOption
	{
		NSFO_NORMAL,
		NSFO_RIGHT_JUSTIFY_SIZE_WITH_DASHES,
		NSFO_RIGHT_JUSTIFY_SIZE_WITH_SPACES
	};

	DisplayMode GetDisplayMode() const	{ return m_DisplayMode; }

// Operations
public:

	// Rebuild the tree control using this node tree
	void Rebuild( FileTree::Node const * pRoot, DisplayMode mode );

	CString BuildNodeString( CString const & name, __int64 const & size, NodeStringFormatOption option = NSFO_NORMAL );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectoryTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDirectoryTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDirectoryTreeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	DisplayMode m_DisplayMode;		// Display mode

	// Insert (recursively) this node and its sub-nodes into the tree
	void  InsertFileNode( FileTree::Node const * pNode, HTREEITEM hParent );
//	void InsertFileItem( CString const & path, HTREEITEM hParent, __int64 const & size );
//public:
//
//	// Returns true if the control is scanning
//	bool IsScanning() const						{ return m_bScanning; }
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//}}AFX_INSERT_LOCATION}}
