#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QDebug>
#include <QWidget>

#include "ui_mainscreen.h"

namespace Ui {
    class Mainscreen;
}

class Mainscreen : public QWidget
{
    Q_OBJECT

public:
    Mainscreen(QWidget *parent = 0);
    ~Mainscreen();

private:
    Ui::Mainscreen *ui;
};

#endif // MAINSCREEN_H
