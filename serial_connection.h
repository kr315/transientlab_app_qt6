#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>

class SerialConnection : public QObject
{
    Q_OBJECT

public:
    explicit SerialConnection(QObject *parent = nullptr);

    QStandardItemModel* listAvailablePorts();
    bool connectToPort(const QString &portName);
    void disconnectPort();
    void send8symbols();

signals:
    void dataReceived(const QByteArray &data);
    void portOpened(const QString &portName);
    void portClosed();

private slots:
    void onReadyRead();

private:
    QSerialPort serial;
    QStandardItemModel *serialModel;
};

#endif // SERIALCONNECTION_H
