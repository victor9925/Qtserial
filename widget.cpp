#include "widget.h"
#include "ui_widget.h"

#include <QDebug>//para las opciones con el ui
#include <QSerialPort>//comserial
#include <QSerialPortInfo>//comserial
#include <QMessageBox>

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
QByteArray serialData = m_serial->readAll();
 qDebug() <<serialData;
if(serialData.at(serialData.length()-1)=='\r'){
    processSerial(serialData);
}
}
void Widget::processSerial(QString datos)
{//prueba mirar como llegan datos
    qDebug() <<"datos en process " <<datos;
    if(datos.contains("\n\r")){
    QStringList lista=datos.split("\r\n");
    QString a =lista.at(0);
    makeplot(a.toDouble()*60,100.0);
    }
}

void Widget::on_pushButton_clicked()
{
    static int cont=0;
    qDebug() << cont++;
    ui->lcdNumber->display(QString::number(cont));}
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
static int n=0;
char texto[32];

sprintf(texto,"%d",n++);
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
    int pwm=ui->lineEdit_3->text().toInt();
    char texto[32];

    sprintf(texto,"%d",pwm);
    if(m_serial->isOpen()){
    m_serial->write(texto,strlen(texto));
    }
}
