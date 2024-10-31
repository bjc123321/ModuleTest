#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMap>
#include <QDebug>

#include "modbusprotocolparser.h"

class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    // 获取单例实例的静态方法
    static SerialPortManager& getInstance() {
        static SerialPortManager instance; // 静态实例对象
        return instance;
    }

    // 数据缓冲区
    QByteArray buffer;


    bool addSerialPort(const QString &portName);
    bool removeSerialPort(const QString &portName);
    QSerialPort* getSerialPort(const QString &portName);

    bool openPort(const QString &portName, QSerialPort::OpenMode mode);
    void closePort(const QString &portName);
    bool writeData(const QString &portName, const QByteArray &data);

    // 设置串口属性
    bool configurePort(const QString &portName,
                       QSerialPort::BaudRate baudRate,
                       QSerialPort::DataBits dataBits,
                       QSerialPort::Parity parity,
                       QSerialPort::StopBits stopBits,
                       QSerialPort::FlowControl flowControl);

signals:
    void dataReceived(const QString &portName, const QByteArray &data);

private slots:
    void handleReadyRead();

    /* 新版处理响应帧数据的方法
     * 假设：协议有n个起始地址(n = 1,2,3,...)根据协议上的起始地址看
     * 如果返回浮点数,即每个起始地址往后读取2个寄存器，数据帧一次性返回(4n)个字节的数据域
     * 如果返回整型,即每个起始地址往后读取1个寄存器，数据帧一次性返回(2n)个字节的数据域
     * 浮点数：2个寄存器，4个字节，8位16进制数(001E 001F)
     * 整型：1个寄存器，2个字节，4位16进制数(0017)
    */
    void handleReadyRead_2();
    // 检查是否有完整的响应帧
    void checkForCompleteFrame(QSerialPort *serialPort);


private:
    SerialPortManager(QObject *parent = nullptr);  // 私有化构造函数
    ~SerialPortManager();

    SerialPortManager(const SerialPortManager&) = delete;  // 禁止拷贝构造
    SerialPortManager& operator=(const SerialPortManager&) = delete;  // 禁止赋值操作

    QMap<QString, QSerialPort*> serialPorts;
};

#endif // SERIALPORTMANAGER_H
