#ifndef P18_FILE_H
#define P18_FILE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QTimerEvent>
#include <QFile>
#include <QTextStream>
#include "fifo.h"

class P18_file : public QObject
{
    Q_OBJECT
public:
    P18_file();
    void file_load(QString path);
    void file_write(QString path);
    FIFO<float> fileData;
};

#endif // P18_FILE_H
