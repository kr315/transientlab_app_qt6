#include "serial_connection.h"
#include <QUuid>
#include <QDebug>



SerialConnection::SerialConnection(QObject *parent)
    : QObject(parent),
    serialModel(new QStandardItemModel(this))
{
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    comm_states = OFFLINE;
}

QStandardItemModel* SerialConnection::listAvailablePorts()
{
    serialModel->clear();
    serialModel->setColumnCount(2);

    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        if (info.vendorIdentifier() != 0) {

            QString infoText = QString("%1 (%2)")
            .arg(info.description())
                .arg(info.vendorIdentifier(), 4, 16, QLatin1Char('0'));

            QString displayText = QString("%1 — %2")
                                      .arg(info.portName(), infoText);

            QList<QStandardItem*> row;
            row << new QStandardItem(displayText)
                << new QStandardItem(info.portName()); // hidden column

            serialModel->appendRow(row);
        }
    }

    return serialModel;
}

bool SerialConnection::connectToPort(const QString &portName)
{
    if (serial.isOpen()) {
        comm_states = OFFLINE;
        serial.close();
    }

    serial.setPortName(portName);

    if (!serial.open(QIODevice::ReadWrite)) {
        qCritical() << "Cannot open serial port:" << serial.errorString();
        return false;
    } else {
        connect(&serial, &QSerialPort::readyRead, this, &SerialConnection::onReadyRead);
        emit portOpened(portName);
        comm_states = ONLINE;
        return true;
    }
}

bool SerialConnection::disconnectPort()
{
    if (serial.isOpen()) {
        disconnect(&serial, &QSerialPort::readyRead, this, &SerialConnection::onReadyRead);
        serial.close();
        emit portClosed();
        comm_states = OFFLINE;
        return true;
    } else {
        return false;
    }
}

QByteArray SerialConnection::send8symbols()
{


    QByteArray cmd = QUuid::createUuid().toString(QUuid::Id128).left(8).toUtf8();
    if (serial.isOpen()) {
        serial.write(cmd);
        serial.flush();
    }
    return cmd;
}

QByteArray SerialConnection::sendData(QString data)
{

    QByteArray cmd = data.toUtf8();
    if (serial.isOpen()) {
        serial.write(cmd);
        serial.flush();
    }
    return cmd;
}

void SerialConnection::onReadyRead()
{
    rxBuffer.append(serial.readAll());

    const char START = '@';
    const char END   = '!';
    const int MAX_BUFFER = 256;

    // Clear oversized buffer
    if (rxBuffer.size() > MAX_BUFFER) {
        qWarning() << "Serial buffer cleared (no valid frame within 256 bytes)";
        rxBuffer.clear();
        return;
    }

    while (true)
    {
        // Find start marker '@'
        int startIndex = rxBuffer.indexOf(START);

        if (startIndex == -1) {
            // No start found → wait for more data
            return;
        }

        // Drop anything before start marker
        if (startIndex > 0)
            rxBuffer.remove(0, startIndex);

        // Now buffer begins with '@'
        // Find end marker '!'
        int endIndex = rxBuffer.indexOf(END, 1);  // search AFTER '@'

        if (endIndex == -1) {
            // No end marker yet → wait for more data
            return;
        }

        // Make sure payload is NOT empty
        int payloadLength = endIndex - 1;
        if (payloadLength <= 0) {
            rxBuffer.remove(0, 1);
            continue;
        }

        // Extract full frame: "@PAYLOAD!"
        QByteArray frame = rxBuffer.mid(0, endIndex + 1);

        rxBuffer.remove(0, endIndex + 1);

        emit dataReceived(frame);
        qDebug() << frame;

    }
}

