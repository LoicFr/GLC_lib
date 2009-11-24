/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 Version 1.2.0, packaged on September 2009.

 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*****************************************************************************/

//! \file glc_extendedgeomengine.cpp Implementation for the GLC_MeshData class.

#include "glc_meshdata.h"
#include "../glc_state.h"

// Default constructor
GLC_MeshData::GLC_MeshData()
: m_VboId(0)
, m_Positions()
, m_Normals()
, m_Texels()
, m_Colors()
, m_NormalVboId(0)
, m_TexelVboId(0)
, m_ColorVboId(0)
, m_EngineLodList()
, m_PositionSize(0)
, m_TexelsSize(0)
, m_ColorSize(0)
{
	//m_EngineLodList.append(new GLC_EngineLod());

}

// Copy constructor
GLC_MeshData::GLC_MeshData(const GLC_MeshData& engine)
: m_VboId(0)
, m_Positions(engine.positionVector())
, m_Normals(engine.normalVector())
, m_Texels(engine.texelVector())
, m_Colors(engine.colorVector())
, m_NormalVboId(0)
, m_TexelVboId(0)
, m_ColorVboId(0)
, m_EngineLodList()
, m_PositionSize(engine.m_PositionSize)
, m_TexelsSize(engine.m_TexelsSize)
, m_ColorSize(engine.m_ColorSize)
{

	const int size= engine.m_EngineLodList.size();
	for (int i= 0; i < size; ++i)
	{
		m_EngineLodList.append(new GLC_Lod(*engine.m_EngineLodList.at(i)));
	}

}

