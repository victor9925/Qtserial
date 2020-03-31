#include "widget.h"
#include "ui_widget.h"

#include <QDebug>//para las opciones con el ui
#include <QSerialPort>//comserial
#include <QSerialPortInfo>//comserial

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
      qDebug() << "name: " << info.portName();
      qDebug() << "Descripcion: " << info.description();
      qDebug() << "Manufacturer: " <<info.manufacturer();
      qDebug() << "Vendor: " <<info.vendorIdentifier();
      qDebug() << "produt: "  <<info.productIdentifier();
      ui->comboBox->addItem(info.portName());//escribir port name en ui

    }


}

Widget::~Widget()
{
    delete ui;
}

void Widget::openSerialPort(QString p)
{
    if(m_serial->isOpen()){
        m_serial->close();
    }
    m_serial->setPortName(p);
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
}
//pr que no sirve git


void Widget::on_pushButton_clicked()
{
    static int cont=0;
    qDebug() << cont++;
    ui->lcdNumber->display(QString::number(cont));
}

void Widget::on_pushButton_2_clicked()
{
    int a=0;
    int b=0;

    a=ui->lineEdit->text().toInt();
    b=ui->lineEdit_2->text().toInt();

    c=a+b;
    ui->plainTextEdit->appendPlainText("la suma es: "+QString::number(c));
    qDebug() <<"la suma es:" << c;

}
