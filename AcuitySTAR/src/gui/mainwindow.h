#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMainWindow>
#include <vector>
#include <QDate>
#include <QDebug>
#include <QDirModel>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPrintDialog>
#include <QPrinter>
#include <QString>
#include <fstream>
#include <ostream>
#include <istream>
#include <sstream>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include "ui_mainwindow.h"
#include "ErrorEditDialog.h"
#include "tab.h"
#include "tabwidget.h"
#include "mainscreen.h"
#include "helpscreen.h"

#include "database/CSVReader.h"
#include "database/QSortListIO.h"
#include "database/RecordsManager.h"
#include "datamodel/TreeModel.h"

class RecordsManager;
class CSVReader;
class TreeModel;
class CustomSort;
class QListWidget;
class QTreeView;
class QPrinter;
class QPainter;
class PieChartWidget;
class QCustomPlot;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    std::vector<std::string> MANFIELDS[TOTAL_TAB_TYPES] = {
        {"Member Name", "Start Date", "Program", "Division"},  //Teaching
        {"Member Name", "Type", "Status Date", "Role", "Title", "Division"},  //Publications
        {"Member Name", "Role", "Type", "Title", "Date", "Division"},  //Presentations
        {"Member Name", "Funding Type", "Status", "Peer Reviewed?", "Role", "Title", "Start Date", "Division"} //Grants
    };


private slots:
    QString load_file();

    //void on_load_file_clicked();

    void on_actionLoad_file_triggered();

//    void on_FromDate_dateChanged(const QDate &date);
//    void on_ToDate_dateChanged(const QDate &date);

    void refresh();

    void on_categoryTab_currentChanged();

    //void on_toolButton_clicked();

    void on_categoryTab_tabCloseRequested(int index);

    void closeEvent(QCloseEvent *event);

    void saveSessionState();

    void loadSessionState();

    //bool saveSession(const QString& filename);

private:
    struct field_error;

    Ui::MainWindow* ui;

    Tab *activeTab;

    /*QList<QStringList> allTeachOrders, allPubOrders, allPresOrders, allFundOrders;
    QString teachPath, pubPath, presPath, fundPath;
    TreeModel *fundTree, *presTree, *pubTree, *teachTree;
    RecordsManager *funddb, *presdb, *pubdb, *teachdb;
    std::vector<std::vector<std::string>> fundData, presData, pubData, teachData;
    std::vector<std::string> teachSortOrder, pubSortOrder, presSortOrder, fundSortOrder;
    QString teachClickedName, pubClickedName, presClickedName, fundClickedName;*/

    std::vector<Tab*> tabs;

    /*TreeModel* currentTree;
    RecordsManager* currentdb;*/
    QTreeView* currentView;

    QList<bool> dateChanged;

    std::vector<std::string> getParentsList(QModelIndex &index);

    int checkFile(QString filePath);
    //int checkFile(int index, QString filePath);
    //std::vector<std::string> getSelectedSortOrder(int tabIndex);
    void createDefaultSortOrder(Tab *tab);
    void makeTree();

    bool handle_field_errors(std::vector<std::vector<std::string>*>& errors,
                             std::vector<std::string>& headers,
                             std::vector<std::string>& mandatory);

    bool load(QString path, bool multi_file = false);


};

#endif // MAINWINDOW_H
