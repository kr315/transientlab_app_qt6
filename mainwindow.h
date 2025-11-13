#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QStandardItemModel>
#include <QUuid>


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
    void on_actionClose_triggered();
    void refreshSerialPorts();
    void on_serialListView_clicked(const QModelIndex &index);
    void refreshWindow();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void onReadyRead();
    void send8symbols();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QStringListModel *serialListModel;
    QTimer *refreshTimer;
    QStandardItemModel *serialModel;


};
#endif // MAINWINDOW_H
