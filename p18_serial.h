#ifndef P18_SERIAL_H
#define P18_SERIAL_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimerEvent>
#include "fifo.h"

#pragma pack(push)
#pragma pack(1)
typedef struct _serialPack
{
    const uint8_t head = 0xAA;
    uint8_t value_ch0[4];
    uint8_t value_ch1[4];
    uint8_t value_ch2[4];
    uint8_t check;
}serialPack;
#pragma pack(pop)

class P18_serial : public QObject
{
    Q_OBJECT
public:
    P18_serial();
signals:
    void serial_dataValid();
public slots:
    void serial_write();
    void serial_read();
public:
    void serial_config(QString comNum, uint32_t baud);
    void serial_open();
    void serial_close();
    /*void timerEvent(QTimerEvent* event) override
    {
        if(event->timerId() == timer_ID)
        {
            serial_write();
        }
    }*/
public:
    QSerialPort serial;
    FIFO<serialPack> txBuffer;
    FIFO<serialPack> rxBuffer;
    bool isSerialOpen = false;
    QString comNum = "";
    uint32_t baud = 9600;
private:
    //int timer_ID;
};

#endif // P18_SERIAL_H
