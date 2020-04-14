#include "widget.h"
#include "ui_widget.h"
#include "string.h"
#include <QDebug>//para las opciones con el ui
#include <QSerialPort>//comserial
#include <QSerialPortInfo>//comserial
#include <QMessageBox>
#include <stdlib.h>
#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define ETB 0x17
#define CX 'C'
#define BLK 0x00
char neg;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        if(info.manufacturer()==NULL){}else{
      qDebug() << "name: " << info.portName();
      qDebug() << "Descripcion: " << info.description();
      qDebug() << "Manufacturer: " <<info.manufacturer();
      qDebug() << "Vendor: " <<info.vendorIdentifier();
      qDebug() << "produt: "  <<info.productIdentifier();
      ui->comboBox->addItem(info.portName());//escribir port name en ui

}
    }
m_serial= new QSerialPort(this);
setupPlot();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openSerialPort(QString p)
{

            if(m_serial->isOpen()){
        ui->pushButton_4->setText("OPEN PORT");
        m_serial->close();
    }
    disconnect(m_serial,SIGNAL(readyRead()),this,SLOT(readSerial()));
    m_serial->setPortName(p);
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

connect(m_serial,SIGNAL(readyRead()),this,SLOT(readSerial()));


    if(m_serial->open(QIODevice::ReadWrite)){
        
//        ui->comboBox->setDisabled(TRUE);
        ui->pushButton_3->setText("ClOSE PORT") ;   }else{
        QMessageBox::critical(this,tr("Error"),m_serial->errorString());
    }

}

void Widget::setupPlot()
{

    x.resize(101);
    y.resize(101);
    z.resize(101);
    w.resize(101);

    for(int i=0;i<101;++i){
        x[i] = (double)i;
        w[i] = (double)4;
        y[i] = (double)2;
        z[i] = (double)i;
    }
    ui->customPlot->addGraph(ui->customPlot->xAxis,ui->customPlot->yAxis);
    ui->customPlot->addGraph(ui->customPlot->xAxis,ui->customPlot->yAxis2);
    ui->customPlot->graph(0)->setData(x,y);
    ui->customPlot->graph(1)->setData(z,w);
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(1)->setBrush(QBrush(QColor(250,0,0,0)));
    ui->customPlot->graph(0)->setPen(QPen(Qt::DotLine));
    ui->customPlot->graph(0)->setName("RPM");
    ui->customPlot->graph(1)->setName("Corriente");
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0,0,new QCPTextElement(ui->customPlot,"velocidad",QFont("sans",12,QFont::Bold)));
    ui->customPlot->legend->setVisible(true);


    QFont legenfont=font();
    legenfont.setPointSize(9);
    ui->customPlot->legend->setFont(legenfont);

    //crear etiquetas
    ui->customPlot->xAxis->setLabel("Relative Time");
    ui->customPlot->yAxis->setLabel("RPM");
    ui->customPlot->xAxis->setRange(0,100);
    ui->customPlot->yAxis->setRange(0,5000);

    ui->customPlot->xAxis2->setLabel("mA");
    ui->customPlot->yAxis2->setRange(0,1000);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->replot();


// generate some data:
QVector<double> x(101), y(101); // initialize with entries 0..100
for (int i=0; i<101; ++i)
{
  x[i] = i/50.0 - 1; // x goes from -1 to 1
  y[i] = x[i]*x[i]; // let's plot a quadratic function
}
// create graph and assign data to it:
ui->customPlot_2->addGraph();
ui->customPlot_2->graph(0)->setData(x, y);
ui->customPlot_2->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
// give the axes some labels:
ui->customPlot_2->xAxis->setLabel("x");
ui->customPlot_2->yAxis->setLabel("y");
// set axes ranges, so we see all data:
ui->customPlot_2->xAxis->setRange(-1, 1);
ui->customPlot_2->yAxis->setRange(0, 1);
ui->customPlot_2->replot();
}
void Widget::makeplot(double rpm, double corriente_ma)
{
    for(int i=0;i<100;++i){
    y[i]=y[i+1];
    w[i]=w[i+1];
    }
    y[100]=rpm;
    w[100]=corriente_ma;
    ui->customPlot->graph(0)->setData(x,y);
    ui->customPlot->graph(1)->setData(z,w);
    ui->customPlot->replot();
}




