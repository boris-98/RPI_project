#ifndef BLUETOOTHMAIN_H
#define BLUETOOTHMAIN_H

#include <qbluetoothserviceinfo.h>
#include <qbluetoothsocket.h>
#include <qbluetoothhostinfo.h>

#include <QDebug>

QT_USE_NAMESPACE

class BluetoothServer;

class BluetoothMain : public QObject
{
    Q_OBJECT

public:
    BluetoothMain();
    ~BluetoothMain();

    void getObject(QObject *obj);


private slots:
    void getCoordinates(const QString &sender, const QString &message);

    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    //void clientDisconnected();
    //void connected(const QString &name);

private:
    BluetoothServer *server;
    QString localName;
    QObject *objekat;
    double oldLatitude, newLatitude;
    double oldLongitude, newLongitude;



};





#endif // BLUETOOTHMAIN_H
