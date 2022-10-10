#ifndef SHADERCONSTRUCTOR_H
#define SHADERCONSTRUCTOR_H

#include <QtOpenGL/QtOpenGL>

class ShaderConstructor
{
public:
    const QString PATH =  "/Shaders/";
    const QString FRAGMENTSHADER = "/FragmentShader.frag";
    const QString VERTEXSHADER ="/VertexShader.vert";
    const QString DATAFILE ="/DataFile.data";

    typedef enum UNIFORM
    {
        resolution,
        time,
        //battery,
        daytime,
        date,
        pointers,
        pointersCount,
        backBuffer
        //cameraBack,
        //cameraFront
    }Uniform;

    ShaderConstructor();
    ~ShaderConstructor();

    void ChangePath(QString _name);

    const QString OpenFile(QString _path);
    void SaveFile(QString _type, QString _text);

    void SetCurrentShader(QString _newName);
    void CompileCurrentShader(QString _vertex, QString _fragment);

    void LoadDataFile();
    void SaveFiles(QString _oldVertex=nullptr, QString _oldFragment=nullptr);

    void CreateNewShader(QString _name, bool _is3D);
    void CreateDataFile(bool _is3D=false, int _verticesCount=0, GLfloat* _vertices=nullptr, GLfloat* _colors=nullptr, float* _perspective=nullptr, float* _translation=nullptr, float* _rotation=nullptr);
    void CreateVertexShader();
    void CreateFragmentShader();

    void AddUniform(QString _shaderType, int _type);
    void RemoveUniform(QString _shaderType, int _type);

//Data
    bool m_is3D = false;
    bool m_resolutionUniform = false;
    bool m_timeUniform = false;
    bool m_batteryUniform = false;
    bool m_daytimeUniform = false;
    bool m_dateUniform = false;
    bool m_pointersUniform = false;
    bool m_pointersCountUniform = false;
    bool m_backBufferUniform = false;
    bool m_cameraBackUniform = false;
    bool m_cameraFrontUniform = false;

    GLfloat* m_vertices;
    GLfloat* m_colors;
    int m_verticesCount;

    float* m_matrixPerspective;
    float* m_matrixTranslation;
    float* m_matrixRotation;

    QString m_vertexShaderSource;
    QString m_fragmentShaderSource;

    QString m_path;
};
#endif // SHADERCONSTRUCTOR_H
