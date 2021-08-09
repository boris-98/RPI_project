#include "hallmeasure.h"

char HallMeasure::wheelCount = 0;
int HallMeasure::lastMillis = 0;
double HallMeasure::wheelRadius = 0.30;

HallMeasure::HallMeasure() : QObject ()
{
    if(wiringPiSetup() < 0)
          qDebug() << "ERROR: WiringPi is not initialized!";

    wiringPiISR(0, INT_EDGE_RISING, &hallInterrupt);

};

HallMeasure::~HallMeasure(){}

void HallMeasure::hallInterrupt()
{
    QString millis = QDateTime::currentDateTime().toString("ss,zzz");
    int currentMillis = (millis.left(2)).toInt() * 1000 + (millis.right(3)).toInt();

    wheelCount++;

    if(wheelCount >= 2)
    {
        double rpm = 60000.0 / (currentMillis - lastMillis) * wheelCount;
        double speed = 2 * M_PI * wheelRadius * rpm/60;

        lastMillis = currentMillis;
        wheelCount = 1;
        qDebug() << speed;
    }

    qDebug() << "Interrupt in class has happened";

}

void HallMeasure::getObject(QObject *obj)
{
    objekat = obj;

}
