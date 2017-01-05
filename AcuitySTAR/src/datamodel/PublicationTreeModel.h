#ifndef PUBLICATIONTREEMODEL_H
#define PUBLICATIONTREEMODEL_H

#include "datamodel/TreeModel.h"
#include "database/RecordsManager.h"
#include "datamodel/TreeItem.h"

class PublicationTreeModel : public TreeModel
{
public:
    PublicationTreeModel(RecordsManager* db, QObject *parent = 0);
    ~PublicationTreeModel();
    void setupModel(int start, int end, std::vector<std::string> sortFields, char filterStart, char filterEnd);
};

#endif // PUBLICATIONTREEMODEL_H
