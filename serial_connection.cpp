#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <QUuid>

QString portName = "ttyACM1";
QSerialPort serial;

serial.setPortName(portName);  // or "COM3" on Windows
serial.setBaudRate(QSerialPort::Baud115200);
serial.setDataBits(QSerialPort::Data8);
serial.setParity(QSerialPort::NoParity);
serial.setStopBits(QSerialPort::OneStop);
serial.setFlowControl(QSerialPort::NoFlowControl);
if (!serial.open(QIODevice::ReadWrite)) {
    qCritical() << "Cannot open serial port:" << serial.errorString();
} else {
    qDebug() << "Connected to" << serial.portName();
}
connect(&serial, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);

disconnect(&serial, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);



// serial ports list
void refreshSerialPorts()
{
    serialModel = new QStandardItemModel(this);
    serialModel->clear();
    serialModel->setColumnCount(2);
    QList<QStandardItem*> row;
    QStandardItem *hiddenItem1;
    QStandardItem *displayItem;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        if (info.vendorIdentifier() != 0) {
            QString infoText = QString("%1 (%2)")
            .arg(info.description())
                .arg(info.vendorIdentifier(), 4, 16, QLatin1Char('0'));

            QString displayText = QString("%1 — %2").arg(info.portName(), infoText);
            displayItem = new QStandardItem(displayText); // shown in list
            hiddenItem1 = new QStandardItem(info.portName()); // internal data

            row << displayItem << hiddenItem1;
            serialModel->appendRow(row);
        }
    }
}

void onReadyRead() {
    QByteArray data = serial.readAll();
    ui->serial_read->append(data);
}

void send8symbols() {
    QByteArray cmd = QUuid::createUuid().toString(QUuid::Id128).left(8).toUtf8();
    if(serial.isOpen()) {
        serial.write(cmd);
        serial.flush();
        qDebug() << "send";
    }
}
