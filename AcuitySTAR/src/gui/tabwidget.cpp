#include "tabwidget.h"
#include "ui_tabwidget.h"

tabWidget::tabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabWidget)
{
    ui->setupUi(this);
}

tabWidget::~tabWidget()
{
    delete ui;
}
