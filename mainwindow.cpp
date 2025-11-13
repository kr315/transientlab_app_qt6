#include "mainwindow.h"
#include "./ui_mainwindow.h"

QSerialPort serial;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // serial port connection
    QString portName = "ttyACM1";

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
    // disconnect(&serial, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
    // refresh timer
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshWindow);
    refreshTimer->start(100);

    serialModel = new QStandardItemModel(this);

    // list of available serial ports
    refreshSerialPorts();



    ui->setupUi(this);
    ui->serialListView->setModel(serialModel);

    // assign model to the listView
    // ui->serialListView->setModel(serialListModel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::refreshWindow()
{

}

// serial ports list
void MainWindow::refreshSerialPorts()
{
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

void MainWindow::onReadyRead() {
    QByteArray data = serial.readAll();
    ui->serial_read->append(data);
}

void MainWindow::send8symbols() {
    QByteArray cmd = QUuid::createUuid().toString(QUuid::Id128).left(8).toUtf8();
    if(serial.isOpen()) {
        serial.write(cmd);
        serial.flush();
        qDebug() << "send";
    }
}

void MainWindow::on_refreshPortsListButton_clicked()
{
    MainWindow::refreshSerialPorts();
}


void MainWindow::on_sendDataToActivePortButton_clicked()
{
    send8symbols();
}

