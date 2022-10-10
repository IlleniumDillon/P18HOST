#ifndef FIFO_H
#define FIFO_H

#include <iostream>
#include <QList>

template<typename Type>
class __declspec(dllexport) FIFO
{
public:
    FIFO();
private:
    QList<Type> buffer;
    //size_t writePtr = 0, readPtr = 0;
    size_t elementNum = 0;
    size_t bufferSize = 512;
public:
    void fifo_write(Type element);
    Type fifo_read();
    void fifo_clear();
    bool fifo_isEmpty();
};

#include "fifo.cpp"

#endif // FIFO_H
