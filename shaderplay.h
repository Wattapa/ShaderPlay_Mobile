#ifndef SHADERPLAY_H
#define SHADERPLAY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ShaderPlay; }
QT_END_NAMESPACE

class ShaderPlay : public QMainWindow
{
    Q_OBJECT

public:
    typedef enum SETTINGSMENU
    {
        SaveShader,
        SaveAsNewShader,
        NewShader,
        DeleteShader,
        SetAsWallpaper,
    }SettingsMenu;

    ShaderPlay(QWidget *parent = nullptr);
    ~ShaderPlay();

protected slots:
    void on_SwitchView_clicked();
    void on_SwitchShaderSource_clicked();
    void on_ButtonCompile_clicked();
    void on_ButtonUndo_clicked();
    void on_ButtonRedo_clicked();

    void MakeCurrentSlot(int _itemIndex);
    void OnSettingsMenu(int _itemIndex);
    void OnUniformMenu(int _itemIndex);

private:
    Ui::ShaderPlay *ui;

    class ShaderManager* shaderMgrInstance;

    class TextHighlighter* highlighterVertex;
    TextHighlighter* highlighterFragment;
};
#endif // SHADERPLAY_H
