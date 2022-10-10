#ifndef P18_DEVICE_H
#define P18_DEVICE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "p18_file.h"
//#include "p18_plot.h"
#include "p18_serial.h"

class P18_device : public QObject
{
    Q_OBJECT
public:
    P18_device();
    void device_writeDataUpdate();
    void device_run();
    void device_stop();
    void timerEvent(QTimerEvent* event) override
    {
        if(event->timerId() == timer_ID)
        {
            device_writeDataUpdate();
        }
    }
public slots:
    void device_readDataUpdate();
signals:
    void device_writeData();
public:
    P18_serial* serialDriver;
    //P18_plot* plotDriver;
    P18_file* fileDriver;
public:
    float voltageRead[3] = {0};
    float shiftRead[3] = {0};
    float voltageSet[3] = {0};
    float shiftSet[3] = {0};
    float load[3] = {0};
    float power[3] = {0};

    bool useFileFlag = 0;
    bool isRunning = 0;
private:
    int timer_ID;
};

#endif // P18_DEVICE_H
