#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

#include <QTimer>


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
    void refreshWindow();

    void refreshSerialPorts();
    void onReadyRead();
    void send8symbols();

    void on_refreshPortsListButton_clicked();
    void on_sendDataToActivePortButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *refreshTimer;


};
#endif // MAINWINDOW_H
