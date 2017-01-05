#include "tab.h"
#include <QMessageLogger>

Tab::Tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab)
{
    ui->setupUi(this);

    tree = NULL;
    db = NULL;

    // set some member variables to the current date values
    yearStart = 1950;
    yearEnd = QDate::currentDate().year();

    // set start year to be 1950
    QDate startDate(1950, 1, 1);

    // set the date format
    QString dFormat("yyyy");
    ui->FromDate->setDisplayFormat(dFormat);
    ui->ToDate->setDisplayFormat(dFormat);

    // set ranges for spinbox
    ui->FromDate->setDateRange(startDate, QDate::currentDate());
    ui->ToDate->setDateRange(startDate, QDate::currentDate());

    // set default dates (1950->current)
    ui->FromDate->setDate(startDate);
    ui->ToDate->setDate(QDate::currentDate());


    }

Tab::~Tab()
{
    if (tree) delete tree;
    if (db) delete db;

    delete ui;
}

void Tab::on_sort_currentIndexChanged(int index)
{
    if(index != -1) {
        QStringList sortOrder = allOrders[index];
        this->sortOrder.clear();
        for (int i = 1; i < sortOrder.size(); i++) {
            this->sortOrder.emplace_back(sortOrder[i].toStdString());
        }
        ui->filter->setText(QString::fromStdString(this->sortOrder[0]));
        refresh();
    }
}

void Tab::refresh()
{
    ui->graph_stackedWidget->hide();
    ui->graphTitle->clear();
    if (this->sortOrder.size() > 0) {
        makeTree();
    }
}

/*
 * Builds a TreeModel for the ui's embedded treeview.
 */
void Tab::makeTree() {
    // set up some member variables to point to the current data in use
    //currentdb = activeTab->db;
    //delete activeTab->tree;

    this->tree = new TabTreeModel(this->fileType, this->db);
    //currentTree = teachTree;

    // get some UI elements so we can put the tree and graphs into them
    //currentView = ui->treeView;
    currentView = this->ui->treeView;

    // analyze the data into a tree
    this->tree->setupModel(this->yearStart, this->yearEnd, this->sortOrder, this->getFilterStartChar(), this->getFilterEndChar());

    this->ui->pie_button->toggle();

    // set up the tree in our UI
    currentView->setModel(this->tree);

    // set resize property to stretch
    currentView->header()->resizeSections(QHeaderView::Stretch);

    //create selectionChanged slot for treeview
    QItemSelectionModel* selectionModel = ui->treeView->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(on_treeView_SelectionChanged(const QItemSelection&,const QItemSelection&)));

}

void Tab::on_delete_sort_clicked()
{
    if (ui->sort->currentIndex()!=0) {
        QMessageBox prompt;
        prompt.setText("Are you sure you want to delete " + ui->sort->currentText() + "?");
        prompt.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        if (prompt.exec()==QMessageBox::Yes) {
            allOrders.removeAt(ui->sort->currentIndex());
            ui->sort->removeItem(ui->sort->currentIndex());

            QSortListIO saveSort(ORDERS_SAVE[fileType]);
            saveSort.saveList(allOrders);
        }
    } else {
        QMessageBox::critical(this, "", "Cannot delete Default");
    }
}

void Tab::on_pie_button_toggled(bool checked)
{
    if (checked) ui->graph_stackedWidget->setCurrentIndex(0);
}

void Tab::on_bar_button_toggled(bool checked)
{
    if (checked) ui->graph_stackedWidget->setCurrentIndex(2);
}

void Tab::on_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export File"),
                                                    QDir::homePath(),
                                                    tr("PDF (*.pdf)"));
    if (fileName.contains("") != 0) {
        QPdfWriter writer(fileName);
        writer.setPageOrientation(QPageLayout::Landscape);
        QPainter painter;
        painter.begin(&writer);
        painter.scale(10.0, 10.0);
        ui->chartFrame->render(&painter);
        /* Another option for bar chart since it is QCustom plot
        if (ui->teach_bar_button->isChecked()) {
            ui->teachBarChart->savePdf(fileName);
        } */
    }
}

void Tab::on_filter_from_textChanged(const QString &arg1)
{
    refresh();
}

void Tab::on_filter_to_textChanged(const QString &arg1)
{
    refresh();
}

