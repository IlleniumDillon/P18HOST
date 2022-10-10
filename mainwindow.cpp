#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    p18_device = new P18_device;
    plotDriver = new P18_plot(ui->widgetPlot,p18_device);

    on_pushButtonRefresh_clicked();

    ui->lineEditSetCh0->setText("0");
    ui->lineEditSetCh1->setText("0");
    ui->lineEditSetCh2->setText("0");

    ui->groupBoxCtrl->setEnabled(false);
    ui->groupBoxData->setEnabled(false);
    ui->groupBoxFileOp->setEnabled(false);

    ui->pushButtonRX->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBoxComName_currentIndexChanged(const QString &arg1)
{
    p18_device->serialDriver->comNum = arg1;
}

void MainWindow::on_lineEdit_editingFinished()
{
    p18_device->serialDriver->baud = ui->lineEdit->text().toInt();
}

void MainWindow::on_pushButtonRefresh_clicked()
{
    ui->comboBoxComName->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxComName->addItem(info.portName());
    }
}

void MainWindow::on_pushButtonOp_clicked()
{
    if(p18_device->serialDriver->isSerialOpen)
    {
        p18_device->serialDriver->serial_close();
        ui->pushButtonOp->setText("open serial port");
        ui->pushButtonRefresh->setEnabled(true);
        ui->comboBoxComName->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->groupBoxCtrl->setEnabled(false);
        ui->groupBoxData->setEnabled(false);
        ui->groupBoxFileOp->setEnabled(false);
        killTimer(timer_ID);
        //killTimer(plot_ID);
    }
    else
    {
        p18_device->serialDriver->serial_open();
        if(!p18_device->serialDriver->isSerialOpen)
        {
            QMessageBox msg(this);
            msg.about(NULL,"INFO","error accured when opening port");
            return;
        }
        ui->pushButtonOp->setText("close serial port");
        ui->pushButtonRefresh->setEnabled(false);
        ui->comboBoxComName->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->groupBoxCtrl->setEnabled(true);
        ui->groupBoxData->setEnabled(true);
        ui->groupBoxFileOp->setEnabled(true);
        timer_ID = startTimer(100);
        //plot_ID = startTimer(200);
    }
}

void MainWindow::on_pushButtonFileLoad_clicked()
{
    p18_device->serialDriver->serial_close();
    QString path = QFileDialog::getOpenFileName(this,"select source file","","*.csv");
    if(!path.isEmpty())
    {
        p18_device->fileDriver->file_load(path);
    }
    p18_device->serialDriver->serial_open();
}

void MainWindow::on_pushButtonFileRun_clicked()
{
    if(p18_device->useFileFlag)
    {
        p18_device->useFileFlag = 0;
        ui->pushButtonFileRun->setText("run");
        ui->lineEditSetCh0->setEnabled(true);
        ui->lineEditSetCh1->setEnabled(true);
        ui->lineEditSetCh2->setEnabled(true);
        //killTimer(timer_ID);
    }
    else
    {
        p18_device->useFileFlag = 1;
        p18_device->device_run();
        ui->pushButtonTX->setText("disnable TX");
        ui->pushButtonFileRun->setText("stop");
        ui->lineEditSetCh0->setEnabled(false);
        ui->lineEditSetCh1->setEnabled(false);
        ui->lineEditSetCh2->setEnabled(false);
        //timer_ID = startTimer(100);
    }
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    static double time = 0;
    if(event->timerId() == timer_ID)
    {
        time += 0.1;
        if(p18_device->useFileFlag == 1)
        {
            ui->lineEditSetCh0->setText(QString("%1").arg(p18_device->shiftSet[0]));
            ui->lineEditSetCh1->setText(QString("%1").arg(p18_device->shiftSet[1]));
            ui->lineEditSetCh2->setText(QString("%1").arg(p18_device->shiftSet[2]));
        }

        ui->lineEditGetCh0->setText(QString("%1").arg(p18_device->shiftRead[0]));
        ui->lineEditGetCh1->setText(QString("%1").arg(p18_device->shiftRead[1]));
        ui->lineEditGetCh2->setText(QString("%1").arg(p18_device->shiftRead[2]));
    }
    /*else if(event->timerId() == plot_ID)
    {
        plotDriver->plot_addLineSeries(CHANNEL0,time,p18_device->shiftSet[0]);
        plotDriver->plot_addLineSeries(CHANNEL1,time,p18_device->shiftSet[1]);
        plotDriver->plot_addLineSeries(CHANNEL2,time,p18_device->shiftSet[2]);
        plotDriver->plot_addLineSeries(CHANNEL3,time,p18_device->shiftRead[0]);
        plotDriver->plot_addLineSeries(CHANNEL4,time,p18_device->shiftRead[1]);
        plotDriver->plot_addLineSeries(CHANNEL5,time,p18_device->shiftRead[2]);
    }*/
}

void MainWindow::on_pushButtonTX_clicked()
{
    if(p18_device->isRunning)
    {
        p18_device->device_stop();
        ui->pushButtonTX->setText("enable TX");
    }
    else
    {
        p18_device->device_run();
        ui->pushButtonTX->setText("disnable TX");
    }
}

void MainWindow::on_pushButtonRX_clicked()
{

}

void MainWindow::on_lineEditSetCh0_editingFinished()
{
    p18_device->shiftSet[0] = ui->lineEditSetCh0->text().toFloat();
}

void MainWindow::on_lineEditSetCh1_editingFinished()
{
    p18_device->shiftSet[1] = ui->lineEditSetCh1->text().toFloat();
}

void MainWindow::on_lineEditSetCh2_editingFinished()
{
    p18_device->shiftSet[2] = ui->lineEditSetCh2->text().toFloat();
}
