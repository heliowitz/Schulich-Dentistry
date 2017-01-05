#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    activeTab = NULL;

    // draw GUI elements
    ui->setupUi(this);

    this->showMaximized();

    // set up application icon
    QIcon icon(":/images/icon32.ico");
    setWindowIcon(icon);

    loadSessionState();

    if (tabs.size()==0) {
        Mainscreen *ms = new Mainscreen();
        ui->categoryTab->addTab(ms, "Welcome!");
        ui->categoryTab->tabBar()->tabButton(0, QTabBar::RightSide)->setVisible(false);
        ui->categoryTab->tabBar()->tabButton(0, QTabBar::RightSide)->setEnabled(false);
        //ui->categoryTab->tabBar()->tabButton(0, QTabBar::LeftSide)->resize(0, 0);
//        tabs.push_back(ms);
    }
    else{
        helpscreen *hs = new helpscreen();
        ui->categoryTab->insertTab(0, hs, "Help");
        ui->categoryTab->tabBar()->tabButton(0, QTabBar::RightSide)->setEnabled(false);
        ui->categoryTab->tabBar()->tabButton(0, QTabBar::RightSide)->setVisible(false);
//        tabs.insert(tabs.begin(), ms);
    }

    dateChanged = {false, false, false, false};
}

MainWindow::~MainWindow() {
    delete ui;

    for(std::vector<Tab*>::iterator it = tabs.begin(); it != tabs.end(); ++it)
        delete (*it);
    tabs.clear();

    activeTab = NULL;
}

void MainWindow::on_actionLoad_file_triggered() {
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          "Select one or more files to load",
                                                          QDir::currentPath(),
                                                          tr("CSV (*.csv);; All files (*.*)"));
    if (filePaths.size() > 0) {
        for ( QStringList::Iterator it = filePaths.begin(); it != filePaths.end(); ++it) {
            load(*it, true);
        }
    }
    if (ui->categoryTab->tabText(0) == "Welcome!"){
        ui->categoryTab->removeTab(0);
//        tabs.erase(tabs.begin());
        helpscreen *hs = new helpscreen();
        ui->categoryTab->insertTab(0, hs, "Help");
        ui->categoryTab->tabBar()->tabButton(0, QTabBar::RightSide)->setEnabled(false);
        ui->categoryTab->tabBar()->tabButton(0, QTabBar::RightSide)->setVisible(false);
//        tabs.insert(tabs.begin(), ms);
    }
}

/*
 * When the load file button is clicked a file open dialog box
 * is opened.  If a file name is successfully returned, makeTree()
 * is called.
 */
QString MainWindow::load_file() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(),
                                                    tr("CSV (*.csv);; All files (*.*)"));

    if (!filePath.isEmpty()) {
        return filePath;
    } else {
        return "";
    }
}
/**
 * @brief MainWindow::closeEvent
 * @param event
 * This function prompts the user if they want to save the session state before closing
 */
void MainWindow::closeEvent (QCloseEvent *event){

    for(std::vector<Tab*>::iterator it = tabs.begin(); it != tabs.end(); ++it) {


        string csvname;
        stringstream path((*it)->path.toStdString());
        for(csvname;getline(path, csvname, '/');)
            ;
        string message = "Would you like to save " + csvname + " to a new CSV file?\n";
        QMessageBox::StandardButton rstBtn = QMessageBox::question(this, "TeamAndromeda" ,tr(message.c_str()),
                                                                   QMessageBox::Yes | QMessageBox::No);
        if (rstBtn == QMessageBox::Yes) {
             //TODO: Add code here to save the session state.
             //saveSession(); or CSVReader object that saves...
             (*it)->saveToNewCSV(this);
         } else { //when the user selects no.
            //close program without saving session anyways
         }
    }
    saveSessionState();
    event->accept();
}

void MainWindow::refresh() {
    if (activeTab == NULL) return;

    makeTree();

    activeTab->refresh();
}
/**
 * @brief MainWindow::saveSessionState
 * This method saves the following things:
 * Selected Sort -> Tab
    Dates -> MainWindow
    Currently Selected Rows -> Tab
 */
