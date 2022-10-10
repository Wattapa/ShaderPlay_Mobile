#include "openglquad.h"

#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QApplication>
#include <QElapsedTimer>

static const char *vertexShaderPath =  "/Shaders/test.vert";

static const char *fragmentShaderPath = "/Shaders/test.frag";

void OpenGLQuad::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_timer = new QElapsedTimer();
    m_timer->start();

    vertexShaderSource = OpenFile(vertexShaderPath);
    fragmentShaderSource = OpenFile(fragmentShaderPath);

    //Debug: Show vertex and Fragment Shader Source
    {
        //qDebug() << vertexShaderSource;
        //qDebug() << fragmentShaderSource;
    }

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_resAttr = m_program->uniformLocation("resolution");
    Q_ASSERT(m_resAttr != -1);
    m_timeAttr = m_program->uniformLocation("time");
    Q_ASSERT(m_timeAttr != -1);

    //Debug: Show Qt version and GLSL version
    {
    //qDebug() << "version: "
    //             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    //qDebug() << "GSLS version: "
    //             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    }
}

void OpenGLQuad::render()
{
    const GLfloat vertices[] = { //Square
        -1.0f,  1.0f,
        -1.0, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f, -1.0f,
         1.0f, 1.0f
    };

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QVector2D tmp = QVector2D(QApplication::primaryScreen()->geometry().height(),QApplication::primaryScreen()->geometry().width());
    m_program->setUniformValue(m_resAttr, tmp);

    m_time += (m_timer->restart() / 1000.0f);

    m_program->setUniformValue(m_timeAttr, m_time);

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    glEnableVertexAttribArray(m_posAttr);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(m_posAttr);

    m_program->release();

    if(m_animating) ++m_frame;
}

