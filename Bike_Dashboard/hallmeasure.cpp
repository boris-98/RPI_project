#include "hallmeasure.h"

char HallMeasure::passCount = 0;
int HallMeasure::lastMillis = 0;
int HallMeasure::currentMillis = 0;
double HallMeasure::wheelRadius = 0.35;
QObject* HallMeasure::objekat;

HallMeasure::HallMeasure() : QObject ()
{
    if(wiringPiSetup() < 0)
          qDebug() << "ERROR: WiringPi is not initialized!";

    wiringPiISR(0, INT_EDGE_RISING, &hallInterrupt);

};

HallMeasure::~HallMeasure(){}

void HallMeasure::getObject(QObject *obj)
{
    objekat = obj;

}

void HallMeasure::hallInterrupt()
{
    qDebug() << "Interrupt in class has happened";

    QString millis = QDateTime::currentDateTime().toString("ss,zzz");
    currentMillis = (millis.left(2)).toInt() * 1000 + (millis.right(3)).toInt();

    passCount++;

    if(passCount == 2)
    {
        int diff = currentMillis - lastMillis;
        if (diff < 0)
            diff = 60000 + diff;    // e.g. current = 100, last = 59500

        double speed = 2 * M_PI * wheelRadius / (double(diff) / 1000.00);

        QVariant spd(speed);
        QMetaObject::invokeMethod(objekat, "changeSpeed", Q_ARG(QVariant, spd));

        lastMillis = currentMillis;
        passCount = 1;
        qDebug() << speed;
        qDebug() << diff;
    }
    else if(passCount > 2)  // just in case
    {
        passCount = 0;
    }

}