void MainWindow::saveSessionState(){

    std::ofstream fileIn("saveSession.txt");
    if (fileIn.is_open()){
        for(std::vector<Tab*>::iterator it = tabs.begin(); it != tabs.end(); ++it) {

            Tab* tab = (*it);

            //get fields to save
            int startYear = (*it)->ui->FromDate->date().year();
            int endYear = (*it)->ui->ToDate->date().year();
            string filterFrom = (*it)->ui->filter_from->text().toStdString();
            string filterTo = (*it)->ui->filter_to->text().toStdString();
            int sortIndex = (*it)->ui->sort->currentIndex();

            fileIn << tab->path.toStdString() << ',' << startYear << ',' << endYear <<
                   ',' << filterFrom << ',' << filterTo << ',' << sortIndex << endl;
        }
        fileIn.close();
    }
    else{
        //std::cout<<"the file wasn't open."<<endl;
    }

}
void MainWindow::loadSessionState(){
    std::ifstream fileOut("saveSession.txt");

    vector<vector<string>> loaded_data;

    //load strings into 2d vector
    if(fileOut.is_open()){
        for(string line; getline(fileOut, line);){
            loaded_data.push_back(vector<string>{});
            stringstream sline(line);
            for(string item; getline(sline, item, ',');){
                loaded_data.back().push_back(item);
            }
        }
    }
    fileOut.close();
    //for each row, build the saved tab
    for(int i = 0; i < loaded_data.size(); i++){
        //check data has correct number of fields
        if(loaded_data.at(i).size() != 6)
            continue;

        //load saved fields
        QString path = QString::fromStdString(loaded_data.at(i).at(0));
        QString startYear = QString::fromStdString(loaded_data.at(i).at(1));
        QString endYear = QString::fromStdString(loaded_data.at(i).at(2));
        QString filterFrom = QString::fromStdString(loaded_data.at(i).at(3));
        QString filterTo = QString::fromStdString(loaded_data.at(i).at(4));
        int sortIndex = stoi(loaded_data.at(i).at(5));

        //attempt to load file
        if(load(path)){
            QDate newDate = QDate::fromString(startYear,"yyyy");
            QDate newEndDate = QDate::fromString(endYear,"yyyy");

            //set tab fields after it has been loaded
            tabs.back()->ui->FromDate->setDate(newDate);
            tabs.back()->ui->ToDate->setDate(newEndDate);
            tabs.back()->ui->filter_from->setText(filterFrom);
            tabs.back()->ui->filter_to->setText(filterTo);
            tabs.back()->ui->sort->setCurrentIndex(sortIndex);
        }

    }

    refresh();
    dateChanged = {true, true, true, true};

}