void Tab::on_printButton_clicked()
{
    QPrinter printer;

        QPrintDialog *printDialog = new QPrintDialog(&printer, this);
        printDialog->setWindowTitle(tr("Print Visual Metric"));

        if (printDialog->exec() == QDialog::Accepted) {
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFileName("print.ps");
            QPainter painter;
            painter.begin(&printer);

            double xscale = printer.pageRect().width()/double(ui->chartFrame->width());
            double yscale = printer.pageRect().height()/double(ui->chartFrame->height());
            double scale = qMin(xscale, yscale);
            painter.translate(printer.paperRect().x(),
                               printer.paperRect().y());
            painter.scale(scale, scale);

            ui->chartFrame->render(&painter);
            painter.end();
        }

        delete printDialog;
}

//This method sorts the data by a user selected list.
void Tab::on_treeView_SelectionChanged(const QItemSelection&,const QItemSelection&)
{
    //Initialize variables
    QModelIndexList listOfIndexes = ui->treeView->selectionModel()->selectedRows(0);
    std::vector<std::string> parentsList;
    std::vector<std::string> namesList;
    QModelIndex current;
    QString name;
    std::vector<std::pair <std::string, double>> chartList;
    std::vector<std::pair <std::string, double>> chartDateList;
    std::vector<std::pair <std::string, std::vector<double>>> chartGroupedList; // category, value
    std::vector<std::string> chartListString;
    std::vector<double> chartListDouble;
    std::vector<std::string> chartDateListString;
    std::vector<double> chartDateListDouble;
    int level = sortOrder.size();
    int tempLevel = 0;

    //Check for empty index list or level too low
    if(listOfIndexes.count() == 0){return;}


    //we loop here to ensure the data is at the same tree level. This confirms all the data can be compared
    for(int j = 0; j < listOfIndexes.count(); j++)
    {

        tempLevel = 0;
        current = listOfIndexes.at(j);

        while (true)
        {
            name = current.data(Qt::DisplayRole).toString();
            if(name!="")
            {
            }
            else
            {
                break;
            }
            current = current.parent();
            tempLevel++;
        }
        //Use the lowest common level.
        if (tempLevel < level)
        {
            level = tempLevel;
        }
    }

    if(level == sortOrder.size()){
        return;
    }
    std::vector<std::string> tempSortOrder(sortOrder.begin(), sortOrder.begin()+level+1);
    std::vector<std::string> tempDateSortOrder(sortOrder.begin(), sortOrder.begin()+level+1);
    std::string dateHeader = tempSortOrder[1];
    if(tempSortOrder[1].find("Date") != std::string::npos){
        tempDateSortOrder = tempSortOrder;
    } else {
        for(int pos = 0; pos < sortOrder.size(); pos++)
            if(sortOrder[pos].find("Date") != std::string::npos)
                dateHeader = sortOrder[pos];
        tempDateSortOrder[1] = dateHeader;
    }

    //we loop here to insert the data into a list that the graphs can interpret.
    for (int i = 0; i < listOfIndexes.count(); i++)
    {
        parentsList.empty();
        current = listOfIndexes.at(i);

        while (true)
        {
            name = current.data(Qt::DisplayRole).toString();
            if(name!="")
            {
                auto it = parentsList.begin();
                it = parentsList.insert(it, name.toStdString());
                if(current.column() == 0){
                    namesList.push_back(name.toStdString());
                }
            }
            else
            {
                break;
            }
            current = current.parent();
         }

        //only select data that can be compared
        while (level < parentsList.size())
        {
            parentsList.pop_back();
        }

        if (parentsList.size()!=sortOrder.size())
        {
            std::vector<std::pair <std::string, double>> list;
            std::vector<std::pair <std::string, double>> dateList;

            if(fileType == FUNDING_TAB){
                list = db->getTotalsTuple(yearStart, yearEnd, tempSortOrder, parentsList, "Total Amount", getFilterStartChar(), getFilterEndChar());
                dateList = db->getTotalsTuple(yearStart, yearEnd, tempDateSortOrder, parentsList, "Total Amount", getFilterStartChar(), getFilterEndChar());
            } else{
                list = db->getCountTuple(yearStart, yearEnd, tempSortOrder, parentsList, getFilterStartChar(), getFilterEndChar());
                dateList = db->getCountTuple(yearStart, yearEnd, tempDateSortOrder, parentsList, getFilterStartChar(), getFilterEndChar());
            }



            //we loop here to add to the list
            for (int h = 0; h < (int) list.size(); h++)
            {
                bool flag = false;

                for(int i = 0; i < chartGroupedList.size(); i++)
                {
                    if (strcmp(chartGroupedList[i].first.c_str(), list[h].first.c_str()) == 0) {
                        chartGroupedList[i].second.push_back(list[h].second);
                        flag = true;
                        break;
                    }
                }

                if (!flag)
                {
                    std::vector<double> doubleList;
                    doubleList.clear();
                    doubleList.push_back(list[h].second);
                    chartGroupedList.emplace_back(list[h].first, doubleList);
                }

                flag = false;
                for(int x = 0; x < chartListString.size(); x++)
                {
                    if (strcmp(chartListString[x].c_str(), list[h].first.c_str()) == 0) {
                        chartListDouble[x] += list[h].second;
                        flag = true;
                    }
                }

                if (!flag)
                {
                    chartListString.push_back(list[h].first);
                    chartListDouble.push_back(list[h].second);
                }
            }

            //we loop here to add to the date list
            for (int h = 0; h < (int) dateList.size(); h++){
                int index = chartDateListString.size() == 0 ? 0 : -1;
                bool flag = false;
                for(int x = 0; x < chartDateListString.size(); x++)
                {
                    if (std::stoi(chartDateListString[x].c_str()) == std::stoi(dateList[h].first.c_str())) {
                        chartDateListDouble[x] += dateList[h].second;
                        flag = true;
                        break;
                    }
                    if(!flag && index == -1 && std::stoi(chartDateListString[x].c_str()) > std::stoi(dateList[h].first.c_str())){
                        index = x;
                    }
                }
                if(!flag){
                    if (index > -1)
                    {
                        chartDateListString.insert(chartDateListString.begin() + index, dateList[h].first);
                        chartDateListDouble.insert(chartDateListDouble.begin() + index, dateList[h].second);
                    } else{
                        chartDateListString.push_back(dateList[h].first);
                        chartDateListDouble.push_back(dateList[h].second);
                    }
                }
            }
        }
    }
   //we loop here to add our data from our list created above to the graph
  for(int n = 0; n < chartListString.size(); n++)
      chartList.emplace_back(chartListString[n], chartListDouble[n]);
  for(int n = 0; n < chartDateListString.size(); n++)
      chartDateList.emplace_back(chartDateListString[n], chartDateListDouble[n]);
    if (!chartGroupedList.empty())
    {

      setupBoxWhiskersChart(ui->boxWhiskersChart, chartGroupedList);

      if (namesList.size()>1)
      {
          ui->graphTitle->setText("Total " + typeNames[fileType] + " for selected list");
      }
      else
      {
          ui->graphTitle->setText("Total " + typeNames[fileType] + " by " + QString::fromStdString(namesList[0]));
      }
      ui->graph_stackedWidget->show();
    }else
    {
      ui->graph_stackedWidget->hide();
      ui->graphTitle->clear();
    }
    if (!chartList.empty())
    {
        ui->barChart->clearPlottables();
        setupBarChart(ui->barChart, chartList);
        ui->barChart->replot();

        ui->lineChart->clearPlottables();
        setupLineChart(ui->lineChart, chartDateList);
        ui->lineChart->replot();

        setupPieChart(ui->pieChart, ui->pieList, chartList);

        if (namesList.size()>1)
        {
            ui->graphTitle->setText("Total " + typeNames[fileType] + " for selected list");
        }
        else
        {
            ui->graphTitle->setText("Total " + typeNames[fileType] + " by " + QString::fromStdString(namesList[0]));
        }
        ui->graph_stackedWidget->show();
    }
    else
    {
        ui->graph_stackedWidget->hide();
        ui->graphTitle->clear();
    }
}





