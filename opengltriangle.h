#ifndef OPENGLTRIANGLE_H
#define OPENGLTRIANGLE_H

#include "openglwindow.h"
#include <QVector2D>

class QOpenGLShaderProgram;

class OpenGLTriangle : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
    GLint m_resolution;

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};
#endif // OPENGLTRIANGLE_H
