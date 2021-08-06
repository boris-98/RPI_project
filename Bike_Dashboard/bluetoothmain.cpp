#include "bluetoothmain.h"
#include "bluetoothserver.h"

#include <qbluetoothuuid.h>
#include <qbluetoothserver.h>
#include <qbluetoothlocaldevice.h>

#include <QTimer>
#include <QDebug>

BluetoothMain::BluetoothMain() : QObject()
{
    server = new BluetoothServer(this);
    connect(server, SIGNAL(clientConnected(QString)), this, SLOT(clientConnected(QString)));
    connect(server, SIGNAL(clientDisconnected(QString)), this, SLOT(clientDisconnected(QString)));
    connect(server, SIGNAL(coordinatesReceived(QString, QString)), this, SLOT(getCoordinates(QString, QString)));

    server->startServer();

    localName = QBluetoothLocalDevice().name();

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

void BluetoothMain::getCoordinates(const QString &sender, const QString &message)
{
    qDebug() << QString::fromLatin1("Sender is: %1").arg(sender);

    if(QString(message).left(6) == "$GPGGA")
    {
        double latitude = (message.section(',', 2, 2).left(2)).toDouble() + (message.section(',', 2, 2).midRef(2)).toDouble() / 60;  // ddmm.mmmm  -> dd + mm.mmmm/60 = latitude
        double longitude = (message.section(',', 4, 4).left(3)).toDouble() + (message.section(',', 4, 4).midRef(3)).toDouble() / 60; // dddmm.mmmm -> ddd + mm.mmmm/60 = longitude
        QVariant a(latitude);
        QVariant b(longitude);
        QMetaObject::invokeMethod(objekat, "addCoordinatesToMap", Q_ARG(QVariant, a), Q_ARG(QVariant, b));
        qDebug() << message;
        qDebug() << a.toString();
    }


}
