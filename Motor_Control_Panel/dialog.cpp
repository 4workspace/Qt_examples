#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QFile>
#include <find_ports.h>
#include <QThread>


volatile int temp_buff = 0;
QString send_data;
QString motor_direction;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    qDebug()<<"The constructor method is called.";
    ui->setupUi(this);
    ui->OnOff_button->setIconSize(QSize(80,80));
    ui->OnOff_button->setIcon(QIcon("C:\\Users\\Point\\Desktop\\Qt\\TargetApp\\ICON\\0.jpg"));

    findPorts();

    FTDI = new QSerialPort;

    FTDI_port_name = "";
    FTDI_is_available = false;


    CheckLog();

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.vendorIdentifier() == FTDI_vendor_id)
            {
                if(serialPortInfo.productIdentifier() == FTDI_product_id )
                {
                    FTDI_port_name = serialPortInfo.portName();
                    FTDI_is_available = true;
                }
            }
        }

    }

    if(FTDI_is_available)
    {   // initially settings for usart
        FTDI->setPortName(FTDI_port_name);
        FTDI->open(QSerialPort::ReadWrite);
        FTDI->setBaudRate(QSerialPort::Baud9600);
        FTDI->setDataBits(QSerialPort::Data8);
        FTDI->setParity(QSerialPort::NoParity);
        FTDI->setStopBits(QSerialPort::OneStop);
        FTDI->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(FTDI, SIGNAL(readyRead()), this, SLOT(readSerial()));

        WriteLog("Connected controller.");

    }
    else
    {   // warning window
        QMessageBox::warning(this, "Warning", "Couldn't find the FTDI!");
    }

    Dialog::passiveAllActions();
}

Dialog::~Dialog()
{
    writeSerial(QString("f%1").arg(temp_buff));     // Çalışmıyor, sebebini bilmiyorum.
    if(FTDI->isOpen())
    {

        FTDI->close();
        qDebug()<<"The destructor method is called.";
    }
    delete ui;
}



void Dialog::on_OnOff_button_toggled(bool checked)
{

    if(checked == TRUE)
    {

        Dialog::writeSerial(QString("o%1").arg(temp_buff));
        Dialog::activeAllActions();
        ui->OnOff_button->setIconSize(QSize(80,80));
        ui->OnOff_button->setIcon(QIcon("C:\\Users\\Point\\Desktop\\Qt\\TargetApp\\ICON\\1.jpg"));

        WriteLog("Motor is ON!");
        qDebug() << "ON";
    }

    else
    {
       Dialog::writeSerial(QString("f%1").arg(temp_buff));
       Dialog::passiveAllActions();
       ui->OnOff_button->setIconSize(QSize(80,80));
       ui->OnOff_button->setIcon(QIcon("C:\\Users\\Point\\Desktop\\Qt\\TargetApp\\ICON\\0.jpg"));

       qDebug() << "OFF";

    }
}

void Dialog::on_speed_Slider_valueChanged(int value)
{
    temp_buff = value;
    Dialog::writeSerial(QString("%1").arg(value));
    ui->speed_value->setText(QString("%1").arg(value));
    refresh();
    //qDebug() << value;
}

void Dialog::on_Button_turn_right_clicked()
{
    motor_direction = "Motor turning right.";
    Dialog::writeSerial(QString("%1%2").arg("r").arg(temp_buff));

    qDebug() << "Clicked turn right.";
    WriteLog("Motor turning right.");
}

void Dialog::on_Button_turn_left_clicked()
{
    motor_direction = "Motor turning left.";
    Dialog::writeSerial(QString("%1%2").arg("l").arg(temp_buff));

    qDebug() << "Clicked turn left.";
    WriteLog("Motor turning left.");
}

void Dialog::CheckLog(void)                                             // when program is starting, to delete the contents of the old file.
{
    QFile logFile("data.log");
    if(logFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       QTextStream stream(&logFile);
       stream <<"";
       logFile.close();
       qDebug() << "Log file created.";
    }
}

void Dialog::WriteLog(QString data)
{
    QFile logFile("data.log");
    if(logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&logFile);
        stream << data <<"\n";
        logFile.close();
        qDebug() << "Writing finished";
    }

    refresh();
}

void Dialog::refresh()                                                 // this method created for log print to messageBox
{

    QFile *file = new QFile ("data.log");
    if (file->open (QIODevice::ReadOnly) == true)
    {
        ui->message_box->setText(QString (file->readAll ()));
        file->close ();
    }
}

void Dialog::passiveAllActions()
{
    qDebug()<<"passiveAllActions() is called!";
    //ui->message_box->setText("Passive all actions!");
    ui->Button_turn_left->setEnabled(false);
    ui->Button_turn_right->setEnabled(false);
    ui->message_box->setEnabled(false);
    ui->speed_Slider->setEnabled(false);
    ui->speed->setEnabled(false);
    ui->speed_value->setEnabled(false);
    ui->temp_value_lcd->setEnabled(false);
    ui->temprature->setEnabled(false);
    ui->rpm->setEnabled(false);
    ui->rpm_value_lcd->setEnabled(false);
    ui->message->setEnabled(false);
    ui->motor_direction->setEnabled(false);
    ui->percent->setEnabled(false);

}

void Dialog::activeAllActions()
{
    motor_direction = "Motor turning right.";
    qDebug()<<"passiveAllActions() is called!";
    //ui->message_box->setText("Activeted all actions!");
    ui->Button_turn_left->setEnabled(true);
    ui->Button_turn_right->setEnabled(true);
    ui->message_box->setEnabled(true);
    ui->speed_Slider->setEnabled(true);
    ui->speed->setEnabled(true);
    ui->speed_value->setEnabled(true);
    ui->temp_value_lcd->setEnabled(true);
    ui->temprature->setEnabled(true);
    ui->rpm->setEnabled(true);
    ui->rpm_value_lcd->setEnabled(true);
    ui->message->setEnabled(true);
    ui->motor_direction->setEnabled(true);
    ui->percent->setEnabled(true);

}

void Dialog::writeSerial(QString command)
{
    if(FTDI->isWritable()){
        FTDI->write(command.toStdString().c_str());
    }else
        qDebug() << "Couldn't write to serial!";
}

void Dialog::readSerial()
{
    if(FTDI->isReadable())
    {
        QString serialData = FTDI->readAll();
        //QString data = QString::fromStdString(serialData.toStdString());

        if(serialData[-1] == NULL)
        {
           qDebug()<<"data: " << serialData <<"\n" ;
        }
    }
}



