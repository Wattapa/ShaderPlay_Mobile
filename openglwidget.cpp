#include "openglwidget.h"
#include "shaderconstructor.h"

#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QApplication>
#include <QElapsedTimer>
#include <QFile>
#include <QDir>
#include <QString>
#include <QWindow>
#if (defined (_WIN32) || defined (_WIN64))
#include <gl/GL.h>
#include <gl/GLU.h>
#endif

#include <QTouchEvent>
#include <QMessageBox>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    m_shaderConstructor = new ShaderConstructor();

    m_timerFrame = new QTimer(this);
    connect(m_timerFrame, SIGNAL(timeout()), this, SLOT(update()));
    m_timerFrame->start(8);
    setAttribute(Qt::WA_AcceptTouchEvents, true);
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    delete m_timer;
    delete m_program;
    doneCurrent();
}

void OpenGLWidget::resetTimerFrame(int _ms)
{
    m_timerFrame->start(_ms);
}

void OpenGLWidget::initializeGL()
{
    //Debug: Show Qt version and GLSL version
    {
    //qDebug() << "version: "
    //             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    //qDebug() << "GSLS version: "
    //             << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    }

    //Debug: Show vertex and Fragment Shader Source
    {
        //qDebug() << vertexShaderSource;
        //qDebug() << fragmentShaderSource;
    }

    m_program = new QOpenGLShaderProgram(this);
    m_timer = new QElapsedTimer();
    m_timer->start();
    m_time = 0.0f;

    if(!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_shaderConstructor->m_vertexShaderSource)) {QMessageBox box; box.setText(m_program->log());}
    if(!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_shaderConstructor->m_fragmentShaderSource)) {QMessageBox box; box.setText(m_program->log());}
    m_program->link();

    if(m_shaderConstructor->m_vertexShaderSource != nullptr && m_shaderConstructor->m_fragmentShaderSource != nullptr)
    {
        initializeOpenGLFunctions();

        m_posAttr = m_program->attributeLocation("posAttr");
        //Q_ASSERT(m_posAttr != -1);

        if(m_shaderConstructor->m_is3D)
        {
            m_colAttr = m_program->attributeLocation("colAttr");
            //Q_ASSERT(m_colAttr != -1);
            m_matrixUniform = m_program->uniformLocation("matrix");
            //Q_ASSERT(m_matrixUniform != -1);
        }

        if(m_shaderConstructor->m_resolutionUniform)
        {
            m_resAttr = m_program->uniformLocation("resolution");
            //Q_ASSERT(m_resAttr != -1);
        }
        if(m_shaderConstructor->m_timeUniform)
        {
            m_timeAttr = m_program->uniformLocation("time");
            //Q_ASSERT(m_timeAttr != -1);
        }
        if(m_shaderConstructor->m_batteryUniform)
        {
            m_batteryAttr = m_program->uniformLocation("battery");
            //Q_ASSERT(m_batteryAttr != -1);
        }
        if(m_shaderConstructor->m_daytimeUniform)
        {
            m_daytimeAttr = m_program->uniformLocation("daytime");
            //Q_ASSERT(m_daytimeAttr != -1);
        }
        if(m_shaderConstructor->m_dateUniform)
        {
            m_dateAttr = m_program->uniformLocation("date");
            //Q_ASSERT(m_dateAttr != -1);
        }
        if(m_shaderConstructor->m_pointersUniform)
        {
            m_pointersAttr = m_program->uniformLocation("pointers");
            //Q_ASSERT(m_pointersAttr != -1);
        }
        if(m_shaderConstructor->m_pointersCountUniform)
        {
            m_pointersCountAttr = m_program->uniformLocation("pointersCount");
            //Q_ASSERT(m_pointersCountAttr != -1);
        }
        if(m_shaderConstructor->m_backBufferUniform)
        {
            m_backBufferAttr = m_program->uniformLocation("backBuffer");
            //Q_ASSERT(m_backBufferAttr != -1);
        }
        if(m_shaderConstructor->m_cameraBackUniform)
        {
            m_cameraBackAttr = m_program->uniformLocation("cameraBack");
            //Q_ASSERT(m_cameraBackAttr != -1);
        }
        if(m_shaderConstructor->m_cameraFrontUniform)
        {
            m_cameraFrontAttr = m_program->uniformLocation("cameraFront");
            //Q_ASSERT(m_cameraFrontAttr != -1);
        }
    }
}

