#include "mainwindow.h"
#include "./ui_mainwindow.h"

QSerialPort serial;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{



    serialModel = new QStandardItemModel(this);

    // refresh timer
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshWindow);
    refreshTimer->start(100);

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

        if (info.vendorIdentifier() == 0)
            continue;

        QString infoText = QString("%1 (%2)")
                               .arg(info.description())
                               .arg(info.vendorIdentifier(), 4, 16, QLatin1Char('0'));

        // What user sees in QListView:
        QString displayText = QString("%1 — %2").arg(info.portName(), infoText);

        // Create items
        displayItem = new QStandardItem(displayText); // shown in list
        hiddenItem1 = new QStandardItem(info.portName()); // internal data

        // Add row (only first column is displayed in QListView)

        row << displayItem << hiddenItem1;
        serialModel->appendRow(row);
    }
    displayItem = new QStandardItem("None");
    hiddenItem1 = new QStandardItem("None");
    row << displayItem << hiddenItem1;
    serialModel->appendRow(row);
}

void MainWindow::on_serialListView_clicked(const QModelIndex &index)
{
    if(serial.isOpen()) {
        serial.close();
    }
}


void MainWindow::on_pushButton_clicked()
{
    MainWindow::refreshSerialPorts();
}


void MainWindow::on_pushButton_2_clicked()
{
    qint8 selected_item = 0;
    QModelIndex selected = ui->serialListView->currentIndex();
    QString portName = "";
    if(selected.isValid()) {
        ui->activePort->setText(serialModel->itemFromIndex(selected)->text());
        portName = serialModel->item(selected.row(), 1)->text();

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


    } else {
        ui->activePort->setText("None");
    }
    qDebug() << portName;
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
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    MainWindow::send8symbols();
}

