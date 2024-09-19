#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QQueue>


#include "serialportmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;



    QQueue<QByteArray> controlDataQueue;  // 控制数据命令队列
    QQueue<QByteArray> readDataQueue;  // 读取数据命令队列
    bool isSerialControlBusy = false;     // 串口控制忙碌标志
    bool isSerialReadBusy = false;     // 串口读取忙碌标志


    //测试控制鲲航科技
    void testKHKJ();
    //测试控制8961C2检测仪
    void test8961C2();

private:
    void sendNextControlData();      // 从队列发送下一个控制的数据

    void sendNextReadData();      // 从队列发送下一个数据

private slots:



    void onDataReceived(const QString &portName, const QByteArray &data);//测试

};

#endif // MAINWINDOW_H
