#include <QtTest/QtTest>
#include <string>
#include <vector>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
//#include <windows.h>
#include <stdio.h>
//#include <VersionHelpers.h>
using namespace std;

#include "database/CSVReader.h"
#include "database/RecordsManager.h"
#include "datamodel/GrantFundingTreeModel.h"
#include "datamodel/PresentationTreeModel.h"
#include "datamodel/PublicationTreeModel.h"
#include "datamodel/TeachingTreeModel.h"
#include "gui/ErrorEditDialog.h"
#include "gui/tab.h"

class TestQString: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};

class TestCSVReader: public QObject
{
    Q_OBJECT
private slots:
    void test();
    bool CSV_READER_TEST_01();
    bool CSV_READER_TEST_02();
    bool CSV_READER_TEST_03();
    bool CSV_READER_TEST_04();
    bool CSV_READER_TEST_05();
    bool CSV_READER_TEST_06();
    bool CSV_READER_TEST_07();
    bool CSV_READER_TEST_08();
};

class TestLoadMultipleCSVFiles: public QObject
{
    enum TABS {
        TEACH, PUBLICATIONS, PRESENTATIONS, FUNDING, TOTAL_TABS
    };

    std::vector<std::string> MANFIELDS[TOTAL_TABS] = {
        // TEACH TABS
        {"Member Name", "Start Date", "Program", "Division"},
        // PUBLICATIONS TABS
        {"Member Name", "Type", "Status Date", "Role", "Title"},
        // PRESENTATIONS TABS
        {"Member Name", "Date", "Type", "Role", "Title"},
        // FUNDING TABS
        {"Member Name", "Funding Type", "Status", "Peer Reviewed?", "Role", "Title", "Start Date"}
    };

    Q_OBJECT
private slots:
    void test();
    bool validateTree(int index, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header);
};

class TestMandatoryFieldErrorProcessing: public QObject
{
    enum TABS {
        TEACH, PUBLICATIONS, PRESENTATIONS, FUNDING, TOTAL_TABS
    };

    std::vector<std::string> MANFIELDS[TOTAL_TABS] = {
        // TEACH TABS
        {"Member Name", "Start Date", "Program"},
        // PUBLICATIONS TABS
        {"Member Name", "Type", "Status Date", "Role", "Title"},
        // PRESENTATIONS TABS
        {"Member Name", "Date", "Type", "Role", "Title"},
        // FUNDING TABS
        {"Member Name", "Funding Type", "Status", "Peer Reviewed?", "Role", "Title", "Start Date"}
    };

    Q_OBJECT
private slots:
    void test(QWidget *parent=0);
    bool validateTree(int fileType, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header, QWidget *parent=0);
    bool handle_field_errors(QWidget *parent, std::vector<std::vector<std::string>*>& errors,
                             std::vector<std::string>& headers,
                             std::vector<std::string>& mandatory);
};

class TestWindowsCompatibility: public QObject
{
    enum TABS {
        TEACH, PUBLICATIONS, PRESENTATIONS, FUNDING, TOTAL_TABS
    };

    Q_OBJECT
private slots:
    void test();
};

class TestUniqueHeaders: public QObject
{
    enum TABS {
        TEACH, PUBLICATIONS, PRESENTATIONS, FUNDING, TOTAL_TABS
    };

    Q_OBJECT
private slots:
    void test();
};

class TestMultiTabs: public QObject
{
    enum TAB_TYPES {
        TEACH_TAB, PUBLICATIONS_TAB, PRESENTATIONS_TAB, FUNDING_TAB, TOTAL_TAB_TYPES
    };

    std::vector<std::string> MANFIELDS[TOTAL_TAB_TYPES] = {
        // TEACH TABS
        {"Member Name", "Start Date", "Program", "Division"},
        // PUBLICATIONS TABS
        {"Member Name", "Type", "Status Date", "Role", "Title"},
        // PRESENTATIONS TABS
        {"Member Name", "Date", "Type", "Role", "Title"},
        // FUNDING TABS
        {"Member Name", "Funding Type", "Status", "Peer Reviewed?", "Role", "Title", "Start Date"}
    };

    Q_OBJECT
private slots:
    void test();
    bool checkFile(Tab *newTab, QString filePath);
};

class TestPrevNextMissingFields: public QObject
{
    enum TABS {
        TEACH, PUBLICATIONS, PRESENTATIONS, FUNDING, TOTAL_TABS
    };

    std::vector<std::string> MANFIELDS[TOTAL_TABS] = {
        // TEACH TABS
        {"Member Name", "Start Date", "Program", "Division"},
        // PUBLICATIONS TABS
        {"Member Name", "Type", "Status Date", "Role", "Title"},
        // PRESENTATIONS TABS
        {"Member Name", "Date", "Type", "Role", "Title"},
        // FUNDING TABS
        {"Member Name", "Funding Type", "Status", "Peer Reviewed?", "Role", "Title", "Start Date"}
    };

    Q_OBJECT
private slots:
    void test(QWidget *parent=0);
    bool validateTree(int fileType, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header, QWidget *parent=0);
    bool handle_field_errors(QWidget *parent, std::vector<std::vector<std::string>*>& errors,
                             std::vector<std::string>& headers,
                             std::vector<std::string>& mandatory);
};

class TestTeachingDivisionSortField: public QObject
{
    Q_OBJECT
private slots:
    void test();
};

//d4
class TestSaveSessionState: public QObject
{
    Q_OBJECT
private slots:
    void test();
};

class TestLoadSessionState: public QObject
{
    Q_OBJECT
private slots:
    void test();
};

class TestSavingTabToCSV: public QObject
{
    enum TABS {
        TEACH, PUBLICATIONS, PRESENTATIONS, FUNDING, TOTAL_TABS
    };

    std::vector<std::string> MANFIELDS[TOTAL_TABS] = {
        // TEACH TABS
        {"Member Name", "Start Date", "Program", "Division"},
        // PUBLICATIONS TABS
        {"Member Name", "Type", "Status Date", "Role", "Title"},
        // PRESENTATIONS TABS
        {"Member Name", "Date", "Type", "Role", "Title"},
        // FUNDING TABS
        {"Member Name", "Funding Type", "Status", "Peer Reviewed?", "Role", "Title", "Start Date"}
    };

    Q_OBJECT
private slots:
    void test();
    bool validateTree(int index, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header);
};
