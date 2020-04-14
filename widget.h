#ifndef WIDGET_H
#define WIDGET_H
#include <QLCDNumber>
#include <QWidget>
//agregar estas dos librerias //
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>//arreglos especiales para mejor rendimiento graficas
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

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void readSerial();
    int calcrc(char *ptr, int count);

    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void makeplot(double rpm,double corriente_ma);

    void processSerial(QString datos);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Widget *ui;
QByteArray data;
int c=0;
  QSerialPort *m_serial=nullptr;
  //funcion para abrir puerto serial y lo guardo en p
//alt enter para crearla en cpp
  void openSerialPort(QString p);

  QVector<double> x;
QVector<double> w;
QVector<double> y;
QVector<double> z;
void setupPlot();
};
#endif // WIDGET_H
