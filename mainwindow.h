#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QMap>
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



    QQueue<QByteArray> controlPanelQueue;  // 控制仪表命令队列
    bool isControlPanelHexBusy = false;     // 仪表16进制命令控制忙碌标志

    QQueue<QByteArray> controlLoadQueue;  // 控制负载命令队列
    bool isControlLoadHexBusy = false;     // 负载16进制控制忙碌标志

    //存储突加Y坐标数据的vector
    QQueue<QByteArray> suddYDataQueue;
    bool isReadSuddLoadYHexBusy = false;


    //保存串口COM名
    QString serialPortComName = "";


    //测试控制鲲航科技
    void testKHKJ();
    //测试控制8961C2检测仪
    void test8961C2();

    QTimer *timer = nullptr;

    QMap<QString, QString> valueMap; // 存储QLineEdit的文本



private:

    // 从队列发送下一个"控制仪表实时读书"的数据
    void sendControlPanelHex();

    // 从队列发送下一个"控制负载"的数据
    void sendControlLoadHex();

    //读取所有突加Y坐标数据
    void readSuddLoadYDataHex();




    // 列出指定类型如:USB串口设备
    void listUsbSerialPorts();
    //判断是不是usb的串口com
    bool isUsbSerialPort(const QSerialPortInfo &portInfo);

    QString get_USB_PORT_COM(const QSerialPortInfo &portInfo);


private slots:


    void sendTestData();

    void onDataReceived(const QString &portName, const QByteArray &data);//测试

};

#endif // MAINWINDOW_H
