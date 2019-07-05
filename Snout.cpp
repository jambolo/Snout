// Snout.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "Snout.h"

#include "MainFrm.h"
#include "AboutDlg.h"
#include "OptionsDlg.h"
#include "resource.h"

#include "Misc/PathName.h"
#include "Misc/SafeStr.h"

#include <algorithm>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{

void SystemErrorMessageBox( UINT nType = MB_OK )
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
		( LPTSTR ) &lpMsgBuf,
		0,
		NULL 
		);
	AfxMessageBox( ( LPCTSTR )lpMsgBuf, nType );

	// Free the buffer.
	LocalFree( lpMsgBuf );
}

} // unnamed namespace


/////////////////////////////////////////////////////////////////////////////
// CSnoutApp

BEGIN_MESSAGE_MAP(CSnoutApp, CWinApp)
	//{{AFX_MSG_MAP(CSnoutApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_COMMAND(ID_FILE_NEWSCAN, OnFileNewScan)
	ON_COMMAND(ID_FILE_RESCAN, OnFileRescan)
	ON_COMMAND(ID_FILE_STOP, OnFileStop)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_VIEW_DISPLAYFILES, OnViewDisplayFiles)
	ON_COMMAND(ID_VIEW_DISPLAYFOLDERSONLY, OnViewDisplayFoldersOnly)
	ON_COMMAND(ID_VIEW_DISPLAYFILES10, OnViewDisplayFiles10)
	ON_COMMAND(ID_VIEW_DISPLAYFILES90, OnViewDisplayFiles90)
	ON_COMMAND(ID_TOOLS_DELETE, OnToolsDelete)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSnoutApp construction

CSnoutApp::CSnoutApp()
	: m_pRoot( 0 )
	, m_bRescanAfterDelete( false )
	, m_bCopyToRecycleBin( false )
	, m_bConfirmFolderDeletion( true )
	, m_bScanning( false )
{
}

CSnoutApp::~CSnoutApp()
{
	delete m_pRoot;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSnoutApp object

CSnoutApp theApp;


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

BOOL CSnoutApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	// Change the registry key under which our settings are stored.
	// such as the name of your company or organization.
//	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame( this );
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW/* | FWS_ADDTOTITLE*/, NULL, NULL);

	// The has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

