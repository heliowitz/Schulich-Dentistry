#include "boxwhiskerswidget.h"

BoxWhiskersWidget::BoxWhiskersWidget(QChartView *parent) : QChartView(parent)
{
}

void BoxWhiskersWidget::setData(std::string title, std::vector<std::pair <std::string, std::vector<double>>> boxChartList)
{
    //vRecordList = boxChartList;

    std::vector<QBoxPlotSeries *> v_plotSeries;
    QBoxPlotSeries *plotSeries;
    QList<double> sortedList;

    // TODO: add chartView to qwidget in mainwindow
    // TODO: calculate min/max Y-axis throughout data loop
    // TODO: create data map based on member names and years

    float minAxisY = 0, maxAxisY = 1;

    chart = new QChart();

    for (int i = 0; i < boxChartList.size(); i++) {
        sortedList.clear();

        plotSeries = new QBoxPlotSeries();
        plotSeries->setName(QString::fromStdString(boxChartList[i].first));
        v_plotSeries.push_back(plotSeries);

        //for(int x = 0; x < 4; x++) sortedList.append(x);
        for(int x = 0; x < boxChartList[i].second.size(); x++) sortedList.append(boxChartList[i].second[x]);

        qSort(sortedList.begin(), sortedList.end());

        QBoxSet *set = createBox(QString::fromStdString(boxChartList[i].first), sortedList);
        if (set) plotSeries->append(set);

        chart->addSeries(plotSeries);

        if (sortedList[0] < minAxisY) minAxisY = sortedList[0]; // compare to lower extreme
        if (sortedList[sortedList.end()-sortedList.begin()-1] > maxAxisY) maxAxisY = sortedList[sortedList.end()-sortedList.begin()-1]; // compare to higher extreme
    }

    chart->setTitle(QString::fromStdString(title));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();
    chart->axisY()->setMin(minAxisY);
    chart->axisY()->setMax(maxAxisY);

    //chartView = new QChartView(chart);
    //chartView->setRenderHint(QPainter::Antialiasing);

    chart->setVisible(true);
    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setVisible(true);
}

QBoxSet * BoxWhiskersWidget::createBox(QString boxStr, QList<double> sortedList)
{
    int count = sortedList.count();

    QBoxSet *box = new QBoxSet(boxStr);
    box->setValue(QBoxSet::LowerExtreme, sortedList.first());
    box->setValue(QBoxSet::UpperExtreme, sortedList.last());
    box->setValue(QBoxSet::Median, findMedian(sortedList, 0, count));
    box->setValue(QBoxSet::LowerQuartile, findMedian(sortedList, 0, count / 2));
    box->setValue(QBoxSet::UpperQuartile, findMedian(sortedList, count / 2 + (count % 2), count));

    return box;
}

qreal BoxWhiskersWidget::findMedian(QList<double> sortedList, int begin, int end)
{
    int count = end - begin;

    if (count == 0) return 0;
    if (count == 1) return sortedList.at(begin);

    if (count % 2) {
        return sortedList.at(count / 2 + begin);
    } else {
        qreal right = sortedList.at(count / 2 + begin);
        qreal left = sortedList.at(count / 2 - 1 + begin);
        return (right + left) / 2.0;
    }
}
