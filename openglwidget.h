#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL/QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>

class OpenGLWidget :  public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget * parent = 0);
    ~OpenGLWidget();

    void MgrInit() {initializeGL();}

    class ShaderConstructor* m_shaderConstructor;

public slots:
    void resetTimerFrame(int _ms);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    bool event(QEvent *e) override;

    bool isPaintingAllowed();

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
    GLint m_resAttr = 0;
    GLint m_timeAttr = 0;
    GLint m_batteryAttr = 0;
    GLint m_daytimeAttr = 0;
    GLint m_dateAttr = 0;
    GLint m_pointersAttr = 0;
    GLint m_pointersCountAttr = 0;
    GLuint m_backBufferAttr = 0;
    GLuint m_cameraBackAttr = 0;
    GLuint m_cameraFrontAttr = 0;

    float m_time = 0.0f;
    class QElapsedTimer* m_timer = nullptr;

    QOpenGLShaderProgram *m_program = nullptr;

    int m_width, m_height;
    QTimer* m_timerFrame;
    QVector2D pointers[10];
    int pointersCount = 0;
};

#endif // OPENGLWIDGET_H
