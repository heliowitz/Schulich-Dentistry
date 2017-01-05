#-------------------------------------------------
#
# Project created by QtCreator 2015-10-21T14:49:31
#
#-------------------------------------------------

QT              += charts
QT              += core gui
QT              += testlib
CONFIG          += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = AndromedaGalaxy
TEMPLATE = app

DISTFILES += \
    TeamPeach.pro.user

HEADERS += \
    gui/CustomSort.h \
    gui/ErrorEditDialog.h \
    gui/piechartwidget.h \
    gui/mainwindow.h \
    database/CSVReader.h \
    database/RecordsManager.h \
    database/QSortListIO.h \
    datamodel/TreeItem.h \
    datamodel/TreeModel.h \
    datamodel/TeachingTreeModel.h \
    datamodel/PublicationTreeModel.h \
    datamodel/PresentationTreeModel.h \
    datamodel/GrantFundingTreeModel.h \
    test/UnitTesting.h \
    gui/qcustomplot.h \
    gui/mainscreen.h \
    datamodel/TabTreeModel.h \
    gui/tab.h \
    gui/helpscreen.h \
    gui/boxwhiskerswidget.h

SOURCES += \
    gui/CustomSort.cpp \
    gui/ErrorEditDialog.cpp \
    gui/piechartwidget.cpp \
    gui/main.cpp \
    gui/mainwindow.cpp \
    database/CSVReader.cpp \
    database/RecordsManager.cpp \
    database/QSortListIO.cpp \
    datamodel/TreeItem.cpp \
    datamodel/TreeModel.cpp \
    datamodel/TeachingTreeModel.cpp \
    datamodel/PublicationTreeModel.cpp \
    datamodel/PresentationTreeModel.cpp \
    datamodel/GrantFundingTreeModel.cpp \
    test/UnitTesting.cpp \
    gui/qcustomplot.cpp \
    gui/mainscreen.cpp \
    datamodel/TabTreeModel.cpp \
    gui/tab.cpp \
    gui/helpscreen.cpp \
    gui/boxwhiskerswidget.cpp

FORMS += \
    gui/customsort.ui \
    gui/mainwindow.ui \
    gui/ErrorEditDialog.ui \
    gui/newMainWindow.ui \
    gui/mainscreen.ui \
    gui/tab.ui \
    gui/helpscreen.ui

RESOURCES += \
    images.qrc \
    images.qrc
