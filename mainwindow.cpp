#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    signalBind2();


}

void MainWindow::signalBind2()
{
    // 配置和使用 SerialPortManager，设置发送端串口，打开串口等
        SerialPortManager &manager = SerialPortManager::getInstance();
        manager.addSerialPort("COM1");
        manager.configurePort("COM1", QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);
        manager.openPort("COM1", QSerialPort::ReadWrite);
        // 发送数据以测试接收
        manager.writeData("COM1", QByteArray::fromHex("010600200007"));


        manager.addSerialPort("COM4");
        manager.configurePort("COM4", QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);
        manager.openPort("COM4", QSerialPort::ReadWrite);
//        manager.writeData("COM4", QByteArray::fromHex("01050020ff00"));
        connect(ui->pushButton,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
            qDebug()<<"打开01";
            QByteArray dataToSend = QByteArray::fromHex("01050020ff00");
            controlDataQueue.enqueue(dataToSend); // 将数据加入队列
                qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
                // 如果串口不忙，立即发送
                if (!isSerialControlBusy) {
                    sendNextControlData();
                }

        });
        connect(ui->pushButton_4,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
            qDebug()<<"关闭01";
            QByteArray dataToSend = QByteArray::fromHex("010500200000");
            controlDataQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
            // 如果串口不忙，立即发送
            if (!isSerialControlBusy) {
                sendNextControlData();
            }
        });
        connect(ui->pushButton_3,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
             qDebug()<<"打开01-03";
            QByteArray dataToSend = QByteArray::fromHex("010600200007");
            controlDataQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
            // 如果串口不忙，立即发送
            if (!isSerialControlBusy) {
                sendNextControlData();
            }
        });
        connect(ui->pushButton_6,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
             qDebug()<<"关闭01-03";
            QByteArray dataToSend = QByteArray::fromHex("010600200000");
            controlDataQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
            // 如果串口不忙，立即发送
            if (!isSerialControlBusy) {
                sendNextControlData();
            }
        });


        //0103002000030401 :开始检测：
        //接收数据：0103060001000000001cb5  检测到1号开
        //接收数据：01030600070000000094b5  检测到1,2,3号开
        //接收数据: 0103060000000000002175  检测到所有开关都是关闭

        connect(ui->pushButton_7,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
             qDebug()<<"开始检测那些开关是开的";
            QByteArray dataToSend = QByteArray::fromHex("0103002000030401");
            readDataQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
            // 如果串口不忙，立即发送
            if (!isSerialControlBusy) {
                sendNextReadData();
            }
        });



        connect(&manager, &SerialPortManager::dataReceived, this, &MainWindow::onDataReceived);

}

void MainWindow::onDataReceived(const QString &portName, const QByteArray &data)
{
    qDebug() << "Data received on port" << portName << ":" << data.toHex();
            // 处理接收到的数据
    isSerialControlBusy = false; // 标记为不忙碌
    // 继续发送队列中的下一个数据
    sendNextControlData();
}


void MainWindow::sendNextControlData()
{
    qDebug()<<"队列中数据:"<<controlDataQueue.length();
    if (!controlDataQueue.isEmpty()) {
        qDebug()<<"队列中有数据";
        QByteArray nextData = controlDataQueue.dequeue(); // 从队列中取出数据
        isSerialControlBusy = true; // 标记为忙碌
        SerialPortManager::getInstance().writeData("COM4", nextData);
    }
//    if(dataQueue.isEmpty()){
//        qDebug()<<"队列中数据全部发送完毕!!";
//    }
}

void MainWindow::sendNextReadData()
{
    qDebug()<<"队列中数据:"<<readDataQueue.length();
    if (!readDataQueue.isEmpty()) {
        qDebug()<<"队列中有数据";
        QByteArray nextData = readDataQueue.dequeue(); // 从队列中取出数据
        isSerialReadBusy = true; // 标记为忙碌
        SerialPortManager::getInstance().readData("COM4",nextData);
    }
//    if(dataQueue.isEmpty()){
//        qDebug()<<"队列中数据全部发送完毕!!";
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
