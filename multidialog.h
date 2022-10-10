#ifndef MULTIDIALOG_H
#define MULTIDIALOG_H

#include <QDialog>

class MultiDialog : public QDialog
{
public:
    MultiDialog();
    ~MultiDialog();

     class QComboBox* comboBox() { return m_comboBox; }

private:
    QComboBox* m_comboBox;
    QStringList m_items = {"Resolution", "Time", /*"Battery",*/ "Day Time", "Time", "Touchs[10]", "TouchsCount", "Backbuffer", /*"Back Camera", "Front Camera"*/};
};

#endif // MULTIDIALOG_H
