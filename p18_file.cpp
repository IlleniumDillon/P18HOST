#include "p18_file.h"

P18_file::P18_file()
{

}

void P18_file::file_load(QString path)
{
    //fileData.fifo_clear();
    QFile file(path);
    QStringList line;
    if(!file.open(QIODevice::ReadOnly)) return;
    QTextStream text(&file);
    while(!text.atEnd())
    {
        line.push_back(text.readLine());
    }
    for(int j = 0; j < line.size(); j++)
    {
        QString temp = line.at(j);
        QStringList split = temp.split(",");
        for(int i = 0; i < split.size(); i++)
        {
            fileData.fifo_write(split.at(i).toFloat());
        }
    }
    file.close();
}

void P18_file::file_write(QString path)
{

}
