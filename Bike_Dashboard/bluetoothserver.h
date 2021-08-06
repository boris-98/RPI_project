#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <qbluetoothserviceinfo.h>
#include <qbluetoothaddress.h>

#include <QtCore/QObject>
#include <QtCore/QList>

QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class BluetoothServer : public QObject
{
    Q_OBJECT

public:

    explicit BluetoothServer(QObject *parent = nullptr);
    ~BluetoothServer();

    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    void stopServer();

signals:
    void coordinatesReceived(const QString &sender, const QString &message);
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);

private slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();

private:
    QBluetoothServer *rfcommServer;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;

};



#endif // BLUETOOTHSERVER_H