void Tab::on_new_sort_clicked()
{
    if (db != NULL) {
        CustomSort* sortdialog = new CustomSort();
        sortdialog->setFields(MANFIELDS[fileType]);

        int ret = sortdialog->exec();
        if (ret) {
            QStringList newSortOrder = sortdialog->getSortFields();
            allOrders << newSortOrder;
            ui->sort->addItem(newSortOrder.at(0));

            // save the sort fields to file
            QSortListIO saveSort(ORDERS_SAVE[fileType]);
            saveSort.saveList(allOrders);
        }
        delete sortdialog;
    } else {
        QMessageBox::critical(this, "Missing File", "Please load a file first.");
    }
}

void Tab::on_edit_sort_clicked()
{
    if (db != NULL) {
        CustomSort* sortdialog = new CustomSort();
        sortdialog->setFields(MANFIELDS[fileType]);
        sortdialog->setSelectedFields(ui->sort->currentText(),sortOrder);
        int ret = sortdialog->exec();
        if (ret) {
            int index = ui->sort->currentIndex();
            allOrders.replace(index, sortdialog->getSortFields());
            ui->sort->setItemText(index, allOrders.at(index)[0]);

            // save the sort fields to file
            QSortListIO saveSort(ORDERS_SAVE[fileType]);
            saveSort.saveList(allOrders);

            //call currentIndexChanged to refresh sort
            on_sort_currentIndexChanged(index);
        }
        delete sortdialog;
    } else {
        QMessageBox::critical(this, "Missing File", "Please load a file first.");
    }
}

