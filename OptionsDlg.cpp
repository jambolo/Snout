// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"

#include "OptionsDlg.h"

// COptionsDlg dialog

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)
COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
	, m_bRescanAfterDelete(FALSE)
	, m_bCopyToRecycleBin(FALSE)
	, m_bConfirmFolderDeletion(FALSE)
{
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_OPTIONS_RESCAN, m_bRescanAfterDelete);
	DDX_Check(pDX, IDC_OPTIONS_RECYCLE, m_bCopyToRecycleBin);
	DDX_Check(pDX, IDC_OPTIONS_CONFIRMFOLDERDELETE, m_bConfirmFolderDeletion);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
END_MESSAGE_MAP()


// COptionsDlg message handlers
