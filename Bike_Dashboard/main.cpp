#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickItem>
#include <QGeoCoordinate>
#include <QQuickView>
#include <wiringPi.h>
#include "bluetoothmain.h"
#include "hallmeasure.h"


int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    BluetoothMain gpsReciever;
    HallMeasure wheelHall;

    QQmlApplicationEngine engine;

    QQmlComponent component(&engine, "qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);

    QObject *object = component.create();
    gpsReciever.getObject(object);
    wheelHall.getObject(object);


    return app.exec();
}
