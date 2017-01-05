#include "helpscreen.h"
#include "ui_helpscreen.h"

helpscreen::helpscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helpscreen)
{
    ui->setupUi(this);
}

helpscreen::~helpscreen()
{
    delete ui;
}
