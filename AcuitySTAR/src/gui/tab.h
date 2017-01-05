#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <vector>
#include <QDate>
#include <QString>
#include <fstream>
#include <ostream>
#include <istream>

#include "ui_tab.h"
#include "datamodel/TabTreeModel.h"
#include "database/RecordsManager.h"
#include "database/QSortListIO.h"
#include "CustomSort.h"

#define FUNDORDER_SAVE  "fundsortorder.dat"
#define PRESORDER_SAVE  "pressortorder.dat"
#define PUBORDER_SAVE   "pubsortorder.dat"
#define TEACHORDER_SAVE "teachsortorder.dat"

enum TAB_TYPES {
    TEACH_TAB, PUBLICATIONS_TAB, PRESENTATIONS_TAB, FUNDING_TAB, TOTAL_TAB_TYPES
};

static std::string ORDERS_SAVE[TOTAL_TAB_TYPES] = {TEACHORDER_SAVE, PUBORDER_SAVE, PRESORDER_SAVE, FUNDORDER_SAVE};

namespace Ui {
    class Tab;
}

class BoxWhiskersWidget;
class PieChartWidget;
class QCustomPlot;

class Tab : public QWidget
{
    Q_OBJECT

public:
    std::vector<std::string> MANFIELDS[TOTAL_TAB_TYPES] = {
        {"Member Name", "Start Date", "Program", "Division"},  //Teaching
        {"Member Name", "Type", "Status Date", "Role", "Title", "Division"},  //Publications
        {"Member Name", "Role", "Type", "Title", "Date", "Division"},  //Presentations
        {"Member Name", "Funding Type", "Status", "Peer Reviewed?", "Role", "Title", "Start Date", "Division"} //Grants
    };

    explicit Tab(QWidget *parent = 0);
    ~Tab();

    std::vector<std::vector<std::string>> saveToNewCSV(QWidget *parent=0);

    void refresh();
    void makeTree();
    char getFilterStartChar();
    char getFilterEndChar();
	
    void setupBoxWhiskersChart(BoxWhiskersWidget* boxChart, std::vector<std::pair <std::string, std::vector<double>>> boxChartList);

    void setupPieChart(PieChartWidget *pieChart, QListWidget *pieListWidget, std::vector<std::pair<std::string, double> > pieChartList);
    void setupBarChart(QCustomPlot *barChart, std::vector<std::pair<std::string, double> > barChartList);
    void setupLineChart(QCustomPlot *lineChart, std::vector<std::pair<std::string, double> > lineChartList);

    QList<QStringList> allOrders;
    QString path;
    TreeModel *tree;
    RecordsManager *db;
    std::vector<std::string> sortOrder;
    std::vector<std::vector<std::string>> data;
    QString clickedName;

    int fileType;
    int yearStart, yearEnd;

    Ui::Tab* ui;

private:    
     QTreeView* currentView;
     std::vector<QString> typeNames = {"Teaching", "Publications", "Presentations", "Grants and Funding"};

private slots:
    void on_sort_currentIndexChanged(int index);
    void on_delete_sort_clicked();
    void on_pie_button_toggled(bool checked);
    void on_bar_button_toggled(bool checked);
    void on_exportButton_clicked();
    void on_filter_from_textChanged(const QString &arg1);
    void on_filter_to_textChanged(const QString &arg1);
    void on_printButton_clicked();
    //void on_treeView_clicked(const QModelIndex &index);
    void on_new_sort_clicked();
    void on_edit_sort_clicked();
    void on_FromDate_dateChanged(const QDate &date);
    void on_ToDate_dateChanged(const QDate &date);
    void on_boxwhisker_button_toggled(bool checked);
    void on_line_button_toggled(bool checked);
    void on_treeView_SelectionChanged(const QItemSelection&,const QItemSelection&);
};

#endif // TAB_H
