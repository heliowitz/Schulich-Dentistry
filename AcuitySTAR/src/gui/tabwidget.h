#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>

namespace Ui {
class tabWidget;
}

class tabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tabWidget(QWidget *parent = 0);
    ~tabWidget();

private:
    Ui::tabWidget *ui;
};

#endif // TABWIDGET_H
