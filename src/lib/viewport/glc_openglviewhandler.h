/*
 *  glc_openglviewhandler.h
 *
 *  Created on: 07/02/2016
 *      Author: Laurent Ribon
 */
#ifndef GLC_OPENGLVIEWHANDLER_H
#define GLC_OPENGLVIEWHANDLER_H


#include "glc_viewhandler.h"

#include "../glc_config.h"

class GLC_OpenGLViewInterface;

class GLC_LIB_EXPORT GLC_OpenGLViewHandler : public GLC_ViewHandler
{
    Q_OBJECT

public:
    GLC_OpenGLViewHandler();
    ~GLC_OpenGLViewHandler() override;

public:
    void updateGL(bool synchrone) override;
    void setOpenGLViewWidget(GLC_OpenGLViewInterface* pViewInterface);
    void updateSelectionBufferOnRender(bool update) override;
    void updateViewBufferOnRender(bool update) override;

public:
    QPair<GLC_SelectionSet, GLC_Point3d> selectAndUnproject(int x, int y, GLC_SelectionEvent::Modes modes) override;
    virtual QImage takeScreenshot(const GLC_ScreenShotSettings& screenShotSettings);

private:
    GLC_OpenGLViewInterface* m_pViewInterface;
};

Q_DECLARE_METATYPE(GLC_OpenGLViewHandler*)

#endif // GLC_OPENGLVIEWHANDLER_H
