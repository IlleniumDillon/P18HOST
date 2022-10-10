#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "p18_device.h"
#include "p18_plot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void timerEvent(QTimerEvent* event);

private slots:
    void on_comboBoxComName_currentIndexChanged(const QString &arg1);

    void on_lineEdit_editingFinished();

    void on_pushButtonRefresh_clicked();

    void on_pushButtonOp_clicked();

    void on_pushButtonFileLoad_clicked();

    void on_pushButtonFileRun_clicked();

    void on_pushButtonTX_clicked();

    void on_pushButtonRX_clicked();

    void on_lineEditSetCh0_editingFinished();

    void on_lineEditSetCh1_editingFinished();

    void on_lineEditSetCh2_editingFinished();

private:
    Ui::MainWindow *ui;
    P18_device* p18_device;
    P18_plot* plotDriver;
    int timer_ID, plot_ID;
};
#endif // MAINWINDOW_H
