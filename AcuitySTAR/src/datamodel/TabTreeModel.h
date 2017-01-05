#ifndef TABTREEMODEL_H
#define TABTREEMODEL_H

#include "datamodel/TreeModel.h"
#include "database/RecordsManager.h"
#include "datamodel/TreeItem.h"

class TabTreeModel : public TreeModel
{
public:
    TabTreeModel(int fileType, RecordsManager* db, QObject *parent=0);
    ~TabTreeModel();
    void setupModel(int start, int end, std::vector<std::string> sortFields, char filterStart, char filterEnd);
    int fileType;
};

#endif // TABTREEMODEL_H
