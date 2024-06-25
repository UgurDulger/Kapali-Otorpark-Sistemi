#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

QSerialPort serial;
QLabel *label;
int count = 0 ;
int birincikat = 0;
int ikincikat = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Seri port ayarlarını yapıyoruz
    serial.setPortName("COM3");
    serial.setBaudRate(9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.open(QIODevice::ReadWrite)) {
      qDebug() << "Serial port baglandi";

      QObject::connect(&serial, &QSerialPort::readyRead, [](void) {
            // Veri okunur
            QByteArray veri = serial.readAll();

             // Veri QString'e dönüştürülür
            QString str = QString::fromUtf8(veri);
            bool ok;
            int data = str.toInt(&ok);

            // Eğer veri "1" ise, yangın tespit edilmiştir
            if (data == '4') {
              label->setText("Yangın sistemi çalıştı");
            }
            if(data == '5'){
               count++;
               if(count > 6){
                   QMessageBox::critical(label,"Hata","Otopark Dolu ","ok");
               }
               else if (birincikat < 3){
                   birincikat++;
               }
               else{
                   ikincikat++;
               }
             label->setText("Araç sayisi = "+ QString::number(count));
            }
          });

    } else {
      qDebug() << "Serial port baglanamadi";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_havalandirma_toggled(bool checked)
{
    if(!checked){
        //motoru kapatıyoruz
        serial.write("0");
    }
    else
    {
        //motoru açıyoruz
        serial.write("1");
    }
}

void MainWindow::on_pb_aydinlatma_toggled(bool checked)
{
    if(!checked){
        //lambaları kapatıyoruz
        serial.write("10");
    }
    else
    {
        //lambaları açıyoruz
        serial.write("2");
    }
}
