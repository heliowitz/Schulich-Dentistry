#ifndef CSVREADER_H
#define CSVREADER_H
using namespace std;

#include <string>
#include <vector>
#include <QString>
#include <QFileDialog>
#include <fstream>
#include <ostream>
#include <istream>

#include <string>
#include <QMainWindow>
#include <vector>
#include <QDate>
#include <QDebug>
#include <QDirModel>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPrintDialog>
#include <QPrinter>

class CSVReader {
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> all_data;
    std::vector<std::vector<std::string>>data;
public:
    CSVReader();
    CSVReader(std::string);
    void loadCSV(std::string);
    int parseDateString(std::string dateString);
    std::vector<std::string> getHeaders();
    std::vector<std::vector<std::string>> getData();
    bool fileOpen;
};

#endif // CSVREADER_H
