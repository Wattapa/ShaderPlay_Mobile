#include "shadermanager.h"

#include <QDir>
#include <QFileInfoList>
#include <QTextEdit>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QInputDialog>

#include "openglwidget.h"
#include "shaderconstructor.h"

ShaderManager::ShaderManager(OpenGLWidget* _openGLWidgetRef, QTextEdit* _vertexWidgetRef, QTextEdit* _fragmentWidgetRef, QComboBox* _savedShadersWidgetRef)
{
    openGLWidgetRef = _openGLWidgetRef;
    shaderConstructorRef = _openGLWidgetRef->m_shaderConstructor;
    vertexWidgetRef = _vertexWidgetRef;
    fragmentWidgetRef = _fragmentWidgetRef;
    savedShadersWidgetRef = _savedShadersWidgetRef;

    LoadSaves();
    MakeCurrent(0);
}

ShaderManager::~ShaderManager()
{
    delete savedShaders;
    delete shaderConstructorRef;
    delete vertexWidgetRef;
    delete fragmentWidgetRef;
}

void ShaderManager::MakeCurrent(int _index)
{
    current = _index;
    shaderConstructorRef->SetCurrentShader(savedShaders[current]);
    vertexWidgetRef->setText(shaderConstructorRef->m_vertexShaderSource);
    fragmentWidgetRef->setText(shaderConstructorRef->m_fragmentShaderSource);
}

void ShaderManager::CompileCurrent()
{
    shaderConstructorRef->CompileCurrentShader(vertexWidgetRef->toPlainText(), fragmentWidgetRef->toPlainText());
    openGLWidgetRef->MgrInit();
    openGLWidgetRef->update();
}

void ShaderManager::SaveCurrent(bool _asNew)
{
    if(_asNew)
    {
        QDir dirShader = QDir(QDir::currentPath() + PATH);
        dirShader.mkdir(savedShaders[current] + "_copy");
        shaderConstructorRef->ChangePath(savedShaders[current] + "_copy");
        LoadSaves();
    }

    shaderConstructorRef->SaveFiles(vertexWidgetRef->toPlainText(), fragmentWidgetRef->toPlainText());
}

void ShaderManager::DeleteCurrent()
{
    QMessageBox msgBox;
    msgBox.setText("Do you want to delete " + savedShaders[current] + " ?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);

    switch (msgBox.exec())
    {
      case QMessageBox::No:
          break;
      case QMessageBox::Yes:
          {
            QDir dirShader(QDir::currentPath() + PATH + savedShaders[current]);
            dirShader.removeRecursively();
            LoadSaves();
            MakeCurrent(0);
          }
          break;
      default:
          break;
    }
}

void ShaderManager::CreateNewShader()
{
    bool ok;
    QString name = QInputDialog::getText(openGLWidgetRef, "Shader Wizard",
                                         "Shader Name:", QLineEdit::Normal,
                                         "New Name", &ok);

    if(ok && !name.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Do you want to make 3D Shader ?");
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);

        switch (msgBox.exec())
        {
          case QMessageBox::No:
              {
                QDir dir(QDir::currentPath() + PATH);
                dir.mkdir(name);
                shaderConstructorRef->CreateNewShader(name, false);
              }
              break;
          case QMessageBox::Yes:
              {//TODO: Construct a way to create 3D Shader with custom data
                QDir dir(QDir::currentPath() + PATH);
                dir.mkdir(name);
                shaderConstructorRef->CreateNewShader(name, true);
              }
              break;
          default:
              break;
        }
        LoadSaves();
    }
}

void ShaderManager::Uniform(int _index)
{
    switch (_index)
    {
        case ShaderConstructor::resolution:
            {
                if(shaderConstructorRef->m_resolutionUniform) shaderConstructorRef->RemoveUniform("çasertàrien", 0);
                else shaderConstructorRef->AddUniform("çasertàrien", 0);
            }
            break;
        case ShaderConstructor::time:
            {
                if(shaderConstructorRef->m_timeUniform) shaderConstructorRef->RemoveUniform("çasertàrien", 1);
                else shaderConstructorRef->AddUniform("çasertàrien", 1);
            }
            break;
        //case QMessageBox::ApplyRole: shaderConstructorRef->AddUniform("çasertàrien", 2);
        //    break;
        case ShaderConstructor::daytime:
            {
                if(shaderConstructorRef->m_daytimeUniform) shaderConstructorRef->RemoveUniform("çasertàrien", 2);
                else shaderConstructorRef->AddUniform("çasertàrien", 2);
            }
            break;
        case ShaderConstructor::date:
            {
                if(shaderConstructorRef->m_dateUniform) shaderConstructorRef->RemoveUniform("çasertàrien", 3);
                else shaderConstructorRef->AddUniform("çasertàrien", 3);
            }
            break;
        case ShaderConstructor::pointers:
            {
                if(shaderConstructorRef->m_pointersUniform) shaderConstructorRef->RemoveUniform("çasertàrien", 4);
                else shaderConstructorRef->AddUniform("çasertàrien", 4);
            }
            break;
        case ShaderConstructor::pointersCount:
            {
                if(shaderConstructorRef->m_pointersCountUniform) shaderConstructorRef->RemoveUniform("çasertàrien", 5);
                else shaderConstructorRef->AddUniform("çasertàrien", 5);
            }
            break;
        case ShaderConstructor::backBuffer:
            {
                if(shaderConstructorRef->m_backBufferUniform) shaderConstructorRef->RemoveUniform("çasertàrien", 6);
                else shaderConstructorRef->AddUniform("çasertàrien", 6);
            }
            break;
        //case QMessageBox::ResetRole: shaderConstructorRef->AddUniform("çasertàrien", 8);
        //    break;
        //case QMessageBox::YesRole: shaderConstructorRef->AddUniform("çasertàrien", 9);
        //    break;
          default:
              break;
    }

    MakeCurrent(current);
}

void ShaderManager::LoadSaves()
{
    QDir dirShader = QDir(QDir::currentPath() + PATH);

    if(!dirShader.exists())
    {
        QDir dirMain(QDir::currentPath());
        dirMain.mkdir("Shaders");
        dirShader.mkdir("Example3D");
        dirShader.mkdir("Example2D");

        shaderConstructorRef->CreateNewShader("Example3D", true);
        shaderConstructorRef->CreateNewShader("Example2D", false);
    }

    dirShader.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    QFileInfoList list = dirShader.entryInfoList();

    savedShadersWidgetRef->clear();
    savedShaders = new QString[list.size()];

    for(int i = 0; i < list.size(); ++i)
    {
       QFileInfo fileInfo = list.at(i);
       savedShaders[i] = fileInfo.fileName();

       savedShadersWidgetRef->insertItem(i, savedShaders[i]);
    }
}
