#include "datamodel/TabTreeModel.h"

TabTreeModel::TabTreeModel(int fileType, RecordsManager* db, QObject *parent):
    TreeModel(db, parent) {

    this->fileType = fileType;
}

TabTreeModel::~TabTreeModel() {
}

void TabTreeModel::setupModel(int yearStart, int yearEnd, std::vector<std::string> sortFields, char filterStart, char filterEnd) {
    rootItem = new TreeItem(dataObj->createHeadersListForTab(fileType, sortFields[0]));
    setupModelData(dataObj->createQStringForTab(fileType, yearStart, yearEnd, sortFields, filterStart, filterEnd).split(QString("\n")), rootItem);
}
