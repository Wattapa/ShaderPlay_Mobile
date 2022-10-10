#ifndef OPENGLQUAD_H
#define OPENGLQUAD_H

#include "openglwindow.h"

class QOpenGLShaderProgram;
class QElapsedTimer;

class OpenGLQuad : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

    QString vertexShaderSource;
    QString fragmentShaderSource;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
    GLint m_resAttr;
    GLint m_timeAttr = 0;

    float m_time = 0.0f;
    QElapsedTimer* m_timer = nullptr;

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};
#endif // OPENGLQUAD_H
