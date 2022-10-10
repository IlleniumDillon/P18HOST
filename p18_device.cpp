#include "p18_device.h"

P18_device::P18_device()
{
    serialDriver = new P18_serial;
    //plotDriver = new P18_plot();
    fileDriver = new P18_file;
    connect(serialDriver,&P18_serial::serial_dataValid,this,&P18_device::device_readDataUpdate);
    connect(this,&P18_device::device_writeData,serialDriver,&P18_serial::serial_write);
}

void P18_device::device_readDataUpdate()
{
    if(this->serialDriver->rxBuffer.fifo_isEmpty()) return ;
    serialPack data = this->serialDriver->rxBuffer.fifo_read();
    memcpy(shiftRead,data.value_ch0,12);
}

void P18_device::device_writeDataUpdate()
{
    if(useFileFlag&&(!fileDriver->fileData.fifo_isEmpty()))
    {
        shiftSet[0] = fileDriver->fileData.fifo_read();
        shiftSet[1] = fileDriver->fileData.fifo_read();
        shiftSet[2] = fileDriver->fileData.fifo_read();
    }
    serialPack data;
    data.check = 0;
    uint8_t* ptr = (uint8_t*)&data;
    memcpy(data.value_ch0,shiftSet,12);
    for(uint8_t i = 0; i < 13; i++)
    {
        data.check += ptr[i];
    }
    this->serialDriver->txBuffer.fifo_write(data);
    emit device_writeData();
}

void P18_device::device_run()
{
    timer_ID = this->startTimer(50,Qt::PreciseTimer);
    isRunning = 1;
}

void P18_device::device_stop()
{
    killTimer(timer_ID);
    isRunning = 0;
}
