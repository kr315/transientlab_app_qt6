#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QTimer>

#include "serial_connection.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshWindow();

    void on_actionClose_triggered();

    void on_refreshPortsListButton_clicked();
    void on_sendDataToActivePortButton_clicked();

    void on_setActivePortButton_clicked();

    void onSerialDataReceived(const QString &data);

    void on_sendEndToActivePortButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *refreshTimer;
    SerialConnection *serialConn;
    QStandardItemModel *portsModel;



};
#endif // MAINWINDOW_H
