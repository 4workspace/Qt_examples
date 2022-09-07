#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

void findPorts()
{
    if(!QSerialPortInfo::availablePorts().empty())
    {
        qDebug() << "Available serial ports: " << QSerialPortInfo::availablePorts().length() <<"\n";

        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            qDebug() << "Port name: " << serialPortInfo.portName();
            qDebug() << "Has Vendor ID: " << serialPortInfo.hasVendorIdentifier();

            if(serialPortInfo.hasVendorIdentifier())
            {
                qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
            }

            qDebug() << "Has product ID: " << serialPortInfo.hasProductIdentifier();

            if(serialPortInfo.hasProductIdentifier())
            {
                qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
            }

            qDebug() << "\n";
         }
    }
}
