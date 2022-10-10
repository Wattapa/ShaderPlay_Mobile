#include "shaderconstructor.h"

#include <QString>
#include <QFile>
#include <QDir>

ShaderConstructor::ShaderConstructor()
{
}

ShaderConstructor::~ShaderConstructor()
{
    delete m_vertices;
    delete m_colors;
    delete m_matrixPerspective;
    delete m_matrixTranslation;
    delete m_matrixRotation;
}

void ShaderConstructor::ChangePath(QString _name)
{
    m_path = QDir::currentPath() + PATH + _name;
}

const QString ShaderConstructor::OpenFile(QString _path)
{
    QFile file(_path);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString r = stream.readAll();
    return r;
}

void ShaderConstructor::SaveFile(QString _type, QString _text)
{
    QFile vs(m_path + _type);
    vs.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outv(&vs);
    outv << _text;
    vs.close();
}

void ShaderConstructor::SetCurrentShader(QString _newName)
{
    if(_newName != nullptr)
    {
        m_is3D = false;
        m_resolutionUniform = false;
        m_timeUniform = false;
        m_vertices = nullptr;
        m_colors = nullptr;
        m_verticesCount = 0;
        m_matrixPerspective = nullptr;
        m_matrixTranslation = nullptr;
        m_matrixRotation = nullptr;
        m_path = QDir::currentPath() + PATH + _newName;

        m_vertexShaderSource = OpenFile(m_path + VERTEXSHADER);
        m_fragmentShaderSource = OpenFile(m_path + FRAGMENTSHADER);
        LoadDataFile();
    }
}

void ShaderConstructor::CompileCurrentShader(QString _vertex, QString _fragment)
{
    m_vertexShaderSource = _vertex;
    m_fragmentShaderSource = _fragment;
}

void ShaderConstructor::LoadDataFile()
{
    if(m_path != nullptr)
    {
        QFile saveFile(m_path + DATAFILE);
        saveFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream out(&saveFile);
        QStringList data;
        if(saveFile.exists()) data = out.readAll().split('\n');
        if(!data.isEmpty())
        {
            for(int i=0; i < data.size(); ++i)
            {
                QStringList line = data[i].split(' ',Qt::SkipEmptyParts);

                if(!line.isEmpty())
                {
                    if(line[0] == "Is3D:")
                    {
                        m_is3D = bool(line[1].toInt());
                    }

                    if(line[0] == "UniformResolution:")
                    {
                        m_resolutionUniform = bool(line[1].toInt());
                    }

                    if(line[0] == "UniformTime:")
                    {
                        m_timeUniform = bool(line[1].toInt());
                    }

                    ////if(line[0] == "UniformBattery:")
                    ////{
                    ////    m_batteryUniform = bool(line[1].toInt());
                    ////}

                    if(line[0] == "UniformDaytime:")
                    {
                        m_daytimeUniform = bool(line[1].toInt());
                    }

                    if(line[0] == "UniformDate:")
                    {
                        m_dateUniform = bool(line[1].toInt());
                    }

                    if(line[0] == "UniformPointers:")
                    {
                        m_pointersUniform = bool(line[1].toInt());
                    }

                    if(line[0] == "UniformPointersCount:")
                    {
                        m_pointersCountUniform = bool(line[1].toInt());
                    }

                    if(line[0] == "UniformBackBuffer:")
                    {
                        m_backBufferUniform = bool(line[1].toInt());
                    }

                    //if(line[0] == "UniformCameraBack:")
                    //{
                    //    m_cameraBackUniform = bool(line[1].toInt());
                    //}
//
                    //if(line[0] == "UniformCameraFront:")
                    //{
                    //    m_cameraFrontUniform = bool(line[1].toInt());
                    //}

                    if(line[0] == "VerticesCount:")
                    {
                        m_verticesCount = line[1].toInt();
                    }

                    if(line[0] == "Vertices:")
                    {
                        QStringList tmp = line[1].split(',');
                        m_vertices = new GLfloat[m_verticesCount * 2];
                        for(int i = 0; i < m_verticesCount * 2; ++i)
                        {
                            m_vertices[i] = tmp[i].toFloat();
                        }
                    }

                    if(m_is3D)
                    {
                        if(line[0] == "VerticesColors(R,G,B):")
                        {
                            QStringList tmp = line[1].split(',');
                            m_colors = new GLfloat[m_verticesCount * 3];
                            for(int i = 0; i < m_verticesCount * 3; ++i)
                            {
                                m_colors[i] = tmp[i].toFloat();
                            }
                        }

                        if(line[0] == "MatrixPerspective(verticalAngle,aspectRatio,nearPlane,farPlane):")
                        {
                            QStringList tmp = line[1].split(',');
                            m_matrixPerspective = new float[4];
                            for(int i = 0; i < 4; ++i)
                            {
                                m_matrixPerspective[i] = tmp[i].toFloat();
                            }
                        }
                        if(line[0] == "MatrixTranslations(on(x,y,z)):")
                        {
                            QStringList tmp = line[1].split(',');
                            m_matrixTranslation = new float[3];
                            for(int i = 0; i < 3; ++i)
                            {
                                m_matrixTranslation[i] = tmp[i].toFloat();
                            }
                        }

                        if(line[0] == "MatrixRotations(around(x,y,z)):")
                        {
                            QStringList tmp = line[1].split(',');
                            m_matrixRotation = new float[3];
                            for(int i = 0; i < 3; ++i)
                            {
                                m_matrixRotation[i] = tmp[i].toFloat();
                            }
                        }
                    }
                }
            }
        }
    }
}

