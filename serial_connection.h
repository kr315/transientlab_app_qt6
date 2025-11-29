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
    bool disconnectPort();
    QByteArray send8symbols();
    QByteArray sendData(QString data);

    enum {
        OFFLINE = 0,
        CONNECTED,
        INITIALIZED,
        ONLINE
    } comm_states;

signals:
    void dataReceived(const QString &data);
    void portOpened(const QString &portName);
    void portClosed();

private slots:
    void onReadyRead();

private:
    QSerialPort serial;
    QStandardItemModel *serialModel;
    QByteArray rxBuffer;

};

#endif // SERIALCONNECTION_H
