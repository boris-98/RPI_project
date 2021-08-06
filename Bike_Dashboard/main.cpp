#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickItem>
#include <QGeoCoordinate>
#include <QQuickView>

#include "bluetoothmain.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    BluetoothMain gpsReciever;

    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;

    QQmlComponent component(&engine, "qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);

    QObject *object = component.create();
    gpsReciever.getObject(object);


    return app.exec();
}
