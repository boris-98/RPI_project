#include "bluetoothmain.h"
#include "bluetoothserver.h"

#include <qbluetoothuuid.h>
#include <qbluetoothserver.h>
#include <qbluetoothlocaldevice.h>

#include <QTimer>
#include <QDebug>
#include <cmath>

BluetoothMain::BluetoothMain() : QObject()
{
    server = new BluetoothServer(this);
    connect(server, SIGNAL(clientConnected(QString)), this, SLOT(clientConnected(QString)));
    connect(server, SIGNAL(clientDisconnected(QString)), this, SLOT(clientDisconnected(QString)));
    connect(server, SIGNAL(coordinatesReceived(QString, QString)), this, SLOT(getCoordinates(QString, QString)));

    server->startServer();

    localName = QBluetoothLocalDevice().name();
    oldLatitude = 0.00;
    oldLongitude = 0.00;
    newLatitude = 0.00;
    newLongitude = 0.00;

};

BluetoothMain::~BluetoothMain()
{
    delete server;

}

void BluetoothMain::clientConnected(const QString &name)
{
    qDebug() << QString::fromLatin1("%1 has connected!").arg(name);

}

void BluetoothMain::clientDisconnected(const QString &name)
{
    qDebug() << QString::fromLatin1("%1 has disconnected.").arg(name);

}

void BluetoothMain::getObject(QObject *obj)
{
    objekat = obj;

}

void BluetoothMain::resetCoords()
{
    oldLatitude = 0.0;
    oldLongitude = 0.0;

}

void BluetoothMain::getCoordinates(const QString &sender, const QString &message)
{
    //qDebug() << QString::fromLatin1("Sender is: %1").arg(sender);

    if(QString(message).left(6) == "$GPGGA")
    {
        qDebug() << message;

        newLatitude = (message.section(',', 2, 2).left(2)).toDouble() + (message.section(',', 2, 2).midRef(2)).toDouble() / 60.00;  // ddmm.mmmm  -> dd + mm.mmmm/60 = latitude
        newLongitude = (message.section(',', 4, 4).left(3)).toDouble() + (message.section(',', 4, 4).midRef(3)).toDouble() / 60.00; // dddmm.mmmm -> ddd + mm.mmmm/60 = longitude

        if((abs( sqrt(pow(newLatitude, 2) + pow(newLongitude, 2)) - sqrt(pow(oldLatitude, 2) + pow(oldLongitude, 2)) ) > 0.0001) && newLatitude > 0 && newLongitude > 0)  // don't update map for small movements
        {
            QVariant lat(newLatitude);
            QVariant lon(newLongitude);
            QMetaObject::invokeMethod(objekat, "addCoordinatesToMap", Q_ARG(QVariant, lat), Q_ARG(QVariant, lon));
            qDebug() << QString::fromLatin1("Sender is: %1").arg(sender);
            //qDebug() << message;
            //qDebug() << QString("%l").arg(newLatitude, 0, 'g', 30);
            oldLatitude = newLatitude;
            oldLongitude = newLongitude;
        }
    }


}