void Widget::readSerial()
{
//# de paquete
static int cnt=0;

char soh=SOH;

char eot=EOT;

char ack[6];
//DATA
char nak[5];
char byte[126];
//numero paquete
char num=0x01+cnt;
//send text

//crc calculo
//char *p=byte;


//listo para recibir

//guardo serial en un arreglo

QByteArray serialData = m_serial->readAll();

//cabecera 1BYTE
if(serialData[0]==soh){
qDebug()<<"$$"<< soh;
qDebug() << "---" << cnt;

    //numero paquete 2BYTE
    if(serialData[1]==num){
        cnt++;
      qDebug()<<"numpack"<< num;
        //num=num+cnt;
        neg=~num;

        //numero paquete negado 3BYTE
        if(serialData[2]==neg){

            qDebug() <<neg<<"este es el negado";
            //guardar DATA en "BYTE" 4BYTE
            for(int i=0;i<serialData.length()-5;i++){
                byte[i]=serialData[i+3];}

          
            if(strlen(serialData)==133){
                data.append(byte);
            }else{
                ;
                ack[0]=0x15;
                m_serial->write(nak,1);
                qDebug() <<"llegó data sin byte de 133";

            }
            qDebug() << "---DATAAAAA---"<<strlen(data);
            qDebug() << "____SERIALSIZE______"<<strlen(serialData.mid(3));

            crcp.append(serialData.mid(131));
            ack[0]=0x06;
           m_serial->write(ack,1);
//      char z=calcrc(p,128);
//            if(z==crcp){
//         qDebug() << "-----  "<<z <<"****"<< crcp;

//            }
        }
    }
}else{if(serialData[0]==eot){
        ack[0]=0x06;
       m_serial->write(ack,1);
    }}
m_serial->clear();





//if(serialData.at(serialData.length()-1)=='\r'){

}

int Widget::calcrc(char *ptr, int count)
{

    int  crc;
    char i;
    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    return (crc);
}



void Widget::processSerial(QString datos)
{//prueba mirar como llegan datos
    qDebug() <<"datos en process " <<datos;
    if(datos.contains("\n\r")){
    QStringList lista=datos.split("\n\r");
    QString a =lista.at(0);
    makeplot(a.toDouble()*60,100.0);}}


void Widget::on_pushButton_clicked()
{
char texto[32];
readSerial();

sprintf(texto,"%c",CX);

m_serial->write(texto,strlen(texto));

    }
void Widget::on_pushButton_2_clicked()
{
    int a=0;
    int b=0;

    a=ui->lineEdit->text().toInt();
    b=ui->lineEdit_2->text().toInt();

    c=a+b;
    ui->plainTextEdit->appendPlainText("la suma es: "+QString::number(c));
    qDebug() <<"la suma es:" << c;}
void Widget::on_pushButton_3_clicked()
{

    QString puerto = ui->comboBox->currentText();
    openSerialPort(puerto);

}
void Widget::on_pushButton_4_clicked()
{
static char n='C';

char texto[32];

sprintf(texto,"%c",n);
qDebug() << n;
if(m_serial->isOpen()){
m_serial->write(texto,strlen(texto));
}}
void Widget::on_pushButton_5_clicked()
{
    double a=0;
    double b=0;

    a=ui->lineEdit->text().toDouble();
    b=ui->lineEdit_2->text().toDouble();
    makeplot(a,b);}
void Widget::on_pushButton_6_clicked()
{
int ab=ACK;
    char texto[32];

    sprintf(texto,"%x",ab);
    if(m_serial->isOpen()){
    m_serial->write(texto,strlen(texto));}}
void Widget::on_pushButton_7_clicked()
{
    char ab=ACK;
        char texto[32];

        sprintf(texto,"%c",ab);
        if(m_serial->isOpen()){
        m_serial->write(texto,strlen(texto));
        }

}
