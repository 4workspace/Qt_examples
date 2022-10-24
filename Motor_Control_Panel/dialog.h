#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QChar>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();


private slots:
    void readSerial();
    void on_speed_Slider_valueChanged(int value);
    void on_Button_turn_right_clicked();
    void on_Button_turn_left_clicked();
    void writeSerial(QString command);
    void on_OnOff_button_toggled(bool checked);
    void passiveAllActions();
    void activeAllActions();
    void WriteLog(QString data);
    void refresh();
    void CheckLog();



private:
    Ui::Dialog *ui;

    QSerialPort *FTDI;
//    static const quint16 FTDI_vendor_id = 6790;       // Arduino (clone) Vendor ID
//    static const quint16 FTDI_product_id = 29987;     // Arduino (clone) Product ID

    static const quint16 FTDI_vendor_id = 1027;      //FTDI VID
    static const quint16 FTDI_product_id = 24577;     //FTDI PID

    QString FTDI_port_name;
    bool FTDI_is_available;


};
#endif // DIALOG_H