void ShaderConstructor::SaveFiles(QString _oldVertex, QString _oldFragment)
{
    if(m_path != nullptr)
    {
        if(_oldVertex != nullptr && _oldFragment != nullptr)
        {
            SaveFile(VERTEXSHADER, _oldVertex);
            SaveFile(FRAGMENTSHADER, _oldFragment);
        }

        QFile sf(m_path + DATAFILE);
        if(!sf.open(QIODevice::WriteOnly | QIODevice::Text)) qDebug() << "file doesn't exist: " << m_path + DATAFILE;
        QTextStream outs(&sf);

        outs << "Is3D: " << m_is3D << "\n";
        outs << "UniformResolution: " << m_resolutionUniform << "\n";
        outs << "UniformTime: " << m_timeUniform << "\n";
        //outs << "UniformBattery: " << m_batteryUniform << "\n";
        outs << "UniformDaytime: " << m_daytimeUniform << "\n";
        outs << "UniformDate: " << m_dateUniform << "\n";
        outs << "UniformPointers: " << m_pointersUniform << "\n";
        outs << "UniformPointersCount: " << m_pointersCountUniform << "\n";
        outs << "UniformBackBuffer: " << m_backBufferUniform << "\n";
        //outs << "UniformCameraBack: " << m_cameraBackUniform << "\n";
        //outs << "UniformCameraFront: " << m_cameraFrontUniform << "\n";
        outs << "VerticesCount: " << m_verticesCount << "\n" << "Vertices: ";

        for(int i=0; i < m_verticesCount*2; i+=2)
        {
            outs << m_vertices[i] << "," << m_vertices[i + 1] << (i==(m_verticesCount*2 - 2)?"\n":",");
        }

        if(m_is3D)
        {
            outs << "VerticesColors(R,G,B): ";
            for(int i=0; i < m_verticesCount*3; i+=3)
            {
                outs << m_colors[i] << "," << m_colors[i + 1] << "," << m_colors[i + 2] << (i==(m_verticesCount*3 - 3)?"\n":",");
            }

            outs << "MatrixPerspective(verticalAngle,aspectRatio,nearPlane,farPlane): ";
            for(int i=0; i < 4; ++i)
            {
                outs << m_matrixPerspective[i] << (i==3?"\n":",");
            }

            outs << "MatrixTranslations(on(x,y,z)): ";
            for(int i=0; i < 3; ++i)
            {
                outs << m_matrixTranslation[i] << (i==2?"\n":",");
            }

            outs << "MatrixRotations(around(x,y,z)): ";
            for(int i=0; i < 3; ++i)
            {
                outs << m_matrixRotation[i] << (i==2?"\n":",") ;
            }
        }
        sf.close();
    }
}

