#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);


    // refresh timer
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshWindow);
    refreshTimer->start(100);

    // serial connection
    serialConn = new SerialConnection(this);
    portsModel = serialConn->listAvailablePorts();
    ui->serialListView->setModel(portsModel);
    connect(serialConn, &SerialConnection::dataReceived, this, &MainWindow::onSerialDataReceived);
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


void MainWindow::on_refreshPortsListButton_clicked()
{
    portsModel = serialConn->listAvailablePorts();
    ui->serialListView->setModel(portsModel);
}

void MainWindow::on_setActivePortButton_clicked()
{
    QModelIndex idx = ui->serialListView->currentIndex();
    if (!idx.isValid()) {
        ui->serial_read->append("Select a port first.");
        return;
    }

    QString portName = portsModel->item(idx.row(), 1)->text();
    serialConn->disconnectPort();

    if (serialConn->connectToPort(portName)) {
        ui->serial_read->append("Connected to " + portName);
        ui->activePort->setText(portName);
    } else {
        ui->serial_read->append("Failed to connect to " + portName);
        ui->activePort->setText("None");
    }
}

void MainWindow::on_sendDataToActivePortButton_clicked()
{
    QByteArray test_data = serialConn->send8symbols();
    ui->serial_read->append("TX: " + test_data);
}

void MainWindow::onSerialDataReceived(const QString &data)
{
    ui->serial_read->append("RX: " + data);
}

void MainWindow::on_sendEndToActivePortButton_clicked()
{
    serialConn->sendData("!");
    qDebug() << "send endline";
}

