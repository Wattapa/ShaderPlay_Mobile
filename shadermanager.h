#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QString>

class ShaderManager
{
public:
    const QString PATH =  "/Shaders/";
    const QString FRAGMENTSHADER = "/FragmentShader.frag";
    const QString VERTEXSHADER ="/VertexShader.vert";
    const QString DATAFILE ="/DataFile.data";

    ShaderManager(class OpenGLWidget* _openGLWidgetRef, class QTextEdit* _vertexWidgetRef, QTextEdit* _fragmentWidgetRef, class QComboBox* _savedShadersWidgetRef);
    ~ShaderManager();

    void MakeCurrent(int _index);
    void CompileCurrent();
    void SaveCurrent(bool _asNew=false);
    void DeleteCurrent();
    void CreateNewShader();
    void Uniform(int _index);

    void LoadSaves();

    class ShaderConstructor* shaderConstructorRef;
private:
    int current = 0;

    QString* savedShaders;

    OpenGLWidget* openGLWidgetRef;
    QTextEdit* vertexWidgetRef;
    QTextEdit* fragmentWidgetRef;
    QComboBox* savedShadersWidgetRef;
};

#endif // SHADERMANAGER_H