int MainWindow::checkFile(QString filePath)
{
    Tab *newTab = new Tab();

    // read if first or if a new file is loaded
    // TODO: need to change this if statement to check all tabs with the same path name. Not just the current active tab.
    if (activeTab == NULL || activeTab->path.isEmpty() || (!activeTab->path.isEmpty() && filePath.compare(activeTab->path))) {
        // create a new reader to read in the file
        CSVReader reader = CSVReader(filePath.toStdString());

        if(!reader.fileOpen){return EXIT_FAILURE;}

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
                    if (index >= newTab->data[i].size() || newTab->data[i][index].compare("") == 0) {
                        f_errs.push_back(&newTab->data[i]);
                        break;
                    }
                }

                // if all mandatory fields are okay
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

            if (f_errs.size() > 0) {
                //dynamically adding records
                if(handle_field_errors(f_errs, header, MANFIELDS[t])) {
                    for (unsigned int i = 0; i < f_errs.size(); i++) {
                        newTab->db->addRecord(reader.parseDateString((*(f_errs[i]))[sortHeaderIndex]), f_errs[i]);
                    }
                }
            }

            tabs.push_back(newTab);
            break;
        }
    }else {
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

void MainWindow::createDefaultSortOrder(Tab *tab) {
    QStringList defaultOrder;

    defaultOrder << "Default";

    // specify default sort order
    for(unsigned int i = 0; i < MANFIELDS[tab->fileType].size(); i++)
        defaultOrder << MANFIELDS[tab->fileType][i].c_str();

    // add default list to member variable
    tab->allOrders << defaultOrder;

    // save the default for the user
    QSortListIO saveSort(ORDERS_SAVE[tab->fileType]);
    saveSort.saveList(tab->allOrders);
}

/*
 * err: vector of pointers to record entries.
 * headers: vector of strings containing the db headers
 * mandatory: vector of strings containing the mandatory fields in the db
 *
 * Returns true when the passed err vector contains edited fields.
 * Returns false when no fields have been edited.
 */
bool MainWindow::handle_field_errors(std::vector<std::vector<std::string>*>& err,
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
    mainText.append(" records with missing mandatory fields.");
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
        ErrorEditDialog diag(this, err, headers, mandatory);
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

/*
 * Builds a TreeModel for the ui's embedded treeview.
 */
void MainWindow::makeTree() {
    // set up some member variables to point to the current data in use
    //currentdb = activeTab->db;
    //delete activeTab->tree;


        activeTab->tree = new TabTreeModel(activeTab->fileType, activeTab->db);
        //currentTree = teachTree;

        // get some UI elements so we can put the tree and graphs into them
        //currentView = ui->treeView;
        currentView = activeTab->ui->treeView;

        // analyze the data into a tree
        activeTab->tree->setupModel(activeTab->yearStart, activeTab->yearEnd, activeTab->sortOrder, activeTab->getFilterStartChar(), activeTab->getFilterEndChar());

        activeTab->ui->pie_button->toggle();

        // set up the tree in our UI
        currentView->setModel(activeTab->tree);

        // set resize property to stretch
        currentView->header()->resizeSections(QHeaderView::Stretch);
}

/*void MainWindow::on_load_file_clicked() {
    QString path = load_file();
    if (!path.isEmpty()) {
        load(path);
    }
}
*/


//void MainWindow::on_FromDate_dateChanged(const QDate &date) {
//    // set the member variable to the new date
//    if (activeTab != NULL) activeTab->yearStart = date.year();


//    // update end date spinbox to not fall below that year
//    ui->ToDate->setMinimumDate(date);

//    dateChanged = {true, true, true, true};

//    // refresh the GUI
//    refresh();
//}

//void MainWindow::on_ToDate_dateChanged(const QDate &date) {
//    // set the member variable to the new date
//    if (activeTab != NULL) activeTab->yearEnd = date.year();

//    // update end date spinbox to not fall below that year
//    ui->ToDate->setMinimumDate(ui->FromDate->date());

//    dateChanged = {true, true, true, true};

//    // refresh the GUI
//    refresh();
//}

void MainWindow::on_categoryTab_currentChanged() {
    /*qFatal( "Tab went beyond index" );
    if (dateChanged[ui->categoryTab->currentIndex()] == true) {
        refresh();
        dateChanged[ui->categoryTab->currentIndex()] = false;
    }*/
}

bool MainWindow::load(QString path, bool multi_file) {
    Tab *newTab = NULL;

    if (checkFile(path) == EXIT_SUCCESS) {
        newTab = tabs.back();
        if (newTab == NULL) return false;
        activeTab = NULL; // prevents call to refresh() and makeTree() when updating FromDate/ToDate dates

        newTab->path = path;

        // enable gui elements
        newTab->ui->sort->setEnabled(true);
        newTab->ui->delete_sort->setEnabled(true);
        newTab->ui->new_sort->setEnabled(true);
        newTab->ui->edit_sort->setEnabled(true);
        newTab->ui->filter_from->setEnabled(true);
        newTab->ui->filter_to->setEnabled(true);
        newTab->ui->pie_button->setEnabled(true);
        newTab->ui->bar_button->setEnabled(true);
//        newTab->ui->to_label->setEnabled(true);
//        newTab->ui->sort_label->setEnabled(true);
        newTab->ui->filter->setEnabled(true);
        newTab->ui->filter_label->setEnabled(true);
        newTab->ui->printButton->setEnabled(true);
        newTab->ui->exportButton->setEnabled(true);

        // load save order
        QSortListIO saveOrder(ORDERS_SAVE[newTab->fileType]);
        newTab->allOrders = saveOrder.readList();
        newTab->ui->sort->clear();
        for (int i = 0; i < newTab->allOrders.size(); i++) {
            newTab->ui->sort->addItem(newTab->allOrders.at(i).at(0));
        }

        // create default sort order if none are loaded
        if (newTab->ui->sort->currentIndex() < 0) {
            createDefaultSortOrder(newTab);
            newTab->ui->sort->addItem(newTab->allOrders[0][0]);
        }



//        QDate tmpDate;

//        tmpDate.setDate(newTab->yearStart, 1, 1);
//        ui->FromDate->setDate(tmpDate);

//        tmpDate.setDate(newTab->yearEnd, 1, 1);
//        ui->ToDate->setDate(tmpDate);

//        newTab->ui->file_label->setText(newTab->path);

        ui->categoryTab->addTab(newTab, newTab->path);

        activeTab = newTab;

        // create the tree
        makeTree();

        ui->categoryTab->setCurrentIndex(tabs.size() - 1);
        ui->categoryTab->setCurrentWidget(activeTab);
        newTab->refresh();

        return true;
    } else {
        //if (!multi_file) {
            QMessageBox::critical(this, "Invalid File", "Not a valid database file.");
            //on_load_file_clicked();
        //}
    }
    return false;
}

/*void MainWindow::on_toolButton_clicked()
{
    // Add tab
    ui->categoryTab->addTab(new Tab(), "NEW TAB");

    // TEMP OLD
//    QString path = load_file();
//    if (!path.isEmpty()) {
//        load_teach(path);
//    }
}
*/

void MainWindow::on_categoryTab_tabCloseRequested(int index)
{
        // add code here to save the different CSV file
        index--;
        Tab *tab = tabs.at(index);

        //Save to a new CSV File
        //const QString &title = "TeamAndromeda";

        string csvname;
        stringstream path(tab->path.toStdString());
        for(csvname;getline(path, csvname, '/');)
            ;
        string message = "Would you like to save " + csvname + " to a new CSV file?\n";
        QMessageBox::StandardButton rstBtn = QMessageBox::question(this, "TeamAndromeda" ,tr(message.c_str()),
                                                                   QMessageBox::Yes | QMessageBox::No);
        if (rstBtn == QMessageBox::Yes) {
             //TODO: Add code here to save the session state.
             //saveSession(); or CSVReader object that saves...
             tab->saveToNewCSV(this);
             //saveSessionForTabs();
         } else { //when the user selects no.
            //close program without saving session anyways
            //saveSessionForTabs();
         }


        tabs.erase(tabs.begin()+index);
        ui->categoryTab->removeTab(index+1);

        if(tabs.empty()){
            activeTab = NULL;
        } else {
            activeTab = tabs.at(0);
        }
//        if (tabs.size()==0) {
//            Mainscreen *ms = new Mainscreen();
//            ui->categoryTab->addTab(ms, "Welcome!");
//        }


}
