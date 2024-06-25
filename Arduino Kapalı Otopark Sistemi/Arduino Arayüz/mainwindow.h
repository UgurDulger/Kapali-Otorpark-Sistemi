#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlQueryModel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_havalandirma_clicked();

    void on_pb_aydinlatma_clicked();

    void on_pb_havalandirma_toggled(bool checked);

    void on_pb_aydinlatma_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QSqlQuery *veritabanisorgusu;
};
#endif // MAINWINDOW_H
