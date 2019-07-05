// DirectoryTreeCtrl.cpp : implementation file
//

#include "stdafx.h"

#include "DirectoryTreeCtrl.h"

#include "Misc/SafeStr.h"
#include "FileTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirectoryTreeCtrl

CDirectoryTreeCtrl::CDirectoryTreeCtrl()
{
}

CDirectoryTreeCtrl::~CDirectoryTreeCtrl()
{
}


BEGIN_MESSAGE_MAP( CDirectoryTreeCtrl, CTreeCtrl )
	//{{AFX_MSG_MAP( CDirectoryTreeCtrl )
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

BOOL CDirectoryTreeCtrl::Create( const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	m_DisplayMode	= DM_FOLDERS;


	return CTreeCtrl::Create( WS_VISIBLE|TVS_DISABLEDRAGDROP|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS,
							  rect, pParentWnd, nID );
}



/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CDirectoryTreeCtrl::Rebuild( FileTree::Node const * pRoot, DisplayMode mode )
{
	CWaitCursor wait_cursor;

	// Set the display mode (except DM_NONE, which means don't change it)

	if ( mode != DM_NONE )
	{
		m_DisplayMode = mode;
	}

	// Clear the tree control

	DeleteAllItems();

	// Rebuild it

	if ( pRoot != 0 )
	{
		InsertFileNode( pRoot, TVI_ROOT );
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void CDirectoryTreeCtrl::InsertFileNode( FileTree::Node const * pNode, HTREEITEM hParent )
{
	// Warning: This function is recursive

	// Create the displayed text for this node

	char node_text[ _MAX_PATH ];
	SafeStrcpy( node_text,
				LPCTSTR( BuildNodeString( CString( pNode->m_Name.c_str() ), pNode->m_Size ) ),
				sizeof node_text );

	// Insert this node into the tree control as a new item

	TVINSERTSTRUCT tvinsertstruct;
	memset( &tvinsertstruct, 0, sizeof tvinsertstruct );
	tvinsertstruct.hParent = hParent;
	tvinsertstruct.hInsertAfter = TVI_LAST;
	tvinsertstruct.item.pszText = node_text;
	tvinsertstruct.item.state = 0;
	tvinsertstruct.item.stateMask = TVIS_BOLD;
	tvinsertstruct.item.lParam = reinterpret_cast< LPARAM >( pNode );
	tvinsertstruct.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_PARAM;

	HTREEITEM const	this_item	= InsertItem( &tvinsertstruct );

	// Go through all the nodes contained by this node and insert them into the tree. Which nodes get inserted
	// depends on the display mode.

	ASSERT( m_DisplayMode != DM_NONE );

	switch ( m_DisplayMode )
	{
	case DM_FOLDERS:		// Folders only
	{
		// Recursively insert each subnode (and its children, if any) if it is a folder

		for ( FileTree::Node::NodeVector::const_iterator i = pNode->m_Nodes.begin(); i != pNode->m_Nodes.end(); ++i )
		{
			FileTree::Node const * const	pSubNode	= *i;

			if ( ( pSubNode->m_Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
			{
				InsertFileNode( pSubNode, this_item );
			}
		}
		break;
	}

	case DM_FILES:			// Folders and files
	{
		// Recursively insert each subnode (and its children, if any)

		for ( FileTree::Node::NodeVector::const_iterator i = pNode->m_Nodes.begin(); i != pNode->m_Nodes.end(); ++i )
		{
			InsertFileNode( *i, this_item );
		}
		break;
	}

	case DM_10:				// Files and nodes that are 10% or more of the contents of the containing node
	{
		__int64 const	threshold	= ( pNode->m_Size + 5i64 ) / 10i64;

		// Recursively insert each subnode (and its children, if any) if its size is >= 10%

		for ( FileTree::Node::NodeVector::const_iterator i = pNode->m_Nodes.begin(); i != pNode->m_Nodes.end(); ++i )
		{
			FileTree::Node const * const	pSubNode	= *i;

			// If the node is less than 10%, then we are done. This assumes that the nodes are sorted by
			// descending size.

			if ( pSubNode->m_Size < threshold )
			{
				break;
			}

			InsertFileNode( pSubNode, this_item );
		}
		break;
	}

	case DM_90:				// All files and nodes except the smallest 10% of the contents of the containing node
	{
		__int64 const	threshold		= ( pNode->m_Size + 9i64 ) / 10i64 * 9i64;
		__int64			accumulatedSize	= 0;

		// Recursively insert each subnode (and its children, if any) if < 90% of the contents have been inserted

		for ( FileTree::Node::NodeVector::const_iterator i = pNode->m_Nodes.begin(); i != pNode->m_Nodes.end(); ++i )
		{
			FileTree::Node const * const	pSubNode	= *i;

			// If the accumulated size of the subnodes >= 90%, then we are done. The nodes should be sorted by
			// descending size.

			if ( accumulatedSize >= threshold )
			{
				break;
			}

			InsertFileNode( pSubNode, this_item );
			accumulatedSize += pSubNode->m_Size;
		}
		break;
	}

	default:
		break;
	}
}
//
//
//
///************************************************************************/
///*																		*/
///*																		*/
///************************************************************************/
//
//void CDirectoryTreeCtrl::InsertFileItem( CString const & name, HTREEITEM hParent, __int64 const & size )
//{
//	char node_text[ _MAX_PATH ];
//	SafeStrcpy( node_text, LPCTSTR( BuildNodeString( name, size ) ), sizeof node_text );
//
//	TVINSERTSTRUCT tvinsertstruct;
//	memset( &tvinsertstruct, 0, sizeof tvinsertstruct );
//	tvinsertstruct.hParent = hParent;
//	tvinsertstruct.hInsertAfter = TVI_LAST;
//	tvinsertstruct.item.pszText = node_text;
//	tvinsertstruct.item.state = 0;
//	tvinsertstruct.item.stateMask = TVIS_BOLD;
//	*reinterpret_cast< float * >( &tvinsertstruct.item.lParam ) = float( size );
//	tvinsertstruct.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_PARAM;
//
//	InsertItem( &tvinsertstruct );
//}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

CString CDirectoryTreeCtrl::BuildNodeString( CString const & name, __int64 const & size, NodeStringFormatOption option )
{
	ASSERT( size >= 0 );

	int const		JUSTIFIED_LENGTH_OF_NUMBER	= 6;	// Length of the number text when justified

	char const *	format;
	double			value;
	CString			numberString;
	CString			nodeString;


	// Adjust the value and the units to get a string like nnn.nM

	if ( size < 1000i64 )
	{
		format = "%.0f";
		value = double( size );
	}
	else if ( size < 1000000i64 )
	{
		format = "%.1fk";
		value = size / 1e3;
	}
	else if ( size < 1000000000i64 )
	{
		format = "%.1fM";
		value = size / 1e6;
	}
	else if ( size < 1000000000000i64 )
	{
		format = "%.1fG";
		value = size / 1e9;
	}
	else if ( size < 1000000000000000i64 )
	{
		format = "%.1fT";
		value = size / 1e12;
	}
	else if ( size < 1000000000000000000i64 )
	{
		format = "%.1fP";
		value = size / 1e15;
	}
	else // if ( size < 1000000000000000000000i64 )
	{
		format = "%.1fE";
		value = size / 1e18;
	}

	ASSERT( value < 1000. );

	numberString.Format( format, value );

	ASSERT( numberString.GetLength() <= JUSTIFIED_LENGTH_OF_NUMBER );

	switch ( option )
	{
	case NSFO_NORMAL:
		break;

	case NSFO_RIGHT_JUSTIFY_SIZE_WITH_DASHES:
		nodeString = CString( '-', JUSTIFIED_LENGTH_OF_NUMBER - numberString.GetLength() );
		break;

	case NSFO_RIGHT_JUSTIFY_SIZE_WITH_SPACES:
		nodeString = CString( ' ', JUSTIFIED_LENGTH_OF_NUMBER - numberString.GetLength() );
		break;

	default:
		ASSERT( FALSE );
		break;
	}

	// Append the size

	nodeString += numberString;

	// Append the name

	nodeString += "  ";
	nodeString += name;

	return nodeString;
}



/////////////////////////////////////////////////////////////////////////////
// CDirectoryTreeCtrl message handlers
