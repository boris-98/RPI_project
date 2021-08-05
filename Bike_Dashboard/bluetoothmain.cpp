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

void BluetoothMain::getCoordinates(const QString &sender, const QString &message)
{
    qDebug() << QString::fromLatin1("Sender is: %1").arg(sender);

    if(QString(message).left(6) == "$GPGGA")
    {
        qDebug() << QString(message);
    }

}
