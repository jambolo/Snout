/*********************************************************************************************************************

                                                     FileTree.cpp

						                    Copyright 2002, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Snout/FileTree.cpp#2 $

	$NoKeywords: $

*********************************************************************************************************************/



#include "stdafx.h"

#include "FileTree.h"

#include "Misc/PathName.h"

#include <vector>
#include <string>
#include <new>

namespace FileTree
{


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

Node::Node( PathName const & name	/*= PathName()*/,
			PathName const & path	/* = PathName()*/,
			unsigned attributes		/* = 0*/,
			__int64 size			/* = 0*/ )
	: m_Name( name ), m_Path( path ), m_Attributes( attributes ) , m_Size( size )
{
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

Node::~Node()
{
	// Delete all sub nodes

	for ( NodeVector::iterator i = m_Nodes.begin(); i != m_Nodes.end(); ++i )
	{
		delete *i;
	}
}


} // namespace FileTree
