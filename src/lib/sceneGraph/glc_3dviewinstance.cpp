/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*****************************************************************************/

//! \file glc_instance.cpp implementation of the GLC_3DViewInstance class.

#include "glc_3dviewinstance.h"
#include "../shading/glc_selectionmaterial.h"
#include "../viewport/glc_viewport.h"
#include "../glc_state.h"
#include "../glc_renderstate.h"

//! The global default LOD
int GLC_3DViewInstance::m_GlobalDefaultLOD= 10;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLC_3DViewInstance::GLC_3DViewInstance()
    : GLC_Object()
    , m_3DRep()
    , m_pBoundingBox(nullptr)
    , m_AbsoluteMatrix()
    , m_IsBoundingBoxValid(false)
    , m_RenderProperties()
    , m_IsVisible(true)
    , m_DefaultLOD(m_GlobalDefaultLOD)
    , m_ViewableFlag(GLC_3DViewInstance::FullViewable)
    , m_ViewableGeomFlag()
    , m_pRenderState(new GLC_RenderState)
    , m_OrderWeight(0)
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

}

GLC_3DViewInstance::GLC_3DViewInstance(GLC_Geometry* pGeom)
    : GLC_Object()
    , m_3DRep(pGeom)
    , m_pBoundingBox(nullptr)
    , m_AbsoluteMatrix()
    , m_IsBoundingBoxValid(false)
    , m_RenderProperties()
    , m_IsVisible(true)
    , m_DefaultLOD(m_GlobalDefaultLOD)
    , m_ViewableFlag(GLC_3DViewInstance::FullViewable)
    , m_ViewableGeomFlag()
    , m_pRenderState(new GLC_RenderState)
    , m_OrderWeight(0)
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

	setName(m_3DRep.name());
}

GLC_3DViewInstance::GLC_3DViewInstance(GLC_Geometry* pGeom, GLC_uint id)
    : GLC_Object(id)
    , m_3DRep(pGeom)
    , m_pBoundingBox(nullptr)
    , m_AbsoluteMatrix()
    , m_IsBoundingBoxValid(false)
    , m_RenderProperties()
    , m_IsVisible(true)
    , m_DefaultLOD(m_GlobalDefaultLOD)
    , m_ViewableFlag(GLC_3DViewInstance::FullViewable)
    , m_ViewableGeomFlag()
    , m_pRenderState(new GLC_RenderState)
    , m_OrderWeight(0)
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

	setName(m_3DRep.name());
}

GLC_3DViewInstance::GLC_3DViewInstance(const GLC_3DRep& rep)
    : GLC_Object(rep.name())
    , m_3DRep(rep)
    , m_pBoundingBox(nullptr)
    , m_AbsoluteMatrix()
    , m_IsBoundingBoxValid(false)
    , m_RenderProperties()
    , m_IsVisible(true)
    , m_DefaultLOD(m_GlobalDefaultLOD)
    , m_ViewableFlag(GLC_3DViewInstance::FullViewable)
    , m_ViewableGeomFlag()
    , m_pRenderState(new GLC_RenderState)
    , m_OrderWeight(0)
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

}

GLC_3DViewInstance::GLC_3DViewInstance(const GLC_3DRep& rep, GLC_uint id)
    : GLC_Object(id, rep.name())
    , m_3DRep(rep)
    , m_pBoundingBox(nullptr)
    , m_AbsoluteMatrix()
    , m_IsBoundingBoxValid(false)
    , m_RenderProperties()
    , m_IsVisible(true)
    , m_DefaultLOD(m_GlobalDefaultLOD)
    , m_ViewableFlag(GLC_3DViewInstance::FullViewable)
    , m_ViewableGeomFlag()
    , m_pRenderState(new GLC_RenderState)
    , m_OrderWeight(0)
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

}

// Copy constructor
GLC_3DViewInstance::GLC_3DViewInstance(const GLC_3DViewInstance& inputNode)
    : GLC_Object(inputNode)
    , m_3DRep(inputNode.m_3DRep)
    , m_pBoundingBox(nullptr)
    , m_AbsoluteMatrix(inputNode.m_AbsoluteMatrix)
    , m_IsBoundingBoxValid(inputNode.m_IsBoundingBoxValid)
    , m_RenderProperties(inputNode.m_RenderProperties)
    , m_IsVisible(inputNode.m_IsVisible)
    , m_DefaultLOD(inputNode.m_DefaultLOD)
    , m_ViewableFlag(inputNode.m_ViewableFlag)
    , m_ViewableGeomFlag(inputNode.m_ViewableGeomFlag)
    , m_pRenderState(inputNode.m_pRenderState->clone())
    , m_OrderWeight(inputNode.m_OrderWeight)

