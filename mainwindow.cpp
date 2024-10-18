#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,this,&MainWindow::sendTestData);

    listUsbSerialPorts();



//    manager.addSerialPort("COM1");
//    manager.configurePort("COM1", QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);
//    manager.openPort("COM1", QSerialPort::ReadWrite);
//    // 发送数据以测试接收
//    manager.writeData("COM1", QByteArray::fromHex("010600200007"));









//    testKHKJ();
    test8961C2();


}

void MainWindow::sendTestData()
{
    /*
     * 注意：对于读取仪表示数前，要先锁定当前数据所在页面，再执行请求帧
     * 这里的requestFramList.at(0):锁定页面1
     * 这里的requestFramList.at(1):读取线路1瞬时电压偏差-
     * 这里的requestFramList.at(2):读取线路1电压恢复时间
    */
    QStringList requestFramList = {"0103001e0002","010300200002","010300220002",
                                   "0103002e0002","010300300002","010300320002",
                                   "0103002a0002","010300340002","0103003e0002","010300580002",
                                   "010300240002","010300260002","010300280002","0103002c0002"};
    for(int i = 0;i<requestFramList.length();i++){

        QString hexString = requestFramList.at(i);  // 获取 QString
        QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
        QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
    }


//            QByteArray dataToSend = QByteArray::fromHex("010300120002");
//            controlDataQueue.enqueue(dataToSend); // 将数据加入队列
//            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
    // 如果串口不忙，立即发送
    if (!isControlPanelHexBusy) {
        sendControlPanelHex();
    }
}

void MainWindow::testKHKJ()
{

        connect(ui->pushButton,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
            qDebug()<<"打开01";
            QByteArray dataToSend = QByteArray::fromHex("01050020ff00");
            controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
                qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
                // 如果串口不忙，立即发送
                if (!isControlPanelHexBusy) {
                    sendControlPanelHex();
                }

        });
        connect(ui->pushButton_4,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
            qDebug()<<"关闭01";
            QByteArray dataToSend = QByteArray::fromHex("010500200000");
            controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
            // 如果串口不忙，立即发送
            if (!isControlPanelHexBusy) {
                sendControlPanelHex();
            }
        });
        connect(ui->pushButton_3,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
             qDebug()<<"打开01-03";
            QByteArray dataToSend = QByteArray::fromHex("010600200007");
            controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
            // 如果串口不忙，立即发送
            if (!isControlPanelHexBusy) {
                sendControlPanelHex();
            }
        });
        connect(ui->pushButton_6,&QPushButton::clicked,this,[this](){
            // 发送指定的16进制数据
             qDebug()<<"关闭01-03";
            QByteArray dataToSend = QByteArray::fromHex("010600200000");
            controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
            // 如果串口不忙，立即发送
            if (!isControlPanelHexBusy) {
                sendControlPanelHex();
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
            controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
            // 如果串口不忙，立即发送
            if (!isControlPanelHexBusy) {
                sendControlPanelHex();
            }
        });





}


void MainWindow::test8961C2()
{
    connect(ui->pushButton_8,&QPushButton::clicked,this,[this](){
        // 发送指定的16进制数据
         qDebug()<<"锁定页面0";
        QByteArray dataToSend = QByteArray::fromHex("011000000001020000");
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            sendControlPanelHex();
        }
    });

    connect(ui->pushButton_9,&QPushButton::clicked,this,[this](){
        // 发送指定的16进制数据
         qDebug()<<"开始突加。。。";
        QByteArray dataToSend = QByteArray::fromHex("011000000001020024");
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            sendControlPanelHex();
        }
    });

    connect(ui->pushButton_10,&QPushButton::clicked,this,[this](){

        qDebug()<<"读取时间:年、月、日、时、分、秒......";
         QStringList requestFramList = {"011000000001020000","010300140003"};
        // 发送指定的16进制数据(最后的0003意为:读3个寄存器即：6个字节)
         for(int i = 0;i<requestFramList.length();i++){

             QString hexString = requestFramList.at(i);  // 获取 QString
             QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
             QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
             controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
             qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
         }
        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            sendControlPanelHex();
        }
    });

    connect(ui->pushButton_11,&QPushButton::clicked,this,[this](){

        qDebug()<<"锁定页面4......";
        //0x84即0084,锁定数据更新并切换到4页
        QByteArray dataToSend = QByteArray::fromHex("011000000001020084");
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            sendControlPanelHex();
        }
    });
    connect(ui->pushButton_12,&QPushButton::clicked,this,[this](){

        qDebug()<<"返回寄存器状态......";

        QByteArray dataToSend = QByteArray::fromHex("010300000001");
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            sendControlPanelHex();
        }
    });

    connect(ui->pushButton_13,&QPushButton::clicked,this,[this](){

        qDebug()<<"锁定页面1......";

        QByteArray dataToSend = QByteArray::fromHex("011000000001020081");
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            sendControlPanelHex();
        }
    });

    connect(ui->pushButton_14,&QPushButton::clicked,this,[this](){

        qDebug()<<"读取线路一电压...";

        timer->start(1000);


    });


    connect(ui->pushButton_15,&QPushButton::clicked,this,[this](){

        qDebug()<<"读取发电机运行时间";

        QByteArray dataToSend = QByteArray::fromHex("010300140001");
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            sendControlPanelHex();
        }
    });


    connect(ui->pushButton_16,&QPushButton::clicked,this,[this](){

        qDebug()<<"打开风机";

        QByteArray dataToSend = QByteArray::fromHex("010600010001");
        controlLoadQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlLoadHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlLoadHexBusy) {
            sendControlLoadHex();
        }

    });

    connect(ui->pushButton_17,&QPushButton::clicked,this,[this](){

        qDebug()<<"关闭风机";

        QByteArray dataToSend = QByteArray::fromHex("010600010000");
        controlLoadQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlLoadHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlLoadHexBusy) {
            sendControlLoadHex();
        }

    });

    connect(ui->pushButton_18,&QPushButton::clicked,this,[this](){

        qDebug()<<"设置0.2KV";

        QByteArray dataToSend = QByteArray::fromHex("010600320002");
        controlLoadQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlLoadHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlLoadHexBusy) {
            sendControlLoadHex();
        }

    });

    connect(ui->pushButton_19,&QPushButton::clicked,this,[this](){

        qDebug()<<"一键卸载";

        QByteArray dataToSend = QByteArray::fromHex("010600230001");
        controlLoadQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlLoadHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlLoadHexBusy) {
            sendControlLoadHex();
        }

    });

    connect(ui->pushButton_20,&QPushButton::clicked,this,[this](){

        qDebug()<<"查看风机状态";

        QByteArray dataToSend = QByteArray::fromHex("010300020001");
        controlLoadQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlLoadHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlLoadHexBusy) {
            sendControlLoadHex();
        }

    });


}

