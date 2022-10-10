#include "openglwindow.h"
#include <QPainter>
#include <QOpenGLPaintDevice>
#include <QFile>
#include <QDir>

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow{parent}
{
    setSurfaceType(QWindow::OpenGLSurface);
}


OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::initialize()
{
}

void OpenGLWindow::render(QPainter *_painter)
{
    Q_UNUSED(_painter);
}

void OpenGLWindow::render()
{
    if(!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    QPainter painter(m_device);
    render(&painter);
}

void OpenGLWindow::setAnimating(bool _isAnimated)
{
    m_animating = _isAnimated;

    if(m_animating)
        renderLater();
}

void OpenGLWindow::renderLater()
{
    requestUpdate();
}

void OpenGLWindow::renderNow()
{
    if(!isExposed())
        return;

    bool needsInitialize = false;

    if(!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if(needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if(m_animating)
        renderLater();
}


bool OpenGLWindow::event(QEvent *_event)
{
    switch(_event->type())
    {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(_event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *_event)
{
    Q_UNUSED(_event);

    if(isExposed())
        renderNow();
}

const QString OpenGLWindow::OpenFile(const char* _path)
{
    QFile file(QDir::currentPath() + _path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open file " <<QDir::currentPath() + _path;
    }
    //else
    //{
    //    qDebug() << "File open successfully";
    //}

    QTextStream stream(&file);
    QString r = stream.readAll();
    return r;
}