{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

    if (nullptr != inputNode.m_pBoundingBox)
	{
		m_pBoundingBox= new GLC_BoundingBox(*inputNode.m_pBoundingBox);
	}
}


// Assignement operator
GLC_3DViewInstance& GLC_3DViewInstance::operator=(const GLC_3DViewInstance& inputNode)
{
	if (this != &inputNode)
	{
		// Clear this instance
		clear();
		GLC_Object::operator=(inputNode);
		// Encode Color Id
		glc::encodeRgbId(m_Uid, m_colorId);

		m_3DRep= inputNode.m_3DRep;
        if (nullptr != inputNode.m_pBoundingBox)
		{
			m_pBoundingBox= new GLC_BoundingBox(*inputNode.m_pBoundingBox);
		}
		m_AbsoluteMatrix= inputNode.m_AbsoluteMatrix;
		m_IsBoundingBoxValid= inputNode.m_IsBoundingBoxValid;
		m_RenderProperties= inputNode.m_RenderProperties;
		m_IsVisible= inputNode.m_IsVisible;
		m_DefaultLOD= inputNode.m_DefaultLOD;
		m_ViewableFlag= inputNode.m_ViewableFlag;
		m_ViewableGeomFlag= inputNode.m_ViewableGeomFlag;

        delete m_pRenderState;
        m_pRenderState= inputNode.m_pRenderState->clone();
        m_OrderWeight= inputNode.m_OrderWeight;

		//qDebug() << "GLC_3DViewInstance::operator= :ID = " << m_Uid;
		//qDebug() << "Number of instance" << (*m_pNumberOfInstance);
	}

	return *this;
}

// Destructor
GLC_3DViewInstance::~GLC_3DViewInstance()
{
	clear();
    delete m_pRenderState;
}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Get the bounding box
GLC_BoundingBox GLC_3DViewInstance::boundingBox(void)
{
	GLC_BoundingBox resultBox;
	if (boundingBoxValidity())
	{
		resultBox= *m_pBoundingBox;
	}
	else if (!m_3DRep.isEmpty())
	{
		computeBoundingBox();
		m_IsBoundingBoxValid= true;
		resultBox= *m_pBoundingBox;
	}

	return resultBox;
}

//! Set the global default LOD value
void GLC_3DViewInstance::setGlobalDefaultLod(int lod)
{
	m_GlobalDefaultLOD= lod;
}

void GLC_3DViewInstance::setVboUsage(bool usage)
{
	m_3DRep.setVboUsage(usage);
}

// Clone the instance
GLC_3DViewInstance GLC_3DViewInstance::deepCopy() const
{

	GLC_3DRep* pRep= dynamic_cast<GLC_3DRep*>(m_3DRep.deepCopy());
    Q_ASSERT(nullptr != pRep);
	GLC_3DRep newRep(*pRep);
	delete pRep;
	GLC_3DViewInstance cloneInstance(newRep);

    if (nullptr != m_pBoundingBox)
	{
		cloneInstance.m_pBoundingBox= new GLC_BoundingBox(*m_pBoundingBox);
	}

	cloneInstance.m_AbsoluteMatrix= m_AbsoluteMatrix;
	cloneInstance.m_IsBoundingBoxValid= m_IsBoundingBoxValid;
	cloneInstance.m_RenderProperties= m_RenderProperties;
	cloneInstance.m_IsVisible= m_IsVisible;
	cloneInstance.m_ViewableFlag= m_ViewableFlag;
	return cloneInstance;
}

// Instanciate the instance
GLC_3DViewInstance GLC_3DViewInstance::instanciate()
{
	GLC_3DViewInstance instance(*this);
	instance.m_Uid= glc::GLC_GenID();
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

	return instance;
}

int GLC_3DViewInstance::globalDefaultLod()
{
    return m_GlobalDefaultLOD;
}

bool GLC_3DViewInstance::firstIsLower(GLC_3DViewInstance* pInstance1, GLC_3DViewInstance* pInstance2)
{
    return (pInstance1->m_OrderWeight < pInstance2->m_OrderWeight);
}

