#include "p18_plot.h"

P18_plot::P18_plot(QWidget *parent,P18_device* dev)
    :QGraphicsView(new QGraphicsScene, parent)
{
    this->dev = dev;

    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    plotChart = new QChart;

    plotChart->resize(638,528);
    plot_initLineSeries();

    plotChart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(plotChart);


    coordX = new QGraphicsSimpleTextItem(plotChart);
    coordX->setPos(plotChart->size().width()/2 + 50, 20);
    coordX->setText("");
    coordX->setPen(QColor(20,164,226));
    coordY = new QGraphicsSimpleTextItem(plotChart);
    coordY->setPos(plotChart->size().width()/2 + 150, 20);
    coordY->setText("");
    coordY->setPen(QColor(226,20,216));


    this->setMouseTracking(true);

    MSecBase = QDateTime::currentDateTime().toMSecsSinceEpoch();

    timer_ID = startTimer(100,Qt::TimerType::PreciseTimer);
}

void P18_plot::plot_initLineSeries()
{
    QColor colors[CHANNEL_MAX];
    for(int i = 0; i < CHANNEL_MAX; i++)
    {
        colors[i] = QColor(
                    QRandomGenerator::global()->bounded(0,150),
                    QRandomGenerator::global()->bounded(0,150),
                    QRandomGenerator::global()->bounded(0,150)
                    );
    }
    axisX = new QValueAxis;
    axisX->setLabelFormat("%ds");
    axisX->setTickCount(5);
    axisX->setRange(0,5);

    axisY = new QValueAxis;
    axisY->setRange(-0.1,0.1);
    axisY->setTitleText("value");
    axisY->setTickCount(9);

    for(int i =0; i<CHANNEL_MAX;i++)
    {
        series[i].setColor(colors[i]);
        series[i].setName(QString("channel%1").arg(i));

        series[i].setVisible(true);

        plotChart->addSeries(&series[i]);
        plotChart->setAxisX(axisX,&series[i]);
        plotChart->setAxisY(axisY,&series[i]);

        plotLine[i] = new QGraphicsSimpleTextItem(plotChart);
        plotLine[i]->setPos(34+i*80, /*plotChart->size().height()- 20*/500);
        plotLine[i]->setText(QString("channel%1").arg(i));
        plotLine[i]->setPen(colors[i]);
    }

    plotChart->legend()->setVisible(false);
}

void P18_plot::plot_addLineSeries(channel ch,float value)
{
    float currentS =(QDateTime::currentDateTime().toMSecsSinceEpoch() - MSecBase)/1000.0;   //1s=1000MS

    if(currentS>=axisX->max())
    {
        axisX->setMin(currentS-5);
        axisX->setMax(currentS);

    }
    if(value>=qMax(qAbs(axisY->max()),qAbs(axisY->min())))
    {
        axisY->setMin(-value*1.1);
        axisY->setMax(value*1.1);
    }

    //定时清除以前不要的曲线数据
    if(series[ch].count()>100)
    {
       series[ch].removePoints(0,series[ch].count()/2);
    }
    series[ch].append(QPointF(currentS,value));
}

void P18_plot::plot_addLineSeries(channel ch,float time, float value)
{
    if(time>=axisX->max())
    {
        axisX->setMin(time-5);
        axisX->setMax(time);

    }
    if(value>=qMax(qAbs(axisY->max()),qAbs(axisY->min())))
    {
        axisY->setMin(-value*1.1);
        axisY->setMax(value*1.1);
    }
    series[ch].append(QPointF(time,value));
}


void P18_plot::timerEvent(QTimerEvent* event)
{
    static double i = 0;
    if(event->timerId() == timer_ID)
    {
        i++;
        plot_addLineSeries(CHANNEL0,i*0.1,dev->shiftSet[0]);
        plot_addLineSeries(CHANNEL1,i*0.1,dev->shiftSet[1]);
        plot_addLineSeries(CHANNEL2,i*0.1,dev->shiftSet[2]);
        plot_addLineSeries(CHANNEL3,i*0.1,dev->shiftRead[0]);
        plot_addLineSeries(CHANNEL4,i*0.1,dev->shiftRead[1]);
        plot_addLineSeries(CHANNEL5,i*0.1,dev->shiftRead[2]);
    }
}

void P18_plot::mouseMoveEvent(QMouseEvent *event)
{
    coordX->setText(QString("value:%1").arg(QString::asprintf("%.2f",plotChart->mapToValue(event->pos()).y())));
    coordY->setText(QString("time:%1S").arg(QString::asprintf("%.2f",plotChart->mapToValue(event->pos()).x())));
    QGraphicsView::mouseMoveEvent(event);
}
