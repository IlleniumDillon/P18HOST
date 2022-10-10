#ifndef P18_PLOT_H
#define P18_PLOT_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QValueAxis>
#include <QLineSeries>
#include <QSplineSeries>
#include <QRandomGenerator>
#include <QDateTime>

#include "p18_device.h"

using namespace QtCharts;

#define CHANNEL_MAX (6)

typedef enum
{
    CHANNEL0,CHANNEL1,CHANNEL2,
    CHANNEL3,CHANNEL4,CHANNEL5,
}channel;

class P18_plot : public QGraphicsView
{
    Q_OBJECT
public:
    P18_plot(QWidget *parent,P18_device* dev);

    void plot_initLineSeries();

    void plot_addLineSeries(channel ch,float value);

    void plot_addLineSeries(channel ch,float time, float value);

    void timerEvent(QTimerEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    QLineSeries series[CHANNEL_MAX];
    QChart *plotChart;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QGraphicsSimpleTextItem *plotLine[CHANNEL_MAX];
    QGraphicsSimpleTextItem *coordX;
    QGraphicsSimpleTextItem *coordY;
    qint64    MSecBase;
    P18_device* dev;
    int timer_ID;
};

#endif // P18_PLOT_H
