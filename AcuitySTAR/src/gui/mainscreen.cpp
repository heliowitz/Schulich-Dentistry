#include "mainscreen.h"

Mainscreen::Mainscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mainscreen)
{
    ui->setupUi(this);

//    QLabel label;
//    QPixmap pixmap(":/galaxy.png");
//    label.setPixmap(pixmap);
//    label.setMask(pixmap.mask());

//    label.show();
}

Mainscreen::~Mainscreen()
{
    delete ui;
}

