#include "bluetoothserver.h"

#include <qbluetoothserver.h>

// Service UUID
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

BluetoothServer::BluetoothServer(QObject *parent) : QObject(parent), rfcommServer(nullptr){}

BluetoothServer::~BluetoothServer()
{
    stopServer();
}

void BluetoothServer::startServer(const QBluetoothAddress &localAdapter)
{

    //-------------------- creating server --------------------//
    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
    bool result = rfcommServer->listen(localAdapter);
    if(!result)
    {
        qWarning() << "Cannot bind server to" << localAdapter.toString();
        return;
    }
    //---------------------------------------------------------//

    //------- Class Uuid must contain at least 1 entry -------//
    QBluetoothServiceInfo::Sequence classId;

    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);
    classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);
    //--------------------------------------------------------//

    // Service name
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Qt bt Server"));
    // Description
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Bluetooth server for GPS coordinates"));
    // Provider
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));     // <__________________

    // Service UUID set
    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    //--------------- Service Discoverability ----------------//
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);
    //--------------------------------------------------------//

    //--------------- Protocol descriptor list ---------------//
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocolDescriptorList);
    //--------------------------------------------------------//

    // Register service
    serviceInfo.registerService(localAdapter);

}

void BluetoothServer::stopServer()
{
    serviceInfo.unregisterService();

    qDeleteAll(clientSockets);

    delete rfcommServer;
    rfcommServer = nullptr;

}

void BluetoothServer::clientConnected()
{
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if(!socket)
        return;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    clientSockets.append(socket);
    emit clientConnected(socket->peerName());

}

void BluetoothServer::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if(!socket)
        return;

    emit clientDisconnected(socket->peerName());

    clientSockets.removeOne(socket);
    socket->deleteLater();

}

void BluetoothServer::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if(!socket)
        return;

    while(socket->canReadLine())
    {
        QByteArray line = socket->readLine().trimmed();

        if(QString::fromUtf8(line.constData(), line.length()).left(6) == "$GPGGA")
            emit coordinatesReceived(socket->peerName(), QString::fromUtf8(line.constData()));

    }

}


