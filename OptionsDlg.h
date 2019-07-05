#pragma once

#include "resource.h"

// COptionsDlg dialog

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Rescan after delete if TRUE
	BOOL m_bRescanAfterDelete;
	// Copy deleted files to the Recycle Bin if TRUE
	BOOL m_bCopyToRecycleBin;
	// Ask for confirmation when a folder is deleted if TRUE
	BOOL m_bConfirmFolderDeletion;
};
