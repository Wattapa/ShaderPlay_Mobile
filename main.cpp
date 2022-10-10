#include "shaderplay.h"

#include <QScreen>
#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include "shaderplay.h"
#include "opengltriangle.h"
#include "openglquad.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
           format.setVersion(3, 3);
           format.setProfile(QSurfaceFormat::CompatibilityProfile); //Check for what compatibility profile is
    } else {
            format.setVersion(3, 0);
    }
    format.setStencilBufferSize(8);
    format.setSamples(16);
    format.setVersion(3, 2);
    QSurfaceFormat::setDefaultFormat(format);

    ShaderPlay windowShader;
    windowShader.resize(QApplication::primaryScreen()->geometry().width(), QApplication::primaryScreen()->geometry().height());
    windowShader.show();

    return a.exec();
}