void ShaderConstructor::CreateNewShader(QString _name, bool _is3D)
{
    ChangePath(_name);
    if(_is3D)
    //Default Vertex
    {
        float vertices[] = { 0.0f,  0.707f, -0.5f, -0.5f, 0.5f, -0.5f }; //Triangle
        float colors[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
        float perspective[] = {60.0f, 4.0f / 3.0f, 0.1f, 100.0f};
        float translation[] = {0, 0, -2.0f};
        float rotation[] = {10000.0f, 0, 1, 0};

        CreateDataFile(true, 3, vertices, colors, perspective, translation, rotation);
        CreateVertexShader();
        CreateFragmentShader();
    }
    else
    //Default Fragment
    {
        CreateDataFile();
        CreateVertexShader();
        CreateFragmentShader();
        AddUniform(FRAGMENTSHADER, time);
        AddUniform(FRAGMENTSHADER, resolution);
    }
    SaveFiles();
}

void ShaderConstructor::CreateDataFile(bool _is3D, int _verticesCount, GLfloat* _vertices, GLfloat* _colors, float* _perspective, float* _translation, float* _rotation)
{
    m_is3D = _is3D;

    if(m_is3D)
    {
        m_verticesCount = _verticesCount;
        m_vertices = _vertices;
        m_colors = _colors;
        m_matrixPerspective = _perspective;
        m_matrixTranslation = _translation;
        m_matrixRotation = _rotation;
    }
    else
    {
        m_verticesCount = 6;
        m_vertices = new GLfloat[m_verticesCount * 2];
        m_vertices[0] = GLfloat(-1.0f);
        m_vertices[1] = GLfloat(1.0f);
        m_vertices[2] = GLfloat(-1.0f);
        m_vertices[3] = GLfloat(-1.0f);
        m_vertices[4] = GLfloat(1.0f);
        m_vertices[5] = GLfloat(-1.0f);
        m_vertices[6] = GLfloat(-1.0f);
        m_vertices[7] = GLfloat(1.0f);
        m_vertices[8] = GLfloat(1.0f);
        m_vertices[9] = GLfloat(-1.0f);
        m_vertices[10] = GLfloat(1.0f);
        m_vertices[11] = GLfloat(1.0f);
    }
}

void ShaderConstructor::CreateVertexShader()
{
#if (defined (_WIN32) || defined (_WIN64))
    QString newVertexShader("#version 330\n\nin vec4 posAttr;\n\n");
#else
    QString newVertexShader("#version 320 es\n\nin vec4 posAttr;\n\n");
#endif
    if(m_is3D)
    {
        newVertexShader += QString("in vec4 colAttr;\n\n"
                                   "uniform mat4 matrix;\n\n"
                                   "out vec4 col;\n\n"
                                   "void main()\n"
                                   "{\n"
                                   "      col = colAttr;\n"
                                   "      gl_Position = matrix * posAttr;\n"
                                   "}\n");
    }
    else
    {
        newVertexShader += QString("void main()\n"
                                   "{\n"
                                   "      gl_Position = posAttr;\n"
                                   "}\n");
    }
    //qDebug() << m_path + FRAGMENTSHADER;
    QFile vs(m_path + VERTEXSHADER);
    vs.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outv(&vs);
    //qDebug() << newVertexShader;
    outv << newVertexShader;
    vs.close();
}

void ShaderConstructor::CreateFragmentShader()
{
#if (defined (_WIN32) || defined (_WIN64))
    QString newFragmentShader("#version 330\n\n");
#else
    QString newFragmentShader("#version 320 es\n\n#ifndef GL_FRAGMENT_PRECISION_HIGH\nprecision highp float;\n#else\nprecision mediump float;\n#endif\n\n");
#endif
    if(m_is3D)
    {
        newFragmentShader += QString("in vec4 col;\n\n"
                                     "out vec4 color;\n\n"
                                     "void main()\n"
                                     "{\n"
                                     "     color = col;\n"
                                     "}\n");
    }
    else
    {
        newFragmentShader += QString("out vec4 color;\n\n"
                                     "void main()\n"
                                     "{\n"
                                     "     vec2 uv = gl_FragCoord.xy / resolution.xy;\n"
                                     "     vec4 finalColor = vec4(uv, 0.5*sin(time), 1.0);\n"
                                     "     color = finalColor;\n"
                                     "}\n");
    }
    //qDebug() << m_path + FRAGMENTSHADER;
    QFile fs(m_path + FRAGMENTSHADER);
    fs.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outf(&fs);
    //qDebug() << newFragmentShader;
    outf << newFragmentShader;
    fs.close();
}

void ShaderConstructor::AddUniform(QString _shaderType, int _type)
{
    QFile reader(m_path + FRAGMENTSHADER);
    reader.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream out(&reader);
    QString shader, line;

    do {
        line = out.readLine();
        shader += line + "\n";

#if (defined (_WIN32) || defined (_WIN64))
        if (line.contains("#version 330"))
#else
        if (line.contains("#endif"))
#endif
        {
            switch(_type)
            {
                //Uniforms
                case resolution:{ m_resolutionUniform = true;
                        shader += "\nuniform vec2 resolution;\n"; }
                    break;

                case time:{ m_timeUniform = true;
                        shader += "\nuniform float time;\n"; }
                    break;

                //case battery:{ m_batteryUniform = true;
                //        shader += "\nuniform float battery;\n"; }
                //    break;
                case daytime:{ m_daytimeUniform = true;
                        shader += "\nuniform vec3 daytime;\n"; }
                    break;
                case date:{ m_dateUniform = true;
                        shader += "\nuniform vec4 date;\n"; }
                    break;
                case pointers:{ m_pointersUniform = true;
                        shader += "\nuniform vec3 pointers[10];\n"; }
                    break;
                case pointersCount:{ m_pointersCountUniform = true;
                        shader += "\nuniform int pointersCount;\n"; }
                    break;

                //Textures2D
                case backBuffer:{ m_backBufferUniform = true;
                        shader += "\nuniform sampler2D backBuffer;\n"; }
                    break;
                //case cameraBack:{ m_cameraBackUniform = true;
                //        shader += "\nuniform samplerExternalOES cameraBack;\n"; }
                //    break;
                //case cameraFront:{ m_cameraFrontUniform = true;
                //        shader += "\nuniform samplerExternalOES cameraFront;\n"; }
                //    break;
                default:
                    break;
            }
        }
    } while (!line.isNull());

    QFile writer(m_path + FRAGMENTSHADER);  //+ _shaderType);
    writer.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream in(&writer);
    in << shader;

    reader.close();
    writer.close();

    SaveFiles();
}

void ShaderConstructor::RemoveUniform(QString _shaderType, int _type)
{
    QFile reader(m_path + FRAGMENTSHADER);
    reader.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream out(&reader);
    QString shader, line;

    do {
        bool write = true;
        line = out.readLine();

        switch(_type)
        {
            //Uniforms
            case resolution:{ if (line.contains("uniform vec2 resolution;")) m_resolutionUniform = write = false;}
                break;
            case time:{ if (line.contains("uniform float time;")) m_timeUniform = write = false;}
                break;
            //case battery:{ m_batteryUniform = true;
            //        shader += "\nuniform float battery;\n"; }
            //    break;
            case daytime:{ if (line.contains("uniform vec3 daytime;")) m_daytimeUniform = write = false;}
                break;
            case date:{ if (line.contains("uniform vec4 date;")) m_dateUniform = write = false;}
                break;
            case pointers:{ if (line.contains("uniform vec3 pointers[10];")) m_pointersUniform = write = false;}
                break;
            case pointersCount:{ if (line.contains("uniform int pointersCount;")) m_pointersCountUniform = write = false;}
                break;
            //Textures2D
            case backBuffer:{ if (line.contains("uniform sampler2D backBuffer;")) m_backBufferUniform = write = false;}
                break;
            //case cameraBack:{ m_cameraBackUniform = true;
            //        shader += "\nuniform samplerExternalOES cameraBack;\n"; }
            //    break;
            //case cameraFront:{ m_cameraFrontUniform = true;
            //        shader += "\nuniform samplerExternalOES cameraFront;\n"; }
            //    break;
            default:
                break;
        }

        if(write) shader += line + "\n";

    } while (!line.isNull());

    QFile writer(m_path + FRAGMENTSHADER);  //+ _shaderType);
    writer.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream in(&writer);
    in << shader;

    reader.close();
    writer.close();

    SaveFiles();
}
