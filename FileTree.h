#pragma once

/*********************************************************************************************************************

                                                      FileTree.h

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Snout/FileTree.h#2 $

	$NoKeywords: $

*********************************************************************************************************************/

#pragma once

#include <vector>
#include "Misc/PathName.h"

namespace FileTree
{


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

class Node
{
public:

	explicit Node( PathName const & name		= PathName(),
				   PathName const & path		= PathName(),
				   unsigned __int32 attributes	= 0,
				   __int64 size					= 0 );
	virtual ~Node();

	typedef std::vector< Node * >	NodeVector;

	PathName			m_Name;			// Name of this node (Note: PathNames are case-insensitive)
	PathName			m_Path;			// Full path to this node (Note: PathNames are case-insensitive)
	unsigned __int32	m_Attributes;	// Attribute flags
	__int64				m_Size;			// Size of this node (including the nodes it contains, if any)
	NodeVector			m_Nodes;		// Contained nodes
	Node *				m_pParent;		// Parent node

	class SorterByName
	{
	public:
		bool operator ()( Node const * const & pA, Node const * const & pB ) const
		{
			// Return true if the name of *pA is lexicographically less than the name of *pB.

			return ( pA->m_Name < pB->m_Name );

		}
	};

	class SorterByDescendingSize
	{
	public:
		bool operator ()( Node const * const & pA, Node const * const & pB ) const
		{
			// Return true if the size of *pA is greater than the size of *pB, or if they are equal and the name of *pA is
			// lexicographically less than the name of *pB.

			return (   ( pA->m_Size > pB->m_Size )
					|| ( pA->m_Size == pB->m_Size && Node::SorterByName()( pA, pB ) ) );
		}
	};
};


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/


} // namespace FileTree
