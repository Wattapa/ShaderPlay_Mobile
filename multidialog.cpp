#include "multidialog.h"
#include "shaderplay.h"

#include <cstdio>
#include <functional>
#include <QComboBox>
#include <QHBoxLayout>

MultiDialog::MultiDialog()
{
    m_comboBox = new QComboBox;

    setLayout(new QHBoxLayout());

    m_comboBox->addItems(m_items);
    layout()->addWidget(m_comboBox);
}

MultiDialog::~MultiDialog()
{
    delete m_comboBox;
}
