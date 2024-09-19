#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

        connect(&manager, &SerialPortManager::dataReceived, this, &MainWindow::onDataReceived);


    testKHKJ();
    test8961C2();


}

void MainWindow::testKHKJ()
{

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





}


void MainWindow::test8961C2()
{
    connect(ui->pushButton_8,&QPushButton::clicked,this,[this](){
        // 发送指定的16进制数据
         qDebug()<<"锁定页面0";
        QByteArray dataToSend = QByteArray::fromHex("011000000001020000");
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        // 如果串口不忙，立即发送
        if (!isSerialControlBusy) {
            sendNextControlData();
        }
    });

    connect(ui->pushButton_9,&QPushButton::clicked,this,[this](){
        // 发送指定的16进制数据
         qDebug()<<"开始突加。。。";
        QByteArray dataToSend = QByteArray::fromHex("011000000001020024");
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        // 如果串口不忙，立即发送
        if (!isSerialControlBusy) {
            sendNextControlData();
        }
    });

    connect(ui->pushButton_10,&QPushButton::clicked,this,[this](){

        qDebug()<<"读取时间:年、月、日、时、分、秒......";
        // 发送指定的16进制数据(最后的0003意为:读3个寄存器即：6个字节)
        QByteArray dataToSend = QByteArray::fromHex("010300140003");
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        // 如果串口不忙，立即发送
        if (!isSerialControlBusy) {
            sendNextControlData();
        }
    });

    connect(ui->pushButton_11,&QPushButton::clicked,this,[this](){

        qDebug()<<"锁定页面4......";
        //0x84即0084,锁定数据更新并切换到4页
        QByteArray dataToSend = QByteArray::fromHex("011000000001020084");
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        // 如果串口不忙，立即发送
        if (!isSerialControlBusy) {
            sendNextControlData();
        }
    });
    connect(ui->pushButton_12,&QPushButton::clicked,this,[this](){

        qDebug()<<"返回寄存器状态......";

        QByteArray dataToSend = QByteArray::fromHex("010300000001");
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        // 如果串口不忙，立即发送
        if (!isSerialControlBusy) {
            sendNextControlData();
        }
    });

    connect(ui->pushButton_13,&QPushButton::clicked,this,[this](){

        qDebug()<<"锁定页面1......";

        QByteArray dataToSend = QByteArray::fromHex("011000000001020081");
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        // 如果串口不忙，立即发送
        if (!isSerialControlBusy) {
            sendNextControlData();
        }
    });

    connect(ui->pushButton_14,&QPushButton::clicked,this,[this](){

        qDebug()<<"读取线路一电压偏差...";
        /*
         * 注意：对于读取仪表示数前，要先锁定当前数据所在页面，再执行请求帧
         * 这里的requestFramList.at(0):锁定页面1
         * 这里的requestFramList.at(1):读取线路1瞬时电压偏差-
         * 这里的requestFramList.at(2):读取线路1电压恢复时间
        */
        QStringList requestFramList = {"011000000001020081","010300120002","0103001A0002"};
        for(int i = 0;i<requestFramList.length();i++){

            QString hexString = requestFramList.at(i);  // 获取 QString
            QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
            QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
            controlDataQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        }


//            QByteArray dataToSend = QByteArray::fromHex("010300120002");
//            controlDataQueue.enqueue(dataToSend); // 将数据加入队列
//            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
        // 如果串口不忙，立即发送
        if (!isSerialControlBusy) {
            sendNextControlData();
        }
    });
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