char Tab::getFilterStartChar() {
    char charInField;

    // get char from the text field
    charInField = ui->filter_from->text().toStdString()[0];

    // convert the char to uppercase
    char uppercase = charInField & ~0x20;

    // if alphabetical, return that letter
    if ('A' <= uppercase && uppercase <= 'Z') {
        return uppercase;
    } else {
        // otherwise, default is '*'
        return '*';
    }
}

char Tab::getFilterEndChar() {
    char charInField;

    // get char from the text field
    charInField = ui->filter_to->text().toStdString()[0];

    // convert the char to uppercase
    char uppercase = charInField & ~0x20;

    // if alphabetical, return that letter
    if ('A' <= uppercase && uppercase <= 'Z') {
        return uppercase;
    } else if (charInField == '*') {
        // otherwise, is it '*'?
        return charInField;
    } else {
        // otherwise, default is 'Z'
        return 'Z';
    }
}

void Tab::setupPieChart(PieChartWidget* pieChart, QListWidget *pieListWidget, std::vector<std::pair <std::string, double>> pieChartList) {
    // draws the pie graph by sending piechartwidget a vector of name, presentation count
    int pieSize = (int) pieChartList.size();
    QVector<QColor> colorList(pieSize);
    pieListWidget->clear();
    for (int i = 0; i < pieSize; i++) {
        colorList[i] = (QColor(qrand() % 256, qrand() % 256, qrand() % 256));
        pieListWidget->addItem(QString::fromStdString(pieChartList[i].first));

        // set legend colors
        QPixmap pixmap(100, 100);
        pixmap.fill(QColor(colorList[i]));
        QIcon tempIcon(pixmap);
        pieListWidget->item(i)->setIcon(tempIcon);
    }

    pieChart->setData(pieChartList, colorList); //passes vector list to piechartwidget
}

void Tab::setupBarChart(QCustomPlot *barChart, std::vector<std::pair <std::string, double>> barChartList) {
    // create empty bar chart objects:
    QCPBars *yLabels = new QCPBars(barChart->yAxis, barChart->xAxis);
    barChart->addPlottable(yLabels);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    yLabels->setName("Type");
    pen.setColor(QColor(255, 131, 0));
    yLabels->setPen(pen);
    yLabels->setBrush(QColor(255, 131, 0, 50));

    //get label list
    int barSize = (int) barChartList.size();
    double maxCount = 0;
    double scaledCount;
    QVector<double> ticks;
    QVector<QString> ylabels;
    QVector<double> count;

    //add label list to y axis labels
    for (int i = 0; i < barSize; i++){
        if (maxCount < barChartList[i].second)
            maxCount = barChartList[i].second;
    }
    for (int i = 0; i < barSize; i++){
        ticks << (i+1);
        ylabels << QString::fromStdString(barChartList[i].first);
        if (maxCount>1000000){
            scaledCount = barChartList[i].second/1000000;
        } else if (maxCount>1000){
            scaledCount = barChartList[i].second/1000;
        } else{
            scaledCount = barChartList[i].second;
        }
        count <<scaledCount;
    }


    //setup Y Axis
    barChart->yAxis->setAutoTicks(false);
    barChart->yAxis->setAutoTickLabels(false);
    barChart->yAxis->setTickVector(ticks);
    barChart->yAxis->setTickVectorLabels(ylabels);
    barChart->yAxis->setTickLabelPadding(1);
    barChart->yAxis->setSubTickCount(0);
    barChart->yAxis->setTickLength(0, 1);
    barChart->yAxis->grid()->setVisible(true);
    barChart->yAxis->setRange(0, barSize+1);

    if(maxCount>1000000){
        maxCount = maxCount/1000000;
        barChart->xAxis->setLabel("Total (in Millions)");
    }else if (maxCount>1000){
        maxCount = maxCount/1000;
        barChart->xAxis->setLabel("Total (in Thousands)");
    }else{
        barChart->xAxis->setLabel("Total");
    }

    // setup X Axis
    barChart->xAxis->setAutoTicks(true);
    barChart->xAxis->setRange(0,maxCount+(maxCount*.05));
    barChart->xAxis->setAutoTickLabels(true);
    barChart->xAxis->setAutoTickStep(true);
    barChart->xAxis->grid()->setSubGridVisible(true);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    barChart->xAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    barChart->xAxis->grid()->setSubGridPen(gridPen);

    yLabels->setData(ticks, count);
}