void OpenGLWidget::paintGL()
{
    if(!isPaintingAllowed()) {qDebug() << "I'm not painting" << m_shaderConstructor->m_path; return;}

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, m_width * retinaScale, m_height * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    m_time += (m_timer->restart() / 1000.0f);

    if(m_shaderConstructor->m_is3D)
    {
         QMatrix4x4 matrix;
         matrix.perspective(m_shaderConstructor->m_matrixPerspective[0],
                            m_shaderConstructor->m_matrixPerspective[1],
                            m_shaderConstructor->m_matrixPerspective[2],
                            m_shaderConstructor->m_matrixPerspective[3]);
         matrix.translate(m_shaderConstructor->m_matrixTranslation[0],
                          m_shaderConstructor->m_matrixTranslation[1],
                          m_shaderConstructor->m_matrixTranslation[2]);
         matrix.rotate(m_shaderConstructor->m_matrixRotation[0]* m_time / screen()->refreshRate(),
                       m_shaderConstructor->m_matrixRotation[1],
                       m_shaderConstructor->m_matrixRotation[2]);
         m_program->setUniformValue(m_matrixUniform, matrix);
    }

    if(m_shaderConstructor->m_resolutionUniform)
    {
        QVector2D tmp = QVector2D(QApplication::primaryScreen()->geometry().width() * retinaScale, QApplication::primaryScreen()->geometry().height() * retinaScale);
        m_program->setUniformValue(m_resAttr, tmp);
    }

    if(m_shaderConstructor->m_timeUniform)
    {
        m_program->setUniformValue(m_timeAttr, m_time);
    }

    if(m_shaderConstructor->m_batteryUniform) //ToDo: find a way to get remaining battery on android
    {
        //m_program->setUniformValue(m_batteryAttr, );
    }

    if(m_shaderConstructor->m_daytimeUniform)
    {
        QDateTime dayTime(QDateTime::currentDateTimeUtc().toLocalTime());
        QVector3D tmp = QVector3D(dayTime.time().hour(), dayTime.time().minute(), dayTime.time().second());
        m_program->setUniformValue(m_daytimeAttr, tmp);
    }

    if(m_shaderConstructor->m_dateUniform)
    {
        QDateTime dayTime(QDateTime::currentDateTimeUtc().toLocalTime());
        QVector4D tmp = QVector4D(dayTime.date().year(), dayTime.date().month(), dayTime.date().day(), dayTime.toMSecsSinceEpoch());
        m_program->setUniformValue(m_dateAttr, tmp);
    }

    if(m_shaderConstructor->m_pointersUniform)
    {
       m_program->setUniformValueArray(m_pointersAttr, pointers, pointersCount);
    }

    if(m_shaderConstructor->m_pointersCountUniform)
    {
        m_program->setUniformValue(m_pointersCountAttr, pointersCount);
    }

    if(m_shaderConstructor->m_backBufferUniform)
    {
        QImage tmp = grabFramebuffer();
        tmp = tmp.convertToFormat(QImage::Format_ARGB32);

        glGenTextures(1, &m_backBufferAttr);
        glBindTexture(GL_TEXTURE_2D, m_backBufferAttr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp.width(), tmp.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp.bits());
        glGenerateMipmap(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
    }

    if(m_shaderConstructor->m_cameraBackUniform)//ToDo: find a way to get back camera on android
    {
        //m_program->setUniformValue(m_cameraBackAttr, m_time);
    }

    if(m_shaderConstructor->m_cameraFrontUniform)//ToDo: find a way to get back camera on android
    {
        //m_program->setUniformValue(m_cameraFrontAttr, m_time);
    }

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, m_shaderConstructor->m_vertices);
    if(m_shaderConstructor->m_is3D)glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, m_shaderConstructor->m_colors);

    glEnableVertexAttribArray(m_posAttr);
    if(m_shaderConstructor->m_is3D) glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLES, 0, m_shaderConstructor->m_verticesCount);

    glDisableVertexAttribArray(m_posAttr);
    if(m_shaderConstructor->m_is3D) glDisableVertexAttribArray(m_colAttr);

    m_program->release();
}

void OpenGLWidget::resizeGL(int _w, int _h)
{
    m_width = _w;
    m_height = _h;
    QOpenGLWidget::resizeGL(_w,_h);
}

bool OpenGLWidget::event(QEvent *e)
{
    switch(e->type())
    {
        case QEvent::TouchBegin:
        //qInfo("TouchBegin Event occured");
        return true;

        case QEvent::TouchUpdate: {
            QTouchEvent* p = static_cast<QTouchEvent *>(e);
            pointersCount = p->points().size() > 10 ? 10 : p->points().size();

                for(int i = 0; i < pointersCount; ++i)
                {
                    qDebug() << "Point nb: "<< i << " x: " << p->points().at(i).position().x() << " y: " << p->points().at(i).position().y();
                    pointers[i] = QVector2D(p->points().at(i).position().x(),p->points().at(i).position().y());
                }
            }
        return true;

        case QEvent::TouchEnd:
        //qInfo("TouchEnd Event occured");
        return true;

        default:
        pointersCount = 0;
        return QWidget::event(e);
    }
}

bool OpenGLWidget::isPaintingAllowed()
{
    if(m_shaderConstructor->m_is3D)
        return (m_shaderConstructor->m_verticesCount != 0 &&
                m_shaderConstructor->m_vertices != nullptr &&
                m_shaderConstructor->m_colors != nullptr &&
                m_shaderConstructor->m_matrixPerspective != nullptr &&
                m_shaderConstructor->m_matrixTranslation != nullptr &&
                m_shaderConstructor->m_matrixRotation != nullptr);
    else
        return (m_shaderConstructor->m_verticesCount != 0 &&
                m_shaderConstructor->m_vertices != nullptr);
}
