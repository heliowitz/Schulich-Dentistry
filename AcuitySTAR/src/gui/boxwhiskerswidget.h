#ifndef BOXWHISKERSWIDGET_H
#define BOXWHISKERSWIDGET_H

#include <QWidget>
#include <QtCharts/QBoxSet>
#include <QString>
#include <QList>
#include <QBoxSet>
#include <QChart>
#include <QChartView>
#include <QBoxPlotSeries>

QT_CHARTS_USE_NAMESPACE

class BoxWhiskersWidget : public QChartView
{
    Q_OBJECT
public:
    explicit BoxWhiskersWidget(QChartView *parent = 0);

    void setData(std::string title, std::vector<std::pair <std::string, std::vector<double>>> boxChartList);
    QBoxSet * createBox(QString boxStr, QList<double> sortedList);
    qreal findMedian(QList<double> sortedList, int begin, int end);

signals:

public slots:

private:
    std::vector<std::pair<std::string, std::vector<double>>> vRecordList;
    QChart *chart;
    QChartView *chartView;
};

#endif // BOXWHISKERSWIDGET_H
