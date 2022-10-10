#include "p18_serial.h"

P18_serial::P18_serial()
{
    connect(&serial,&QSerialPort::readyRead,this,&P18_serial::serial_read);
}

void P18_serial::serial_config(QString comNum, uint32_t baud)
{
    if(isSerialOpen) serial_close();
    serial.setBaudRate(baud);
    serial.setPortName(comNum);
    serial.setDataBits(QSerialPort::Data8);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
}

void P18_serial::serial_write()
{
    if(!isSerialOpen) return;
    if(txBuffer.fifo_isEmpty()) return;
    serialPack txdata = txBuffer.fifo_read();
    serial.write((char*)&txdata,sizeof (txdata));
}

void P18_serial::serial_read()
{
    static uint8_t readNum = 0;
    static uint8_t rxData[28] = {0};
    static uint8_t check = 0;

    int64_t readsize = serial.read((char*)rxData,28);
    serial.clear();
    if(readsize>=14)
    {
        int64_t startIndx = -1;
        for(int64_t i = 0; i < readsize; i++)
        {
            if(rxData[i]==0xaa)
            {
                startIndx = i;
            }
        }
        if(startIndx == -1) return;
        uint8_t temp = 0;
        for(uint8_t i = 0; i < 13; i++)
        {
            temp += rxData[startIndx+i];
        }
        if(temp==rxData[startIndx+13])
        {
            serialPack pack;
            memcpy(&pack,rxData+startIndx,14);
            rxBuffer.fifo_write(pack);
            emit serial_dataValid();
        }
    }

    //serial.read((char*)&rxData[readNum],1);

    /*check+=rxData[readNum];
    readNum++;
    if(readNum==1 && rxData[readNum-1]!=0xaa)
    {
        check = 0;
        readNum = 0;
    }
    else if(readNum==14)
    {
        if(check==(uint8_t)(rxData[readNum-1]*2&0xff))
        {
            serialPack temp;
            memcpy(&temp,rxData,14);
            rxBuffer.fifo_write(temp);
            emit serial_dataValid();
        }
        check = 0;
        readNum = 0;
    }*/
}

void P18_serial::serial_open()
{
    serial_config(comNum,baud);
    if(serial.open(QIODevice::ReadWrite))
    {
        //timer_ID = this->startTimer(20);
        isSerialOpen = true;
    }
    else
    {
        ;
    }
}

void P18_serial::serial_close()
{
    serial.close();
    //this->killTimer(timer_ID);
    txBuffer.fifo_clear();
    rxBuffer.fifo_clear();
    isSerialOpen = false;
}