void MainWindow::onDataReceived(const QString &portName, const QByteArray &data)
{
    qDebug() << "Data received on port" << portName << ":" << data.toHex();
            // 处理接收到的数据

    // 提取功能码(值得注意的是：异常响应中"功能码"的最高位会设置为 1，表示错误响应,)
    uint8_t functionCode = static_cast<uint8_t>(data.at(1));
    if (functionCode >= 0x01 && functionCode <= 0x04){
        qDebug()<<"读。。。。要确保已经切换到数据所在的相应的页面,否则打印会错误!";
    }else{
        qDebug()<<"写。。。。";
    }


    if(portName == "COM5"){

        isControlLoadHexBusy = false; // 标记为负载控制不忙碌
        // 继续发送队列中的下一个数据
        sendControlLoadHex();

    }else if(portName == "COM7"){
        isControlPanelHexBusy = false; // 标记仪表控制为不忙碌
        // 继续发送队列中的下一个数据
        sendControlPanelHex();
    }


}

bool MainWindow::isUsbSerialPort(const QSerialPortInfo &portInfo)
{

    // 检查串口的名称或描述中是否包含 "USB"
        return portInfo.portName().contains("USB", Qt::CaseInsensitive) ||
               portInfo.description().contains("USB", Qt::CaseInsensitive);

}

void MainWindow::listUsbSerialPorts()
{

    // 配置和使用 SerialPortManager，设置发送端串口，打开串口等
    SerialPortManager &manager = SerialPortManager::getInstance();

    connect(&manager, &SerialPortManager::dataReceived, this, &MainWindow::onDataReceived);

    const QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

        if (serialPorts.isEmpty()) {
            qDebug() << "没有找到可用的串口设备。";
            return;
        }

        // 输出USB串口设备的信息
        for (const QSerialPortInfo &info : serialPorts) {

            if (isUsbSerialPort(info)) {
                qDebug() << "USB串口名称:" << info.portName();
                qDebug() << "描述:" << info.description();
                qDebug() << "制造商:" << info.manufacturer();
                qDebug() << "序列号:" << info.serialNumber();
                qDebug() << "系统路径:" << info.systemLocation();
                qDebug() << "是否可用:" << (info.isBusy() ? "是" : "否");
                qDebug() << "------------------------";

                manager.addSerialPort(info.portName());
                manager.configurePort(info.portName(), QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);
                manager.openPort(info.portName(), QSerialPort::ReadWrite);


            }
        }



}

QString MainWindow::get_USB_PORT_COM(const QSerialPortInfo &portInfo)
{


    return portInfo.portName();




}


void MainWindow::sendControlPanelHex()
{
    qDebug()<<"********************队列中数据个数:"<<controlPanelQueue.length()<<"*******************";
    if (!controlPanelQueue.isEmpty()) {
        QByteArray nextData = controlPanelQueue.dequeue(); // 从队列中取出数据
        isControlPanelHexBusy = true; // 标记为忙碌
        SerialPortManager::getInstance().writeData("COM7", nextData);
    }else{
        qDebug()<<"队列中数据全部发送完毕!!";
    }
}


void MainWindow::sendControlLoadHex()
{
    qDebug()<<"********************队列中数据个数:"<<controlLoadQueue.length()<<"*******************";
    if (!controlLoadQueue.isEmpty()) {
        QByteArray nextData = controlLoadQueue.dequeue(); // 从队列中取出数据
        isControlLoadHexBusy = true; // 标记为忙碌
        SerialPortManager::getInstance().writeData("COM5", nextData);
    }else{
        qDebug()<<"队列中数据全部发送完毕!!";
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
