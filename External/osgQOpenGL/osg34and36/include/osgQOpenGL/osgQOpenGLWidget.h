#pragma once

#ifdef __APPLE__
#   define __glext_h_
#   include <QtGui/qopengl.h>
#   undef __glext_h_
#   include <QtGui/qopenglext.h>
#endif

#include <osgQOpenGL/Export>
#include <OpenThreads/ReadWriteMutex>

#ifdef WIN32
//#define __gl_h_
#include <osg/GL>
#endif

#include <osg/ArgumentParser>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QReadWriteLock>

class OSGRenderer;

namespace osgViewer
{
    class Viewer;
}

class OSGQOPENGL_EXPORT osgQOpenGLWidget : public QOpenGLWidget,
    protected QOpenGLFunctions
{
    Q_OBJECT

protected:
    OSGRenderer* m_renderer {nullptr};
    bool _osgWantsToRenderFrame{true};
    OpenThreads::ReadWriteMutex _osgMutex;
    osg::ArgumentParser* _arguments {nullptr};
	bool _isFirstFrame {true};

    friend class OSGRenderer;

public:
    osgQOpenGLWidget(QWidget* parent = nullptr);
    osgQOpenGLWidget(osg::ArgumentParser* arguments, QWidget* parent = nullptr);
    virtual ~osgQOpenGLWidget();

    /** Get osgViewer View */
    virtual osgViewer::Viewer* getOsgViewer();

    //! get mutex
    virtual OpenThreads::ReadWriteMutex* mutex();
    
    //! override this to change default size or aspect ratio 
    QSize sizeHint() const override { return QSize(640,480); }
    
signals:
    void initialized();

protected:

    //! call createRender. If overloaded, this method must send initialized signal at end
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    //! lock scene graph and call osgViewer::frame()
    void paintGL() override;

    //! called before creating renderer
    virtual void setDefaultDisplaySettings();

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void createRenderer();

private:
};

#ifndef Q_MOC_RUN
#include "osgQOpenGLWidget.inl"
#endif // Q_MOC_RUN
