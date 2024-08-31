#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{

}

SerialPortManager::~SerialPortManager()
{
    // 清理所有串口
    foreach (QSerialPort *port, serialPorts.values()) {
        if (port->isOpen()) {
            port->close();
        }
        delete port;
    }
}

bool SerialPortManager::addSerialPort(const QString &portName)
{
    if (serialPorts.contains(portName)){
        qDebug()<<"串口已经存在";
        return false;  // 端口已经存在
    }

    QSerialPort *serialPort = new QSerialPort(this);
    serialPort->setPortName(portName);

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortManager::handleReadyRead);

    serialPorts.insert(portName, serialPort);
    return true;
}

bool SerialPortManager::removeSerialPort(const QString &portName)
{
    if (!serialPorts.contains(portName))
        return false;

    QSerialPort *serialPort = serialPorts.take(portName);
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;

    return true;
}

QSerialPort* SerialPortManager::getSerialPort(const QString &portName)
{
    //找到就返回 QSerialPort*,没找到就返回nullptr;
    //value参数中的nullpte是默认值
    return serialPorts.value(portName, nullptr);
}

bool SerialPortManager::openPort(const QString &portName, QSerialPort::OpenMode mode)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (!serialPort){
        qDebug()<<"没找到该串口";
        return false;
    }

    if (!serialPort->isOpen()) {
        qDebug()<<serialPort->portName()<<"打开";
        return serialPort->open(mode);
    }

    return false;  // 已经打开
}

void SerialPortManager::closePort(const QString &portName)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
    }
}

bool SerialPortManager::writeData(const QString &portName, const QByteArray &data)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (!serialPort || !serialPort->isOpen()){
        qDebug()<<"串口没打开!";
        return false;

    }

    QByteArray packet = data;
    //    uint16_t crc = calculateCRC16(data);//逐位计算法,效率较低，不适用于高性能场景
    //查表法:高性能的场景（如网络协议实现、文件校验等）通常使用查表法.查表法的速度明显快于逐位计算法
    uint16_t crc = ModbusProtocolParser::crc16UsingTable(data);
    qDebug()<<"CRC:"<<QString("%1").arg(crc,4,16,QLatin1Char('0')); //不足8位补0

    //假如：查表法查出来的CRC = 0xABCD，则其中高位字节是0xAB,低位字节是:0xCD
    //注意：Modbus 协议通常使用小端序(小地址存低字节,高地址存高字节),且发送的字节序要跟接收端字节序保持一致
    packet.append(crc & 0xFF);        // Append low byte of CRC,先低字节(0xCD)
    packet.append((crc >> 8) & 0xFF); // Append high byte of CRC,再高字节(0xAB)
    if (!serialPort->isOpen()) {
        qDebug() << "Serial port is not open!";
        return false;
    }

    qint64 bytesWritten = serialPort->write(packet);
    //flush() 强制将缓冲区的内容立即写入到底层设备，这就保证了数据不会滞留在发送缓冲区中。
    serialPort->flush();
    if (bytesWritten == -1) {
        qDebug() << "Failed to write to the serial port!";
    } else if (bytesWritten < packet.size()) {
        qDebug() << "Only partially wrote" << bytesWritten << "bytes to the serial port.";
    } else {
        qDebug() << "Successfully wrote" << bytesWritten << "bytes to the serial port.";
    }
    qDebug() << "Sending data:" << packet.toHex();
    return serialPort->waitForBytesWritten(500);//等待最多500毫秒直到所有数据都写入成功。此时间内没有完成写入,返回 false。
}

QByteArray SerialPortManager::readData(const QString &portName, const QByteArray &data)
{

    QSerialPort *serialPort = getSerialPort(portName);
    if (!serialPort || !serialPort->isOpen()){
        qDebug()<<"串口没打开!";
         return QByteArray();
    }
    ModbusProtocolParser parser;
    if (parser.parseRequest(data)) {
        uint8_t slaveAddress = parser.getSlaveAddress();  // 获取地址域
        uint8_t functionCode = parser.getFunctionCode();  // 获取功能域
        QByteArray parsedData = parser.getData();         // 获取数据域

        qDebug() << "Received Modbus Request. Slave Address:" << slaveAddress
                 << "Function Code:" << functionCode
                 << "Data:" << parsedData.toHex();

        // 根据功能码和数据生成响应帧（假设只回送原始数据）
        QByteArray response = parser.generateResponse(slaveAddress, functionCode, parsedData);
        serialPort->write(response);
        qDebug() << "Response sent:" << response.toHex();
    } else {
       qDebug() << "Modbus 请求解析失败，可能是 CRC 校验失败或数据格式错误";
    }
    return data;
}

void SerialPortManager::handleReadyRead()
{
    /*
     * 局部变量 serialPort: 在槽函数内部使用 serialPort 是为了访问信号的发送者（也就是具体的 QSerialPort 对象）
       通过这种方式,在处理多个串口对象时，区分哪个串口对象发出了信号，并对该对象进行相应的操作。
    */
    QSerialPort *serialPort = qobject_cast<QSerialPort *>(sender());
    if (serialPort) {

        //读取串口缓存区中数据
        QByteArray data = serialPort->readAll();
        qDebug()<<"data长度:"<<data.size()<<"串口名:"<<serialPort->portName();
        if (data.size() >= 2) {

            /*
             * 例：这时串口缓存读到的CRC为0xCDAB。
             * 则crc的倒数第一个元素0xAB左移 8 位后得到 0xAB00;
             * crc的倒数第二个元素0xCD，保持不变为 0x00CD
             * 按位或操作后，结果为 0xAB00 | 0x00CD = 0xABCD，即receivedCRC = ABCD
             * 通过 static_cast<uint8_t> 明确指定类型为"无符号" 8 位整数，这可以避免符号扩展问题。
            */
            uint16_t receivedCRC = (static_cast<uint8_t>(data.at(data.size() - 1)) << 8) |
                                   static_cast<uint8_t>(data.at(data.size() - 2));
            qDebug()<<"data:"<<data.toHex();
            data.chop(2);  // Remove CRC bytes from data
            uint16_t calculatedCRC = ModbusProtocolParser::crc16UsingTable(data);
            qDebug()<<"receivedCRC:"<<receivedCRC<<"calculatedCRC"<<calculatedCRC<<"ChopData:"<<data.toHex();
            //开始检查收到的和发送的一致性;
            if (receivedCRC == calculatedCRC) {
                qDebug() << "收到合法数据(Succesed):" << data.toHex();
                emit dataReceived(serialPort->portName(), data);
            } else {
                qDebug() << "*******CRC 检查失败(Fail),发送的和接收数据不一致!!!*******";
            }
        }


    }
}

bool SerialPortManager::configurePort(const QString &portName,
                                      QSerialPort::BaudRate baudRate,
                                      QSerialPort::DataBits dataBits,
                                      QSerialPort::Parity parity,
                                      QSerialPort::StopBits stopBits,
                                      QSerialPort::FlowControl flowControl)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (!serialPort)
        return false;

    bool success = true;

    success &= serialPort->setBaudRate(baudRate);
    success &= serialPort->setDataBits(dataBits);
    success &= serialPort->setParity(parity);
    success &= serialPort->setStopBits(stopBits);
    success &= serialPort->setFlowControl(flowControl);

    return success;
}
