#include <iostream>
#include <stdio.h>
#include <QApplication>
#include <QTreeView>


#include "test/UnitTesting.h"
#include "gui/mainwindow.h"
using namespace std;

using namespace std;
int main(int argc, char *argv[]) {
    int test = 0;   /* test toggler */

    QApplication app(argc, argv);

    if (test) {
        TestQString *test_obj1 = new TestQString();
        QTest::qExec(test_obj1);

        TestCSVReader *test_obj2 = new TestCSVReader();
        QTest::qExec(test_obj2);

        TestLoadMultipleCSVFiles *test_obj3 = new TestLoadMultipleCSVFiles();
        QTest::qExec(test_obj3);

        TestMandatoryFieldErrorProcessing *test_obj4 = new TestMandatoryFieldErrorProcessing();
        QTest::qExec(test_obj4);

        TestWindowsCompatibility *test_obj5 = new TestWindowsCompatibility();
        QTest::qExec(test_obj5);

        TestUniqueHeaders *test_obj6 = new TestUniqueHeaders();
        QTest::qExec(test_obj6);

        TestMultiTabs *test_obj7 = new TestMultiTabs();
        QTest::qExec(test_obj7);

        TestPrevNextMissingFields *test_obj8 = new TestPrevNextMissingFields();
        QTest::qExec(test_obj8);

        TestTeachingDivisionSortField *test_obj9 = new TestTeachingDivisionSortField();
        QTest::qExec(test_obj9);

        TestSaveSessionState *test_obj10 = new TestSaveSessionState();
        QTest::qExec(test_obj10);

        TestLoadSessionState *test_obj11 = new TestLoadSessionState();
        QTest::qExec(test_obj11);

        TestSavingTabToCSV *test_obj12 = new TestSavingTabToCSV();
        QTest::qExec(test_obj12);
    }

    //Mainscreen w;
    MainWindow w;
    w.show();

    return app.exec();
}
