#ifndef PRESENTATIONTREEMODEL_H
#define PRESENTATIONTREEMODEL_H

#include "datamodel/TreeModel.h"
#include "database/RecordsManager.h"
#include "datamodel/TreeItem.h"

class PresentationTreeModel : public TreeModel
{
public:
    PresentationTreeModel(RecordsManager* db, QObject *parent = 0);
    ~PresentationTreeModel();
    void setupModel(int start, int end, std::vector<std::string> sortFields, char filterStart, char filterEnd);
};

#endif // PRESENTATIONTREEMODEL_H
