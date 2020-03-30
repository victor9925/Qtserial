#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//agregar estas dos librerias //
#include <QSerialPort>
#include <QSerialPortInfo>
//            ++             //


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;


  QSerialPort *m_serial=nullptr;
  //funcion para abrir puerto serial y lo guardo en p
//alt enter para crearla en cpp
  void openSerialPort(QString p);


};
#endif // WIDGET_H
