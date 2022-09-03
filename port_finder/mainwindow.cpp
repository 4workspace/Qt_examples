#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QTextBrowser>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        setWindowTitle("Serial Port Searcher");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_searchPorts_clicked()
{
    QString filename = "C:\\Users\\Point\\Desktop\\Qt\\port_finder\\serial_ports.txt";
    QFile file(filename);
    QTextStream stream(&file);

    if(QSerialPortInfo::availablePorts().empty())
    {
        ui->textBrowser->setText("Doesn't available any ports!");
    }

    else if (file.open(QIODevice::ReadWrite) && QSerialPortInfo::availablePorts().length())
    {
        stream << "Available serial ports: " << QSerialPortInfo::availablePorts().length() <<"\n\n";

        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            stream << "Port name: " << serialPortInfo.portName()<<"\n";
            stream << "Has Vendor ID: " << serialPortInfo.hasVendorIdentifier()<<"\n";

            if(serialPortInfo.hasVendorIdentifier())
            {
                stream << "Vendor ID: " << serialPortInfo.vendorIdentifier()<<"\n";
            }

            stream << "Has product ID: " << serialPortInfo.hasProductIdentifier()<<"\n";

            if(serialPortInfo.hasProductIdentifier())
            {
                stream << "Product ID: " << serialPortInfo.productIdentifier()<<"\n";
            }

            stream << "\n";
         }

        stream << "\n";
        ui->textBrowser->setText(file.readAll());


    }

    file.close();
}

