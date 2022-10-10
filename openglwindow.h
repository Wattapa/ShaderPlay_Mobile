#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>

class  QOpenGLPaintDevice;

class OpenGLWindow : public QWindow, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWindow(QWindow *_parent = nullptr);
    ~OpenGLWindow();

    virtual void initialize();

    virtual void render(QPainter *_painter);
    virtual void render();

    void setAnimating(bool _isAnimated);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *_event) override;
    void exposeEvent(QExposeEvent *_event) override;

    const QString OpenFile(const char* _path);

    bool m_animating = false;

private:
    QOpenGLContext *m_context = nullptr;
    QOpenGLPaintDevice *m_device = nullptr;

signals:

};

#endif // OPENGLWINDOW_H