void Tab::setupLineChart(QCustomPlot *lineChart, std::vector<std::pair <std::string, double>> lineChartList) {

    QVector<double> y(lineChartList.size());
    double maxCount = 0;
    double scaledCount;
    QVector<double> ticks;
    QVector<QString> xlabels;
    QVector<double> count;

    //Add label list to X axis labels
    for (int i = 0; i < lineChartList.size(); i++){
        if (maxCount < lineChartList[i].second)
            maxCount = lineChartList[i].second;
    }
    for (int i = 0; i < lineChartList.size(); i++){
        ticks << (i+1);
        xlabels << QString::fromStdString(lineChartList[i].first);
        if (maxCount>1000000){
            scaledCount = lineChartList[i].second/1000000;
        } else if (maxCount>1000){
            scaledCount = lineChartList[i].second/1000;
        } else{
            scaledCount = lineChartList[i].second;
        }
        count <<scaledCount;
    }

    lineChart->addGraph();

    //Set up X axis
    lineChart->xAxis->setAutoTicks(false);
    lineChart->xAxis->setAutoTickLabels(false);
    lineChart->xAxis->setTickVector(ticks);
    lineChart->xAxis->setTickVectorLabels(xlabels);
    lineChart->xAxis->setTickLabelPadding(1);
    lineChart->xAxis->setSubTickCount(0);
    lineChart->xAxis->setTickLength(0, 1);
    lineChart->xAxis->grid()->setVisible(true);
    lineChart->xAxis->setRange(0, lineChartList.size()+1);

    if(maxCount>1000000){
        maxCount = maxCount/1000000;
        lineChart->xAxis->setLabel("Total (in Millions)");
    }else if (maxCount>1000){
        maxCount = maxCount/1000;
        lineChart->xAxis->setLabel("Total (in Thousands)");
    }else{
        lineChart->xAxis->setLabel("Total");
    }

    //Set up Y axis
    lineChart->yAxis->setAutoTicks(true);
    lineChart->yAxis->setRange(0,maxCount+(maxCount*.05));
    lineChart->yAxis->setAutoTickLabels(true);
    lineChart->yAxis->setAutoTickStep(true);
    lineChart->yAxis->grid()->setSubGridVisible(true);

    //Set the graph data
    lineChart->graph(0)->setData(ticks,count);
    lineChart->replot();

}


//saves modified CSV to a new a CSV file
 std::vector<std::vector<std::string>> Tab::saveToNewCSV(QWidget *parent){

    std::ofstream fileout;

    QString fileName = QFileDialog::getSaveFileName(parent,
           QObject::tr("Save database file"), QDir::currentPath(),
           QObject::tr("CSV (*.csv);;All Files (*)"));

    //give the user an option to save it to a specific filepath (replacement or overwrite
    fileout.open(fileName.toStdString(), std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);

    if (fileout.is_open()){

        BasicRecord headers = db->getHeaders();
        for (BasicRecord::iterator it = headers.begin(); it != headers.end(); ++it){
            if (it == headers.end()-1)
                fileout << '"' << *it << '"' << "\n";
            else fileout << '"' << *it << '"' << ",";
        }

        for (std::vector<std::vector<std::string>>::iterator it = data.begin(); it != data.end(); ++it){
            for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2){
                if (it2 == it->end()-1)
                    fileout << '"' << *it2 << '"' << "\n";
                else fileout << '"' << *it2 << '"' << ",";
            }
        }
        fileout.close();
        path = fileName;
        return data;
        //std::cout<<"writing to file: successful"<<std::endl;
    }
    else{
        return data;
        //std::cout<<"couldn't open file, didn't save to file."<<std::endl;
    }

}
void Tab::on_FromDate_dateChanged(const QDate &date)
{
    this->yearStart = date.year();
    ui->ToDate->setMinimumDate(date);
    refresh();
}

void Tab::on_ToDate_dateChanged(const QDate &date)
{
    this->yearEnd = date.year();
    ui->ToDate->setMinimumDate(ui->FromDate->date());
    refresh();
}



void Tab::on_boxwhisker_button_toggled(bool checked)
{
    if (checked) ui->graph_stackedWidget->setCurrentIndex(1);
}

void Tab::setupBoxWhiskersChart(BoxWhiskersWidget* boxChart, std::vector<std::pair <std::string, std::vector<double>>> boxChartList) {
    boxChart->setData("TITLE", boxChartList); //passes vector list to BoxWhiskersWidget
}

void Tab::on_line_button_toggled(bool checked)
{
    if (checked) ui->graph_stackedWidget->setCurrentIndex(3);
}
