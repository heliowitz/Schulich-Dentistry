#ifndef ERROREDITDIALOG_H
#define ERROREDITDIALOG_H

#include <QDialog>
#include <vector>
#include <string>
#include "QTableWidgetItem"

class QAbstractButton;

namespace Ui {
    class ErrorEditDialog;
}

class ErrorEditDialog : public QDialog
{
    Q_OBJECT

public:
     ErrorEditDialog(QWidget *parent,
                     std::vector<std::vector<std::string>*>& errors,
                     std::vector<std::string>& headers,
                     std::vector<std::string>& mandatory);
    ~ErrorEditDialog();

private slots:
     void on_save_clicked();
     void on_cancel_clicked();
     void on_NextBtn_clicked();
     void on_PrevBtn_clicked();
     void on_tableWidget_itemChanged(QTableWidgetItem * item);

private:
    std::vector<std::vector<std::string>*> errorList;
    std::vector<std::string> headerList;
    std::vector<std::string> mandatoryList;

    Ui::ErrorEditDialog *ui;
    bool populated = false;

    void saveData();
};

#endif // ERROREDITDIALOG_H
