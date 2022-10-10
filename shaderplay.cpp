#include "shaderplay.h"
#include "ui_shaderplay.h"
#include "shadermanager.h"
#include "texthighlighter.h"

ShaderPlay::ShaderPlay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShaderPlay)
{
    ui->setupUi(this);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    ui->VertexSource->setFont(font);
    ui->FragmentSource->setFont(font);
    ui->SwitchView->setFont(font);
    ui->SwitchShaderSource->setFont(font);
    ui->MenuSavedShaders->setFont(font);
    ui->MenuSettings->setFont(font);
    ui->MenuUniTex->setFont(font);
    ui->ButtonCompile->setFont(font);
    ui->ButtonUndo->setFont(font);
    ui->ButtonRedo->setFont(font);

    highlighterVertex = new TextHighlighter(ui->VertexSource->document());
    highlighterFragment = new TextHighlighter(ui->FragmentSource->document());

    shaderMgrInstance = new ShaderManager(ui->DisplayShader, ui->VertexSource, ui->FragmentSource, ui->MenuSavedShaders);

    connect(ui->MenuSavedShaders, SIGNAL(activated(int)), this, SLOT(MakeCurrentSlot(int)));
    connect(ui->MenuSettings, SIGNAL(activated(int)), this, SLOT(OnSettingsMenu(int)));
    connect(ui->MenuUniTex, SIGNAL(activated(int)), this, SLOT(OnUniformMenu(int)));

    ui->VertexSource->hide();
    ui->FragmentSource->hide();
    ui->SwitchView->setBackgroundRole(QPalette::Dark);
    ui->SwitchView->setText("Source");
    ui->SwitchShaderSource->setText("Fragment");
    ui->SwitchView->setText("Source");

    ui->MenuSettings->insertItem(0, "Save Shader");
    ui->MenuSettings->insertItem(1, "Save as new Shader");
    ui->MenuSettings->insertItem(2, "New Shader");
    ui->MenuSettings->insertItem(3, "Delete Shader");
    ui->MenuSettings->insertItem(5, "Set as Wallpaper");

    ui->MenuUniTex->insertItem(0, "Resolution");
    ui->MenuUniTex->insertItem(1, "Time");
    ui->MenuUniTex->insertItem(2, "Day Time");
    ui->MenuUniTex->insertItem(3, "Date");
    ui->MenuUniTex->insertItem(5, "Touch[10]");
    ui->MenuUniTex->insertItem(6, "Touch Count");
    ui->MenuUniTex->insertItem(7, "Backbuffer");
}

ShaderPlay::~ShaderPlay()
{
    delete ui;
}

void ShaderPlay::on_SwitchView_clicked()
{
    if(ui->DisplayShader->isVisible())
    {
        if(ui->SwitchShaderSource->text() == "Vertex") ui->VertexSource->show();
        else ui->FragmentSource->show();
        ui->DisplayShader->hide();
        ui->SwitchView->setText("Source");
    }
    else
    {
        ui->DisplayShader->show();
        if(ui->VertexSource->isVisible()) ui->VertexSource->hide();
        ui->FragmentSource->hide();
        ui->SwitchView->setText("Shader");
    }
}

void ShaderPlay::on_SwitchShaderSource_clicked()
{
    if(!ui->DisplayShader->isVisible())
    {
        if(ui->VertexSource->isVisible())
        {
            ui->FragmentSource->show();
            ui->VertexSource->hide();
            ui->SwitchShaderSource->setText("Fragment");
        }
        else
        {
            ui->VertexSource->show();
            ui->FragmentSource->hide();
            ui->SwitchShaderSource->setText("Vertex");
        }
    }
    else
    {
        if(ui->SwitchShaderSource->text() == "Vertex")
        {
            ui->SwitchShaderSource->setText("Fragment");
        }
        else
        {
            ui->SwitchShaderSource->setText("Vertex");
        }
    }
}

void ShaderPlay::on_ButtonCompile_clicked()
{
    shaderMgrInstance->CompileCurrent();
}

void ShaderPlay::on_ButtonUndo_clicked()
{
    if(ui->SwitchShaderSource->text() == "Vertex")
    {
        ui->VertexSource->undo();
    }
    else
    {
        ui->FragmentSource->undo();
    }
}

void ShaderPlay::on_ButtonRedo_clicked()
{
    if(ui->SwitchShaderSource->text() == "Vertex")
    {
        ui->VertexSource->redo();
    }
    else
    {
        ui->FragmentSource->redo();
    }
}

void ShaderPlay::MakeCurrentSlot(int _itemIndex)
{
    shaderMgrInstance->MakeCurrent(_itemIndex);
}

void ShaderPlay::OnSettingsMenu(int _itemIndex)
{
    switch(_itemIndex)
    {
    case SaveShader:
        shaderMgrInstance->SaveCurrent();
        break;
    case SaveAsNewShader:
        shaderMgrInstance->SaveCurrent(true);
        break;
    case NewShader:
        shaderMgrInstance->CreateNewShader();
        break;
    case DeleteShader:
        shaderMgrInstance->DeleteCurrent();
        break;
    case SetAsWallpaper:
        //shaderMgrInstance->
        break;
    default:
        break;
    }
    ui->MenuSettings->setCurrentIndex(-1);
}

void ShaderPlay::OnUniformMenu(int _itemIndex)
{
    shaderMgrInstance->Uniform(_itemIndex);
    ui->MenuUniTex->setCurrentIndex(-1);
}