bool CSnoutApp::IsScanning() const
{
	return m_bScanning;
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::Scan( PathName const & root )
{
	CWaitCursor wait_cursor;
	m_bScanning = true;

	if ( m_pRoot != 0 )
	{
		delete m_pRoot;
		m_pRoot = 0;
	}

	InsertFolderItem( 0, root, root, 0 );

	m_bScanning = false;
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

__int64 CSnoutApp::InsertFolderItem( FileTree::Node * pParent,
									 PathName const & name,
									 PathName const & path,
									 unsigned __int32 attributes )
{
	// Warning: This function is recursive

	// Create a new node

	FileTree::Node * const	pFolder	= new FileTree::Node( name, path, attributes );

	// Insert it into the parent's list of nodes

	if ( pParent == 0 )
	{
		ASSERT( m_pRoot == 0 );
		m_pRoot = pFolder;
	}
	else
	{
		pParent->m_Nodes.push_back( pFolder );
	}

	// Go through all the files and subnodes and total the size

	__int64 size = 0;

	// Set up the find file stuff and find the first

	PathName const find_string = path + "\\*";
	WIN32_FIND_DATA find_data;
	HANDLE hFindFile = FindFirstFile( find_string.c_str(), &find_data ); 

	// If there is an error abort

	if ( hFindFile == INVALID_HANDLE_VALUE )
	{
#if defined( _DEBUG )
		SystemErrorMessageBox( MB_OK | MB_ICONINFORMATION );
#endif // defined( _DEBUG )

		return 0;
 	}

	do
	{
		// Skip "." and ".." and "System Volume Information"

		if (    strcmp( ".", find_data.cFileName ) == 0
			 || strcmp( "..", find_data.cFileName ) == 0
			 || strcmp( "System Volume Information", find_data.cFileName ) == 0 )
		{
			continue;
		}

		// Construct the full path

		PathName const subPath = path + "\\" + find_data.cFileName;

		// Insert the item and get its size

		__int64	nodeSize;

		if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			nodeSize = InsertFolderItem( pFolder, find_data.cFileName, subPath, find_data.dwFileAttributes );
		}
		else
		{
			nodeSize = ( __int64( find_data.nFileSizeHigh ) << 32 ) + find_data.nFileSizeLow;

			InsertFileItem( pFolder, find_data.cFileName, subPath, find_data.dwFileAttributes, nodeSize );
		}

		// Accumulate the size of this item

		size += nodeSize;

	} while ( FindNextFile( hFindFile, &find_data ) );

	// If FindNextFile failed, display a message

	if ( GetLastError() != ERROR_NO_MORE_FILES )
	{
		SystemErrorMessageBox( MB_OK | MB_ICONINFORMATION );
 	}

	// Clean up the WIN32_FIND_DATA data. If there is an error, display a message.

	if ( FindClose( hFindFile ) == FALSE )
	{
		SystemErrorMessageBox( MB_OK | MB_ICONINFORMATION );
	}

	// Fill in the rest of the data for this node

	pFolder->m_Size = size;

	// Now sort the contents based on descending size

	std::sort( pFolder->m_Nodes.begin(), pFolder->m_Nodes.end(), FileTree::Node::SorterByDescendingSize() );

	return size;
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::InsertFileItem( FileTree::Node * pParent,
							    PathName const & name,
								PathName const & path,
								unsigned __int32 attributes,
								__int64 size )
{
	// Create a new node

	FileTree::Node * const	pFile	= new FileTree::Node( name, path, attributes, size );
	if ( !pFile ) throw std::bad_alloc();

	// Insert it into the parent's list of nodes

	pParent->m_Nodes.push_back( pFile );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::Display( CDirectoryTreeCtrl::DisplayMode mode/* = CDirectoryTreeCtrl::DM_NONE*/ )
{
	// Rebuild the tree control

	CDirectoryTreeCtrl * const	pTreeCtrl	= GetView();
	pTreeCtrl->Rebuild( m_pRoot, mode );
}



/////////////////////////////////////////////////////////////////////////////
// CSnoutApp message handlers


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnFileNewScan()
{
    LPMALLOC pMalloc;
    if ( ::SHGetMalloc( &pMalloc ) == NOERROR )
    {
		char	directory_name[ MAX_PATH ];

		BROWSEINFO bi =
		{
			m_pMainWnd->GetSafeHwnd(),
			NULL,
			directory_name,	// not used
			_T( "Select the folder or drive to scan:" ),
			BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS,
			NULL,
			0
		};

		LPITEMIDLIST pidl = ::SHBrowseForFolder( &bi );

		if ( pidl != NULL )
		{
			PathName	rootName;

			// Get the file name from the file dialog and set it

            SHGetPathFromIDList( pidl, directory_name );
			pMalloc->Free( pidl );

			rootName = directory_name;

			// Trim any terminating baskslash

			if ( !rootName.empty() && *(rootName.end()-1) == '\\' )
			{
				rootName.erase( rootName.end()-1 );
			}

			// Scan

			Scan( rootName );
			Display();
		}

		pMalloc->Release();
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnFileRescan()
{
	// Scan

	if ( m_pRoot != 0 )
	{
		Scan( m_pRoot->m_Name );
		Display();
	}
	else
	{
		OnFileNewScan();
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnFileStop()
{
	// TODO: Implement CSnoutApp::OnFileStop
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnFilePrint()
{
	// TODO: Implement CSnoutApp::OnFilePrint

//	CPrintDialog print_dialog;
//	
//	print_dialog.m_pd.
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnViewDisplayFiles() 
{
	Display( CDirectoryTreeCtrl::DM_FILES );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnViewDisplayFoldersOnly() 
{
	Display( CDirectoryTreeCtrl::DM_FOLDERS );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnViewDisplayFiles10() 
{
	Display( CDirectoryTreeCtrl::DM_10 );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnViewDisplayFiles90() 
{
	Display( CDirectoryTreeCtrl::DM_90 );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnToolsDelete()
{
	CDirectoryTreeCtrl * const	pTreeCtrl		= GetView();
	HTREEITEM const				selectedItem	= pTreeCtrl->GetSelectedItem();

	// If nothing is selected, then abort

	if ( selectedItem == NULL )
	{
		return;
	}


	bool					succeeded;
	FileTree::Node * const	pSelectedNode	= reinterpret_cast< FileTree::Node * >( pTreeCtrl->GetItemData( selectedItem ) );

	// Copy the path to a buffer because a double-NULL is needed for SHFileOperation.

	char buffer[ MAX_PATH + 1 ];
	memset( buffer, 0, sizeof( buffer ) );
	SafeStrcpy( buffer, pSelectedNode->m_Path.c_str(), MAX_PATH );

	// Set up for the delete

	SHFILEOPSTRUCT	fileOp =
	{
		m_pMainWnd->GetSafeHwnd(),				//HWND hwnd; 
		FO_DELETE,								//UINT wFunc; 
		buffer,									//LPCTSTR pFrom; 
		NULL,									//LPCTSTR pTo; 
		0,	/* filled in below */				//FILEOP_FLAGS fFlags; 
		FALSE,									//BOOL fAnyOperationsAborted; 
		NULL,									//LPVOID hNameMappings; 
		NULL									//LPCTSTR lpszProgressTitle; 
	};

	if ( m_bCopyToRecycleBin )
	{
		fileOp.fFlags |= FOF_WANTNUKEWARNING|FOF_ALLOWUNDO;	// Move the file to the recycle bin and confirm if deleted instead
	}
	
	if ( !m_bConfirmFolderDeletion || pSelectedNode->m_Nodes.size() == 0 )
	{
		fileOp.fFlags |= FOF_NOCONFIRMATION;				// Don't ask for confirmations
	}

	// Delete the file

	succeeded = ( SHFileOperation( &fileOp ) == 0 );

	// If it was not successful then report an error

	if ( !succeeded )
	{
		std::ostringstream message;
		message <<  "An error was reported while trying to delete '" << pSelectedNode->m_Path.c_str() << "'.";
		AfxMessageBox( message.str().c_str(), MB_OK );

		return;
	}

	// If the operation was canceled, then don't change the display -- just abort

	if ( fileOp.fAnyOperationsAborted != FALSE )
	{
		return;
	}

	// If the immediate rescan option is set, then rescan. Otherwise, fix up the display.

	if ( m_bRescanAfterDelete )
	{
		OnFileRescan();
	}
	else
	{
		// Subtract the size of the deleted item from all the folders that contain it and fix the displayed text

		for ( HTREEITEM hItem = pTreeCtrl->GetParentItem( selectedItem );
			  hItem != NULL;
			  hItem = pTreeCtrl->GetParentItem( hItem ) )
		{
			FileTree::Node * const	pNode	= reinterpret_cast< FileTree::Node * >( pTreeCtrl->GetItemData( hItem ) );

			pNode->m_Size -= pSelectedNode->m_Size;
			pTreeCtrl->SetItemText( hItem, LPCTSTR( pTreeCtrl->BuildNodeString( CString( pNode->m_Name.c_str() ), pNode->m_Size ) ) );
		}

		// Remove the deleted item from the tree

		pTreeCtrl->DeleteItem( selectedItem );
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnToolsOptions()
{
	COptionsDlg optionsDlg;

	optionsDlg.m_bRescanAfterDelete		= m_bRescanAfterDelete;
	optionsDlg.m_bCopyToRecycleBin		= m_bCopyToRecycleBin;
	optionsDlg.m_bConfirmFolderDeletion	= m_bConfirmFolderDeletion;

	switch ( optionsDlg.DoModal() )
	{
	case IDOK:
		m_bRescanAfterDelete		= ( optionsDlg.m_bRescanAfterDelete != FALSE );
		m_bCopyToRecycleBin			= ( optionsDlg.m_bCopyToRecycleBin != FALSE );
		m_bConfirmFolderDeletion	= ( optionsDlg.m_bConfirmFolderDeletion != FALSE );
		break;

	default:
		break;
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CSnoutApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
