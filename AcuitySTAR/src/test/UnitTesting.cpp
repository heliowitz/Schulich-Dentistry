#include "UnitTesting.h"
#include "gui/mainwindow.h"
#include "database/CSVReader.h"

void TestQString::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}

bool TestCSVReader::CSV_READER_TEST_01(){
CSVReader reader("../Project Information/Sample Data/Grants_expanded.csv");
vector<string> header = reader.getHeaders();
if(header.size() != 0) return true;
return false;
}

bool TestCSVReader::CSV_READER_TEST_02(){
CSVReader reader("../Project Information/Sample Data/GrantsClinicalFunding_sample.csv");
vector<string> header = reader.getHeaders();
if(header.size() != 0) return true;
return false;
}

bool TestCSVReader::CSV_READER_TEST_03(){
CSVReader reader("../Project Information/Sample Data/Presentations_expanded.csv");
vector<string> header = reader.getHeaders();
if(header.size() != 0) return true;
return false;
}
bool TestCSVReader::CSV_READER_TEST_04(){
CSVReader reader("../Project Information/Sample Data/Presentations_sample.csv");
vector<string> header = reader.getHeaders();
if(header.size() != 0) return true;
return false;
}
bool TestCSVReader::CSV_READER_TEST_05(){
CSVReader reader("../Project Information/Sample Data/Program_Teaching_expanded.csv");
vector<string> header = reader.getHeaders();
if(header.size() != 0) return true;
return false;
}

bool TestCSVReader::CSV_READER_TEST_06(){
CSVReader reader("NO FILE");
vector<string> header = reader.getHeaders();
if(header.size() == 0) return true;
return false;
}

bool TestCSVReader::CSV_READER_TEST_07(){
CSVReader reader("../Project Information/Sample Data/Presentations_sample.csv");
vector< vector<string> > all_data = reader.getData();
if(all_data.size() != 0) return true;
return false;
}

bool TestCSVReader::CSV_READER_TEST_08(){
CSVReader reader("../Project Information/Sample Data/Program_Teaching_expanded.csv");
vector< vector<string> > all_data = reader.getData();
if(all_data.size() != 0) return true;
return false;
}

void TestCSVReader::test(){
QVERIFY(CSV_READER_TEST_01() == true);
QVERIFY(CSV_READER_TEST_02() == true);
QVERIFY(CSV_READER_TEST_03() == true);
QVERIFY(CSV_READER_TEST_04() == true);
QVERIFY(CSV_READER_TEST_05() == true);
QVERIFY(CSV_READER_TEST_06() == true);
QVERIFY(CSV_READER_TEST_07() == true);
QVERIFY(CSV_READER_TEST_08() == true);
}