GLC_MeshData::~GLC_MeshData()
{
	// Delete Main Vbo ID
	if (0 != m_VboId)
	{
		glDeleteBuffers(1, &m_VboId);
		m_VboId= 0;
	}

	// Delete Normal VBO
	if (0 != m_NormalVboId)
	{
		glDeleteBuffers(1, &m_NormalVboId);
	}

	// Delete Texel VBO
	if (0 != m_TexelVboId)
	{
		glDeleteBuffers(1, &m_TexelVboId);
	}

	// Delete Color VBO
	if (0 != m_ColorVboId)
	{
		glDeleteBuffers(1, &m_ColorVboId);
	}

	const int size= m_EngineLodList.size();
	for (int i= 0; i < size; ++i)
	{
		delete m_EngineLodList.at(i);
	}
}
//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Return the Position Vector
GLfloatVector GLC_MeshData::positionVector() const
{
	if (0 != m_VboId)
	{
		// VBO created get data from VBO
		const int sizeOfVbo= m_PositionSize;
		const GLsizeiptr dataSize= sizeOfVbo * sizeof(float);
		GLfloatVector positionVector(sizeOfVbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
		GLvoid* pVbo = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
		memcpy(positionVector.data(), pVbo, dataSize);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return positionVector;
	}
	else
	{
		return m_Positions;
	}

}

// Return the normal Vector
GLfloatVector GLC_MeshData::normalVector() const
{
	if (0 != m_NormalVboId)
	{
		// VBO created get data from VBO
		const int sizeOfVbo= m_PositionSize;
		const GLsizeiptr dataSize= sizeOfVbo * sizeof(GLfloat);
		GLfloatVector normalVector(sizeOfVbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_NormalVboId);
		GLvoid* pVbo = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
		memcpy(normalVector.data(), pVbo, dataSize);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return normalVector;
	}
	else
	{
		return m_Normals;
	}
}

// Return the texel Vector
GLfloatVector GLC_MeshData::texelVector() const
{
	if (0 != m_TexelVboId)
	{
		// VBO created get data from VBO
		const int sizeOfVbo= m_TexelsSize;
		const GLsizeiptr dataSize= sizeOfVbo * sizeof(GLfloat);
		GLfloatVector texelVector(sizeOfVbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_TexelVboId);
		GLvoid* pVbo = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
		memcpy(texelVector.data(), pVbo, dataSize);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return texelVector;
	}
	else
	{
		return m_Texels;
	}
}

// Return the color Vector
GLfloatVector GLC_MeshData::colorVector() const
{
	if (0 != m_ColorVboId)
	{
		// VBO created get data from VBO
		const int sizeOfVbo= m_ColorSize;
		const GLsizeiptr dataSize= sizeOfVbo * sizeof(GLfloat);
		GLfloatVector normalVector(sizeOfVbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_ColorVboId);
		GLvoid* pVbo = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
		memcpy(normalVector.data(), pVbo, dataSize);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return normalVector;
	}
	else
	{
		return m_Colors;
	}
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// The mesh wich use this engine is finished
void GLC_MeshData::finished()
{
	m_PositionSize= m_Positions.size();
	m_Positions.clear();
	m_Normals.clear();
	m_TexelsSize= m_Texels.size();
	m_Texels.clear();
	m_ColorSize= m_Colors.size();
	m_Colors.clear();

	// Finish the LOD
	const int size= m_EngineLodList.size();
	for (int i= 0; i < size; ++i)
	{
		m_EngineLodList[i]->finish();
	}
}
// If the there is more than 2 LOD Swap the first and last
void GLC_MeshData::finishedLod()
{
	// PLace the master LOD at the beginning of the list
	const int size= m_EngineLodList.size();
	if (size > 1)
	{
		GLC_Lod* PMasterLod= m_EngineLodList.at(size - 1);
		m_EngineLodList.removeAt(size - 1);
		m_EngineLodList.prepend(PMasterLod);
	}
}

// Clear the engine
void GLC_MeshData::clear()
{

	m_Positions.clear();
	m_Normals.clear();
	m_Texels.clear();
	m_PositionSize= 0;
	m_TexelsSize= 0;

	// Delete Main Vbo ID
	if (0 != m_VboId)
	{
		glDeleteBuffers(1, &m_VboId);
		m_VboId= 0;
	}

	// Delete Normal VBO
	if (0 != m_NormalVboId)
	{
		glDeleteBuffers(1, &m_NormalVboId);
		m_NormalVboId= 0;
	}

	// Delete Texel VBO
	if (0 != m_TexelVboId)
	{
		glDeleteBuffers(1, &m_TexelVboId);
		m_TexelVboId= 0;
	}

	const int size= m_EngineLodList.size();
	for (int i= 0; i < size; ++i)
	{
		delete m_EngineLodList.at(i);
	}
	m_EngineLodList.clear();
}


//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////
//! Vbo creation
void GLC_MeshData::createVBOs()
{
	// Create position VBO
	if (0 == m_VboId)
	{
		glGenBuffers(1, &m_VboId);
		glGenBuffers(1, &m_NormalVboId);

		// Create Texel VBO
		if (0 == m_TexelVboId && !m_Texels.isEmpty())
		{
			glGenBuffers(1, &m_TexelVboId);
		}

		// Create Color VBO
		if (0 == m_ColorVboId && !m_Colors.isEmpty())
		{
			glGenBuffers(1, &m_ColorVboId);
		}

		const int size= m_EngineLodList.size();
		for (int i= 0; i < size; ++i)
		{
			m_EngineLodList.at(i)->createIBO();
		}
	}
}
//! Ibo Usage
bool GLC_MeshData::useVBO(bool use, GLC_MeshData::VboType type)
{
	bool result= true;
	if (use)
	{
		// Chose the right VBO
		if (type == GLC_MeshData::GLC_Vertex)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
		}
		else if (type == GLC_MeshData::GLC_Normal)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_NormalVboId);
		}
		else if ((type == GLC_MeshData::GLC_Texel) && (0 != m_TexelVboId))
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_TexelVboId);
		}
		else if ((type == GLC_MeshData::GLC_Color) && (0 != m_ColorVboId))
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ColorVboId);
		}

		else result= false;
	}
	else
	{
		// Unbind VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return result;

}
// Non Member methods
#define GLC_BINARY_CHUNK_ID 0xA704

// Non-member stream operator
QDataStream &operator<<(QDataStream &stream, const GLC_MeshData &engine)
{
	quint32 chunckId= GLC_BINARY_CHUNK_ID;
	stream << chunckId;

	stream << engine.positionVector();
	stream << engine.normalVector();
	stream << engine.texelVector();
	stream << engine.colorVector();

	// List of lod serialisation
	const int lodCount= engine.m_EngineLodList.size();
	QList<GLC_Lod> lodsList;
	for (int i= 0; i < lodCount; ++i)
	{
		lodsList.append(*(engine.m_EngineLodList[i]));
	}
	stream << lodsList;

	return stream;
}
QDataStream &operator>>(QDataStream &stream, GLC_MeshData &engine)
{
	quint32 chunckId;
	stream >> chunckId;
	Q_ASSERT(chunckId == GLC_BINARY_CHUNK_ID);

	engine.clear();

	GLfloatVector position, normal, texel, color;

	stream >> position;
	stream >> normal;
	stream >> texel;
	stream >> color;

	*(engine.positionVectorHandle())= position;
	*(engine.normalVectorHandle())= normal;
	*(engine.texelVectorHandle())= texel;
	*(engine.colorVectorHandle())= color;

	// List of lod serialisation
	QList<GLC_Lod> lodsList;
	stream >> lodsList;
	const int lodCount= lodsList.size();
	for (int i= 0; i < lodCount; ++i)
	{
		engine.m_EngineLodList.append(new GLC_Lod(lodsList.at(i)));
	}

	return stream;
}
