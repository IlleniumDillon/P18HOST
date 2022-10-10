#include "fifo.h"

//template class FIFO<uint8_t>;

template<typename Type>
FIFO<Type>::FIFO()
{
    elementNum = 0;
}

template<typename Type>
void FIFO<Type>::fifo_write(Type element)
{
    if(elementNum>bufferSize) buffer.pop_front();
    buffer.push_back(element);
    elementNum++;
}

template<typename Type>
Type FIFO<Type>::fifo_read()
{
    //if(elementNum==0)return (Type)0;
    Type temp = buffer.first();
    buffer.pop_front();
    elementNum--;
    return temp;
}

template<typename Type>
void FIFO<Type>::fifo_clear()
{
    buffer.clear();
    elementNum = 0;
}

template<typename Type>
bool FIFO<Type>::fifo_isEmpty()
{
    return buffer.isEmpty();
}