bool TestLoadMultipleCSVFiles::validateTree(int fileType, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header){
    std::string searchstring;
    std::vector<std::vector<std::string>> fundData, presData, pubData, teachData;

    int sortHeaderIndex = 2;

    switch(fileType) {
    case TEACH:
        // check for right file type by searching for unique header
        searchstring = "Program";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            teachData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) teachData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (teachData[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&teachData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(teachData[i][yrIndex].c_str(), "%4d", &year);
                    teachData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(teachData[i][sortHeaderIndex]), &teachData[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PUBLICATIONS:
        // check for right file type by searching for unique header
        searchstring = "Publication Status";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Status Date");
            pubData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) pubData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (pubData[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&pubData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Status Date");
                    int year;
                    sscanf(pubData[i][yrIndex].c_str(), "%4d", &year);
                    pubData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(pubData[i][sortHeaderIndex]), &pubData[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PRESENTATIONS:
        // check for right file type by searching for unique header
        searchstring = "Activity Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Date");
            presData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j = 0;
            for (int i = 0; i < (int) presData.size(); i++) {
                //check through mandatory MANFIELDS for empty MANFIELDS
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (presData[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&presData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Date");
                    int year;
                    sscanf(presData[i][yrIndex].c_str(), "%4d", &year);
                    presData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(presData[i][sortHeaderIndex]), &presData[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case FUNDING:
        // check for right file type by searching for unique header
        searchstring = "Funding Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            fundData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) fundData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (fundData[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&fundData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(fundData[i][yrIndex].c_str(), "%4d", &year);
                    fundData[i][yrIndex] = std::to_string(year);

                    // boolean interpretation
                    int prIndex = db->getHeaderIndex("Peer Reviewed?");
                    if (fundData[i][prIndex].compare("True") == 0) {
                        fundData[i][prIndex] = "Peer Reviewed";
                    } else {
                        fundData[i][prIndex] = "Not Peer Reviewed";
                    }
                    db->addRecord(reader->parseDateString(fundData[i][sortHeaderIndex]), &fundData[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    }

    return EXIT_SUCCESS;
}

void TestLoadMultipleCSVFiles::test(){
    bool result = EXIT_SUCCESS;
    std::vector<std::string> teachHeader, presHeader, pubHeader, fundHeader;
    CSVReader *teachReader, *presReader, *pubReader, *fundReader;

    RecordsManager *teachdb, *presdb, *pubdb, *funddb;

    /*TeachingTreeModel *teachTree;
    PresentationTreeModel *presTree;
    PublicationTreeModel *pubTree;
    GrantFundingTreeModel *fundTree;*/

    // create a new reader to read in the file
    teachReader = new CSVReader("../Project Information/Sample Data/Teaching_sample.csv");
    teachHeader = teachReader->getHeaders();
    presReader = new CSVReader("../Project Information/Sample Data/Presentations_sample.csv");
    presHeader = presReader->getHeaders();
    pubReader = new CSVReader("../Project Information/Sample Data/Publications_sample.csv");
    pubHeader = pubReader->getHeaders();
    fundReader = new CSVReader("../Project Information/Sample Data/GrantsClinicalFunding_sample.csv");
    fundHeader = fundReader->getHeaders();

    // create a new manager for the data
    teachdb = new RecordsManager(&teachHeader);
    presdb = new RecordsManager(&presHeader);
    pubdb = new RecordsManager(&pubHeader);
    funddb = new RecordsManager(&fundHeader);

    // set up some member variables to point to the current data in use
    /*teachTree = new TeachingTreeModel(teachdb);
    presTree = new PresentationTreeModel(presdb);
    pubTree = new PublicationTreeModel(pubdb);
    fundTree = new GrantFundingTreeModel(funddb);*/

    // validate date trees
    if (validateTree(TEACH, teachReader, teachdb, &teachHeader) != EXIT_SUCCESS) result = EXIT_FAILURE;
    if (validateTree(PRESENTATIONS, presReader, presdb, &presHeader) != EXIT_SUCCESS) result = EXIT_FAILURE;
    if (validateTree(PUBLICATIONS, pubReader, pubdb, &pubHeader) != EXIT_SUCCESS) result = EXIT_FAILURE;
    if (validateTree(FUNDING, fundReader, funddb, &fundHeader) != EXIT_SUCCESS) result = EXIT_FAILURE;

    delete teachReader;
    delete presReader;
    delete pubReader;
    delete fundReader;

    /*delete teachTree;
    delete presTree;
    delete pubTree;
    delete fundTree;*/

    delete teachdb;
    delete presdb;
    delete pubdb;
    delete funddb;

    QVERIFY(result == EXIT_SUCCESS);
}

bool TestMandatoryFieldErrorProcessing::validateTree(int fileType, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header, QWidget *parent){
    std::string searchstring;
    std::vector<std::vector<std::string>> fundData, presData, pubData, teachData;

    int sortHeaderIndex = 2;

    switch(fileType) {
    case TEACH:
        // check for right file type by searching for unique header
        searchstring = "Program";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            teachData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) teachData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (teachData[i][index].compare("") == 0) {
                        f_errs.push_back(&teachData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(teachData[i][yrIndex].c_str(), "%4d", &year);
                    teachData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(teachData[i][sortHeaderIndex]), &teachData[i]);
                }
            }

            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PUBLICATIONS:
        // check for right file type by searching for unique header
        searchstring = "Publication Status";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Status Date");
            pubData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) pubData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (pubData[i][index].compare("") == 0) {
                        f_errs.push_back(&pubData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Status Date");
                    int year;
                    sscanf(pubData[i][yrIndex].c_str(), "%4d", &year);
                    pubData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(pubData[i][sortHeaderIndex]), &pubData[i]);
                }
            }

            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PRESENTATIONS:
        // check for right file type by searching for unique header
        searchstring = "Activity Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Date");
            presData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j = 0;
            for (int i = 0; i < (int) presData.size(); i++) {
                //check through mandatory MANFIELDS for empty MANFIELDS
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (presData[i][index].compare("") == 0) {
                        f_errs.push_back(&presData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Date");
                    int year;
                    sscanf(presData[i][yrIndex].c_str(), "%4d", &year);
                    presData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(presData[i][sortHeaderIndex]), &presData[i]);
                }
            }

            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case FUNDING:
        // check for right file type by searching for unique header
        searchstring = "Funding Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            fundData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) fundData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (fundData[i][index].compare("") == 0) {
                        f_errs.push_back(&fundData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(fundData[i][yrIndex].c_str(), "%4d", &year);
                    fundData[i][yrIndex] = std::to_string(year);

                    // boolean interpretation
                    int prIndex = db->getHeaderIndex("Peer Reviewed?");
                    if (fundData[i][prIndex].compare("True") == 0) {
                        fundData[i][prIndex] = "Peer Reviewed";
                    } else {
                        fundData[i][prIndex] = "Not Peer Reviewed";
                    }
                    db->addRecord(reader->parseDateString(fundData[i][sortHeaderIndex]), &fundData[i]);
                }
            }
            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    }

    return EXIT_SUCCESS;
}

bool TestMandatoryFieldErrorProcessing::handle_field_errors(QWidget *parent, std::vector<std::vector<std::string>*>& err,
                                     std::vector<std::string>& headers,
                                     std::vector<std::string>& mandatory) {
    //Since CSVReader alldata contains completely empty records
    //remove them first.
    std::vector<std::vector<std::string>*>::iterator it;
    for (it = err.begin(); it != err.end(); it++) {
        bool allEmpty = true;
        for (int col = 0; col < (int) (*it)->size(); col++) {
            if ((*it)->at(col).compare("") != 0) {
                allEmpty = false;
            }
        }
        if (allEmpty) {
            it = err.erase(it);
            it--;
        }
    }
    //Return false; there are no errors to correct
    if (err.size() == 0) {
        return false;
    }
    QMessageBox prompt;
    QString mainText = "File contains ";
    mainText.append(QString::number(err.size()));
    mainText.append(" records with missing mandatory MANFIELDS.");
    prompt.setText(mainText);
    prompt.setInformativeText("Do you want to edit these entries or discard?");
    prompt.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    prompt.setDefaultButton(QMessageBox::Yes);
    prompt.setButtonText(QMessageBox::Yes, "Edit");
    prompt.setButtonText(QMessageBox::No, "Discard");
    prompt.setWindowIcon(QIcon(":/icon32.ico"));
    int ret = prompt.exec();

    switch (ret) {
    case QMessageBox::Yes: {
        ErrorEditDialog diag(parent, err, headers, mandatory);
        if(diag.exec()) {
            return true;
        }
        return false;
    }

    case QMessageBox::No:
    default:
        return false;
    }
}

void TestMandatoryFieldErrorProcessing::test(QWidget *parent){
    std::vector<std::string> teachHeader;
    CSVReader *teachReader;

    RecordsManager *teachdb;

    TeachingTreeModel *teachTree;

    // create a new reader to read in the file
    teachReader = new CSVReader("../Project Information/Sample Data/Program_Teaching_expanded.csv");
    teachHeader = teachReader->getHeaders();

    // create a new manager for the data
    teachdb = new RecordsManager(&teachHeader);

    // set up some member variables to point to the current data in use
    teachTree = new TeachingTreeModel(teachdb);

    // validate data trees
    QVERIFY(validateTree(TEACH, teachReader, teachdb, &teachHeader, parent) == EXIT_SUCCESS);

    delete teachTree;
    delete teachdb;
    delete teachReader;
}

void TestWindowsCompatibility::test(){
    /*
    QVERIFY2(IsWindowsXPOrGreater() == true, "Windows XP Compatible");
    QVERIFY2(IsWindowsXPSP1OrGreater() == true, "Windows XP SP1 Compatible");
    QVERIFY2(IsWindowsXPSP2OrGreater() == true, "Windows XP SP2 Compatible");
    QVERIFY2(IsWindowsXPSP3OrGreater() == true, "Windows XP SP3 Compatible");
    QVERIFY2(IsWindowsVistaOrGreater() == true, "Windows Vista Compatible");
    QVERIFY2(IsWindowsVistaSP1OrGreater() == true, "Windows Vista SP1 Compatible");
    QVERIFY2(IsWindowsVistaSP2OrGreater() == true, "Windows Vista SP2 Compatible");
    QVERIFY2(IsWindows7OrGreater() == true, "Windows 7 Compatible");
    QVERIFY2(IsWindows7SP1OrGreater() == true, "Windows 7 SP1 Compatible");
    QVERIFY2(IsWindows8OrGreater() == true, "Windows 8 Compatible");
    QVERIFY2(IsWindows8Point1OrGreater() == true, "Windows 8.1 Compatible");
    //QVERIFY2(IsWindows10OrGreater() == true, "Windows 10 Compatible");
    */
}

void TestUniqueHeaders::test(){
    std::vector<std::string> header[TOTAL_TABS];
    CSVReader *readers[TOTAL_TABS];

    // create a new reader to read in the file
    readers[TEACH] = new CSVReader("../Project Information/Sample Data/Teaching_sample.csv");
    header[TEACH] = readers[TEACH]->getHeaders();
    readers[PRESENTATIONS] = new CSVReader("../Project Information/Sample Data/Presentations_sample.csv");
    header[PRESENTATIONS] = readers[PRESENTATIONS]->getHeaders();
    readers[PUBLICATIONS] = new CSVReader("../Project Information/Sample Data/Publications_sample.csv");
    header[PUBLICATIONS] = readers[PUBLICATIONS]->getHeaders();
    readers[FUNDING] = new CSVReader("../Project Information/Sample Data/GrantsClinicalFunding_sample.csv");
    header[FUNDING] = readers[FUNDING]->getHeaders();

    std::string searchstrings[] = {"Program", "Publication Status", "Activity Type", "Funding Type"};

    for(int x = 0; x < TOTAL_TABS; x++)
    for(int i = 0; i < TOTAL_TABS; i++) {
        if (std::find(header[i].begin(), header[i].end(), searchstrings[x]) != header[i].end()) {
            QVERIFY(i == x);
        }
    }

    for(int i = 0; i < TOTAL_TABS; i++) delete readers[i];
}

bool TestMultiTabs::checkFile(Tab *newTab, QString filePath)
{
    // read if first or if a new file is loaded
    //if (activeTab->path.isEmpty() || (!activeTab->path.isEmpty() && filePath.compare(activeTab->path))) {
        // create a new reader to read in the file
        CSVReader reader = CSVReader(filePath.toStdString());
        std::vector<std::string> header = reader.getHeaders();

        int sortHeaderIndex = 2;

        // create a new manager for the data
        //delete teachdb;
        newTab->db = new RecordsManager(&header);

        // check for right file type by searching for unique header
        std::string searchstring[TOTAL_TAB_TYPES] = {"Program", "Publication Status", "Activity Type", "Funding Type"};
        std::string sortstring[TOTAL_TAB_TYPES] = {"Start Date", "Status Date", "Date", "Start Date"};

        for(int t = 0; t < TOTAL_TAB_TYPES; t++)
        if (std::find(header.begin(), header.end(), searchstring[t]) != header.end()) {
            newTab->fileType = t;

            // load in data into the manager, with the date as the key
            sortHeaderIndex = newTab->db->getHeaderIndex(sortstring[t]);
            newTab->data = reader.getData();
            std::vector<std::vector<std::string>*> f_errs;

            for (int i = 0; i < (int) newTab->data.size(); i++) {
                unsigned int j;
                for (j = 0; j < MANFIELDS[t].size(); j++) {
                    int index = newTab->db->getHeaderIndex(MANFIELDS[t][j]);
                    if (newTab->data[i][index].compare("") == 0) {
                        f_errs.push_back(&newTab->data[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[t].size()) {
                    int /*yrIndex, */year;
                    sscanf(newTab->data[i][sortHeaderIndex].c_str(), "%4d", &year);
                    newTab->data[i][sortHeaderIndex] = std::to_string(year);

                    switch(t) {
                    /*case TEACH_TAB:
                        break;
                    case PUBLICATIONS_TAB:
                        break;
                    case PRESENTATIONS_TAB:
                        break;*/
                    case FUNDING_TAB:
                        // boolean interpretation
                        int prIndex = newTab->db->getHeaderIndex("Peer Reviewed?");
                        if (newTab->data[i][prIndex].compare("True") == 0) {
                            newTab->data[i][prIndex] = "Peer Reviewed";
                        } else {
                            newTab->data[i][prIndex] = "Not Peer Reviewed";
                        }
                        break;
                    }

                    newTab->db->addRecord(reader.parseDateString(newTab->data[i][sortHeaderIndex]), &newTab->data[i]);
                }
            }

            /*if (f_errs.size() > 0) {
                if(handle_field_errors(f_errs, header, MANFIELDS[t])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        newTab->db->addRecord(reader.parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }*/

            //tabs.push_back(newTab);
            //activeTab = newTab;
            break;
        }
    /*}else {
        return EXIT_SUCCESS;
    }*/

    //ui->printButton->setEnabled(true);
    //ui->exportButton->setEnabled(true);

    return EXIT_SUCCESS;
}

void TestMultiTabs::test(){
    bool result = EXIT_SUCCESS;

    Tab *tabs[TOTAL_TAB_TYPES];

    //create new tabs for the csv files
    for(int i = 0; i < TOTAL_TAB_TYPES; i++) tabs[i] = new Tab();

    // validate date trees
    if (checkFile(tabs[TEACH_TAB], "../Project Information/Sample Data/Teaching_sample.csv") != EXIT_SUCCESS) result = EXIT_FAILURE;
    if (checkFile(tabs[PRESENTATIONS_TAB], "../Project Information/Sample Data/Presentations_sample.csv") != EXIT_SUCCESS) result = EXIT_FAILURE;
    if (checkFile(tabs[PUBLICATIONS_TAB], "../Project Information/Sample Data/Publications_sample.csv") != EXIT_SUCCESS) result = EXIT_FAILURE;
    if (checkFile(tabs[FUNDING_TAB], "../Project Information/Sample Data/GrantsClinicalFunding_sample.csv") != EXIT_SUCCESS) result = EXIT_FAILURE;

    for(int i = 0; i < TOTAL_TAB_TYPES; i++) delete tabs[i];

    QVERIFY(result == EXIT_SUCCESS);
}


bool TestPrevNextMissingFields::validateTree(int fileType, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header, QWidget *parent){
    std::string searchstring;
    std::vector<std::vector<std::string>> fundData, presData, pubData, teachData;

    int sortHeaderIndex = 2;

    switch(fileType) {
    case TEACH:
        // check for right file type by searching for unique header
        searchstring = "Program";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            teachData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) teachData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (teachData[i][index].compare("") == 0) {
                        f_errs.push_back(&teachData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(teachData[i][yrIndex].c_str(), "%4d", &year);
                    teachData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(teachData[i][sortHeaderIndex]), &teachData[i]);
                }
            }

            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PUBLICATIONS:
        // check for right file type by searching for unique header
        searchstring = "Publication Status";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Status Date");
            pubData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) pubData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (pubData[i][index].compare("") == 0) {
                        f_errs.push_back(&pubData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Status Date");
                    int year;
                    sscanf(pubData[i][yrIndex].c_str(), "%4d", &year);
                    pubData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(pubData[i][sortHeaderIndex]), &pubData[i]);
                }
            }

            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PRESENTATIONS:
        // check for right file type by searching for unique header
        searchstring = "Activity Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Date");
            presData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j = 0;
            for (int i = 0; i < (int) presData.size(); i++) {
                //check through mandatory MANFIELDS for empty MANFIELDS
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (presData[i][index].compare("") == 0) {
                        f_errs.push_back(&presData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Date");
                    int year;
                    sscanf(presData[i][yrIndex].c_str(), "%4d", &year);
                    presData[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(presData[i][sortHeaderIndex]), &presData[i]);
                }
            }

            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case FUNDING:
        // check for right file type by searching for unique header
        searchstring = "Funding Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            fundData = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) fundData.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int index = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (fundData[i][index].compare("") == 0) {
                        f_errs.push_back(&fundData[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(fundData[i][yrIndex].c_str(), "%4d", &year);
                    fundData[i][yrIndex] = std::to_string(year);

                    // boolean interpretation
                    int prIndex = db->getHeaderIndex("Peer Reviewed?");
                    if (fundData[i][prIndex].compare("True") == 0) {
                        fundData[i][prIndex] = "Peer Reviewed";
                    } else {
                        fundData[i][prIndex] = "Not Peer Reviewed";
                    }
                    db->addRecord(reader->parseDateString(fundData[i][sortHeaderIndex]), &fundData[i]);
                }
            }
            if (f_errs.size() > 0) {
                if(handle_field_errors(parent, f_errs, *header, MANFIELDS[fileType])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        db->addRecord(reader->parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    }

    return EXIT_SUCCESS;
}

bool TestPrevNextMissingFields::handle_field_errors(QWidget *parent, std::vector<std::vector<std::string>*>& err,
                                     std::vector<std::string>& headers,
                                     std::vector<std::string>& mandatory) {
    //Since CSVReader alldata contains completely empty records
    //remove them first.
    std::vector<std::vector<std::string>*>::iterator it;
    for (it = err.begin(); it != err.end(); it++) {
        bool allEmpty = true;
        for (int col = 0; col < (int) (*it)->size(); col++) {
            if ((*it)->at(col).compare("") != 0) {
                allEmpty = false;
            }
        }
        if (allEmpty) {
            it = err.erase(it);
            it--;
        }
    }
    //Return false; there are no errors to correct
    if (err.size() == 0) {
        return false;
    }
    QMessageBox prompt;
    QString mainText = "File contains ";
    mainText.append(QString::number(err.size()));
    mainText.append(" records with missing mandatory MANFIELDS.");
    prompt.setText(mainText);
    prompt.setInformativeText("Do you want to edit these entries or discard?");
    prompt.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    prompt.setDefaultButton(QMessageBox::Yes);
    prompt.setButtonText(QMessageBox::Yes, "Edit");
    prompt.setButtonText(QMessageBox::No, "Discard");
    prompt.setWindowIcon(QIcon(":/icon32.ico"));
    int ret = prompt.exec();

    switch (ret) {
    case QMessageBox::Yes: {
        ErrorEditDialog diag(parent, err, headers, mandatory);
        if(diag.exec()) {
            return true;
        }
        return false;
    }

    case QMessageBox::No:
    default:
        return false;
    }
}

void TestPrevNextMissingFields::test(QWidget *parent){
    std::vector<std::string> teachHeader;
    CSVReader *teachReader;

    RecordsManager *teachdb;

    TeachingTreeModel *teachTree;

    // create a new reader to read in the file
    teachReader = new CSVReader("../Project Information/Sample Data/Program_Teaching_expanded.csv");
    teachHeader = teachReader->getHeaders();

    // create a new manager for the data
    teachdb = new RecordsManager(&teachHeader);

    // set up some member variables to point to the current data in use
    teachTree = new TeachingTreeModel(teachdb);

    // validate data trees
    QVERIFY(validateTree(TEACH, teachReader, teachdb, &teachHeader, parent) == EXIT_SUCCESS);

    delete teachTree;
    delete teachdb;
    delete teachReader;
}

void TestTeachingDivisionSortField::test(){
    CSVReader reader("../Project Information/Sample Data/Program_Teaching_expanded.csv");
    vector<string> header = reader.getHeaders();

    bool flag = false;

    vector<string>::iterator it;
    for(it = header.begin(); it != header.end(); ++it)
    if((*it) == "Division") flag = true;

    QVERIFY(flag == true);
}

//d4
void TestSaveSessionState::test(){

    std::ofstream fileIn("saveSession.txt");

    QVERIFY(fileIn.is_open());

    if (fileIn.is_open()){
        fileIn.close();
    }
    else{
        //std::cout<<"the file wasn't open."<<endl;
    }
}

void TestLoadSessionState::test(){

    std::ifstream fileOut("saveSession.txt");

    QVERIFY(fileOut.is_open());

    if (fileOut.is_open()){
        fileOut.close();
    }
    else{
        //std::cout<<"the file wasn't open."<<endl;
    }
}


bool TestSavingTabToCSV::validateTree(int fileType, CSVReader *reader, RecordsManager *db, std::vector<std::string> *header){
    std::string searchstring;
    std::vector<std::vector<std::string>> data;
    int sortHeaderIndex = 2;

    switch(fileType) {
    case TEACH:
        // check for right file type by searching for unique header
        searchstring = "Program";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            data = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) data.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (data[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&data[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(data[i][yrIndex].c_str(), "%4d", &year);
                    data[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(data[i][sortHeaderIndex]), &data[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PUBLICATIONS:
        // check for right file type by searching for unique header
        searchstring = "Publication Status";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Status Date");
            data = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) data.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (data[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&data[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Status Date");
                    int year;
                    sscanf(data[i][yrIndex].c_str(), "%4d", &year);
                    data[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(data[i][sortHeaderIndex]), &data[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case PRESENTATIONS:
        // check for right file type by searching for unique header
        searchstring = "Activity Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Date");
            data = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j = 0;
            for (int i = 0; i < (int) data.size(); i++) {
                //check through mandatory MANFIELDS for empty MANFIELDS
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (data[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&data[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Date");
                    int year;
                    sscanf(data[i][yrIndex].c_str(), "%4d", &year);
                    data[i][yrIndex] = std::to_string(year);

                    db->addRecord(reader->parseDateString(data[i][sortHeaderIndex]), &data[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    case FUNDING:
        // check for right file type by searching for unique header
        searchstring = "Funding Type";

        if (std::find(header->begin(), header->end(), searchstring) != header->end()) {
            // load in data into the manager, with the date as the key
            sortHeaderIndex = db->getHeaderIndex("Start Date");
            data = reader->getData();
            std::vector<std::vector<std::string>*> f_errs;
            unsigned int j;
            for (int i = 0; i < (int) data.size(); i++) {
                for (j = 0; j < MANFIELDS[fileType].size(); j++) {
                    int headerIndex = db->getHeaderIndex(MANFIELDS[fileType][j]);
                    if (data[i][headerIndex].compare("") == 0) {
                        f_errs.push_back(&data[i]);
                        break;
                    }
                }

                // if all mandatory MANFIELDS are okay
                if (j == MANFIELDS[fileType].size()) {
                    // date interpretation
                    int yrIndex = db->getHeaderIndex("Start Date");
                    int year;
                    sscanf(data[i][yrIndex].c_str(), "%4d", &year);
                    data[i][yrIndex] = std::to_string(year);

                    // boolean interpretation
                    int prIndex = db->getHeaderIndex("Peer Reviewed?");
                    if (data[i][prIndex].compare("True") == 0) {
                        data[i][prIndex] = "Peer Reviewed";
                    } else {
                        data[i][prIndex] = "Not Peer Reviewed";
                    }
                    db->addRecord(reader->parseDateString(data[i][sortHeaderIndex]), &data[i]);
                }
            }
        } else {
            return EXIT_FAILURE;
        }
        break;
    }

    return EXIT_SUCCESS;
}

void TestSavingTabToCSV::test(){

    bool result = EXIT_SUCCESS;
    std::vector<std::string> teachHeader;
    CSVReader *teachReader;
    RecordsManager *teachdb;
    TeachingTreeModel *teachTree;
    int loadDataSize, saveDataSize;

    // create a new reader to read in the file
    teachReader = new CSVReader("../Project Information/Sample Data/Teaching_sample.csv");
    teachHeader = teachReader->getHeaders();

    // create a new manager for the data
    teachdb = new RecordsManager(&teachHeader);

    // set up some member variables to point to the current data in use
    teachTree = new TeachingTreeModel(teachdb);

    // validate date trees
    if (validateTree(TEACH, teachReader, teachdb, &teachHeader) != EXIT_SUCCESS) result = EXIT_FAILURE;

    loadDataSize = (int)teachReader->getData().size();

    std::ofstream fileout;

    if (result == EXIT_SUCCESS) {
        QString fileName = QFileDialog::getSaveFileName(0,
               QObject::tr("Save database file"), QDir::currentPath(),
               QObject::tr("CSV (*.csv);;All Files (*)"));

        //give the user an option to save it to a specific filepath (replacement or overwrite
        fileout.open(fileName.toStdString(), std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);

        if (fileout.is_open()){
            BasicRecord headers = teachdb->getHeaders();
            for (BasicRecord::iterator it = headers.begin(); it != headers.end(); ++it){
                if (it == headers.end()-1)
                    fileout << '"' << *it << '"' << "\n";
                else fileout << '"' << *it << '"' << ",";
            }

            std::vector<std::vector<std::string>> data = teachReader->getData();

            for (std::vector<std::vector<std::string>>::iterator it = data.begin(); it != data.end(); ++it){
                for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2){
                    if (it2 == it->end()-1)
                        fileout << '"' << *it2 << '"' << "\n";
                    else fileout << '"' << *it2 << '"' << ",";
                }
            }
            fileout.close();
            //std::cout<<"writing to file: successful"<<std::endl;

            delete teachReader;
            delete teachTree;
            delete teachdb;

            // create a new reader to read in the file
            teachReader = new CSVReader(fileName.toStdString());
            teachHeader = teachReader->getHeaders();

            // create a new manager for the data
            teachdb = new RecordsManager(&teachHeader);

            // set up some member variables to point to the current data in use
            teachTree = new TeachingTreeModel(teachdb);

            // validate date trees
            if (validateTree(TEACH, teachReader, teachdb, &teachHeader) != EXIT_SUCCESS) result = EXIT_FAILURE;

            saveDataSize = (int)teachReader->getData().size();
        }
        else{
            //std::cout<<"couldn't open file, didn't save to file."<<std::endl;
            result = EXIT_FAILURE;

            delete teachReader;
            delete teachTree;
            delete teachdb;
        }
    }else {
        delete teachReader;
        delete teachTree;
        delete teachdb;
    }

    QVERIFY(result == EXIT_SUCCESS && loadDataSize == saveDataSize);
}