void GLC_3DViewInstance::setMeshWireColorAndLineWidth(const QColor& color, GLfloat lineWidth)
{
    m_3DRep.setMeshWireColorAndLineWidth(color, lineWidth);
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////


// Set the instance Geometry
bool GLC_3DViewInstance::addGeometry(GLC_Geometry* pGeom)
{
	if (m_3DRep.contains(pGeom))
	{
		return false;
	}
	else
	{
		m_3DRep.addGeom(pGeom);
		return true;
	}
}

// Instance translation
GLC_3DViewInstance& GLC_3DViewInstance::translate(double Tx, double Ty, double Tz)
{
	multMatrix(GLC_Matrix4x4(Tx, Ty, Tz));

	return *this;
}


// Move instance with a 4x4Matrix
GLC_3DViewInstance& GLC_3DViewInstance::multMatrix(const GLC_Matrix4x4 &MultMat)
{
	m_AbsoluteMatrix= MultMat * m_AbsoluteMatrix;
	m_IsBoundingBoxValid= false;

	return *this;
}

// Replace the instance Matrix
GLC_3DViewInstance& GLC_3DViewInstance::setMatrix(const GLC_Matrix4x4 &SetMat)
{
	m_AbsoluteMatrix= SetMat;
	m_IsBoundingBoxValid= false;

	return *this;
}

// Reset the instance Matrix
GLC_3DViewInstance& GLC_3DViewInstance::resetMatrix(void)
{
	m_AbsoluteMatrix.setToIdentity();
	m_IsBoundingBoxValid= false;

	return *this;
}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

// Display the instance
void GLC_3DViewInstance::render(glc::RenderFlag renderFlag, bool useLod, GLC_Viewport* pView)
{
    if (!GLC_State::isInSelectionMode() || (GLC_State::isInSelectionMode() && m_RenderProperties.selectable()))
    {
        //qDebug() << "GLC_3DViewInstance::render render properties= " << m_RenderProperties.renderingMode();
        if (m_3DRep.isEmpty()) return;
        const int bodyCount= m_3DRep.numberOfBody();

        if (bodyCount != m_ViewableGeomFlag.size())
        {
            m_ViewableGeomFlag.fill(true, bodyCount);
        }

        m_RenderProperties.setRenderingFlag(renderFlag);

        // Save current OpenGL Matrix
        GLC_Context* pContext= GLC_ContextManager::instance()->currentContext();
        pContext->glcPushMatrix();
        OpenglVisProperties();
        m_pRenderState->modifyOpenGLState();

        // Change front face orientation if this instance absolute matrix is indirect
        if (m_AbsoluteMatrix.type() == GLC_Matrix4x4::Indirect)
        {
            glFrontFace(GL_CW);
        }
        if(GLC_State::isInSelectionMode())
        {
            glColor3ubv(m_colorId); // D'ont use Alpha component
        }



        if (useLod && (nullptr != pView))
        {
            for (int i= 0; i < bodyCount; ++i)
            {
                if (m_ViewableGeomFlag.at(i))
                {
                    const int lodValue= choseLod(m_3DRep.geomAt(i)->boundingBox(), pView, useLod);
                    if (lodValue <= 100)
                    {
                        m_3DRep.geomAt(i)->setCurrentLod(lodValue);
                        m_RenderProperties.setCurrentBodyIndex(i);
                        m_3DRep.geomAt(i)->render(m_RenderProperties);
                    }
                }
            }
        }
        else
        {
            for (int i= 0; i < bodyCount; ++i)
            {
                if (m_ViewableGeomFlag.at(i))
                {
                    int lodValue= 0;
                    if (GLC_State::isPixelCullingActivated() && (nullptr != pView))
                    {
                        lodValue= choseLod(m_3DRep.geomAt(i)->boundingBox(), pView, useLod);
                    }

                    if (lodValue <= 100)
                    {
                        m_3DRep.geomAt(i)->setCurrentLod(m_DefaultLOD);
                        m_RenderProperties.setCurrentBodyIndex(i);
                        m_3DRep.geomAt(i)->render(m_RenderProperties);
                    }
                }
            }
        }
        // Restore OpenGL Matrix
        pContext->glcPopMatrix();

        // Restore front face orientation if this instance absolute matrix is indirect
        if (m_AbsoluteMatrix.type() == GLC_Matrix4x4::Indirect)
        {
            glFrontFace(GL_CCW);
        }

        m_pRenderState->restoreOpenGLState();
    }
}

// Display the instance in Body selection mode
void GLC_3DViewInstance::renderForBodySelection()
{
	Q_ASSERT(GLC_State::isInSelectionMode());
    if (m_RenderProperties.selectable())
    {
        m_pRenderState->modifyOpenGLState();
        if (m_3DRep.isEmpty()) return;

        // Save previous rendering mode and set the rendering mode to BodySelection
        glc::RenderMode previousRenderMode= m_RenderProperties.renderingMode();
        m_RenderProperties.setRenderingMode(glc::BodySelection);

        // Save current OpenGL Matrix
        GLC_Context* pContext= GLC_ContextManager::instance()->currentContext();
        pContext->glcPushMatrix();
        OpenglVisProperties();

        GLubyte colorId[4];
        const int size= m_3DRep.numberOfBody();
        for (int i= 0; i < size; ++i)
        {
            GLC_Geometry* pGeom= m_3DRep.geomAt(i);
            glc::encodeRgbId(pGeom->id(), colorId);
            glColor3ubv(colorId);
            pGeom->setCurrentLod(m_DefaultLOD);
            m_RenderProperties.setCurrentBodyIndex(i);
            pGeom->render(m_RenderProperties);
        }

        // Restore rendering mode
        m_RenderProperties.setRenderingMode(previousRenderMode);
        // Restore OpenGL Matrix
        pContext->glcPopMatrix();
        m_pRenderState->restoreOpenGLState();
    }
}

// Display the instance in Primitive selection mode and return the body index
int GLC_3DViewInstance::renderForPrimitiveSelection(GLC_uint bodyId)
{
	Q_ASSERT(GLC_State::isInSelectionMode());
    if (m_RenderProperties.selectable())
    {
        m_pRenderState->modifyOpenGLState();
        if (m_3DRep.isEmpty()) return -1;
        // Save previous rendering mode and set the rendering mode to BodySelection
        glc::RenderMode previousRenderMode= m_RenderProperties.renderingMode();
        m_RenderProperties.setRenderingMode(glc::PrimitiveSelection);

        // Save current OpenGL Matrix
        GLC_Context* pContext= GLC_ContextManager::instance()->currentContext();
        pContext->glcPushMatrix();
        OpenglVisProperties();

        const int size= m_3DRep.numberOfBody();
        int i= 0;
        bool continu= true;
        while ((i < size) && continu)
        {
            GLC_Geometry* pGeom= m_3DRep.geomAt(i);
            if (pGeom->id() == bodyId)
            {
                pGeom->setCurrentLod(0);
                pGeom->render(m_RenderProperties);
                continu= false;
            }
            else ++i;
        }

        m_RenderProperties.setRenderingMode(previousRenderMode);

        // Restore OpenGL Matrix
        pContext->glcPopMatrix();
        m_pRenderState->restoreOpenGLState();

        return i;
    }
    else
    {
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////


// compute the instance bounding box
// m_pGeomList should be not null
void GLC_3DViewInstance::computeBoundingBox(void)
{
	if (m_3DRep.isEmpty()) return;

    if (m_pBoundingBox != nullptr)
	{
		delete m_pBoundingBox;
        m_pBoundingBox= nullptr;
	}
	m_pBoundingBox= new GLC_BoundingBox();
	const int size= m_3DRep.numberOfBody();
	for (int i= 0; i < size; ++i)
	{
		m_pBoundingBox->combine(m_3DRep.geomAt(i)->boundingBox());
	}

	m_pBoundingBox->transform(m_AbsoluteMatrix);
}

// Clear current instance
void GLC_3DViewInstance::clear()
{

	delete m_pBoundingBox;
    m_pBoundingBox= nullptr;

	// invalidate the bounding box
	m_IsBoundingBoxValid= false;

    delete m_pRenderState;
    m_pRenderState= new GLC_RenderState;

}

// Compute LOD
int GLC_3DViewInstance::choseLod(const GLC_BoundingBox& boundingBox, GLC_Viewport* pView, bool useLod)
{
    if (nullptr == pView) return 0;
	double pixelCullingRatio= 0.0;
	if (useLod)
	{
		pixelCullingRatio= pView->minimumDynamicPixelCullingRatio();
	}
	else
	{
		pixelCullingRatio= pView->minimumStaticPixelCullingRatio();
	}

    double scaling= qMax(m_AbsoluteMatrix.scalingX(), m_AbsoluteMatrix.scalingY());
    scaling= qMax(scaling, m_AbsoluteMatrix.scalingZ());
    const double diameter= boundingBox.boundingSphereRadius() * 2.0 * scaling;
	GLC_Vector3d center(m_AbsoluteMatrix * boundingBox.center());

	const double dist= (center - pView->cameraHandle()->eye()).length();
	const double cameraCover= dist * pView->viewTangent();

	double ratio= diameter / cameraCover * 100.0;
	if (ratio > 100.0) ratio= 100.0;
	ratio= 100.0 - ratio;

	if ((ratio > (100.0 - pixelCullingRatio)) && GLC_State::isPixelCullingActivated()) ratio= 110.0;
	else if(useLod && (ratio > 50.0))
	{
		ratio= (ratio - 50.0) / 50.0 * 100.0;
		if (ratio < static_cast<double>(m_DefaultLOD)) ratio= static_cast<double>(m_DefaultLOD);
	}
	else
	{
		ratio= static_cast<double>(m_DefaultLOD);
	}

	return static_cast<int>(ratio);
}


