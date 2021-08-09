#ifndef HALLMEASURE_H
#define HALLMEASURE_H

#include <QtCore/QObject>
#include <wiringPi.h>
#include <QDebug>
#include <QDateTime>
#include <math.h>

class HallMeasure : public QObject
{
    Q_OBJECT

public:

    HallMeasure();
    ~HallMeasure();

    static void hallInterrupt();
    void getObject(QObject *obj);
    //void measure();

private:
    QObject *objekat;
    static char wheelCount;
    static int lastMillis;
    static double wheelRadius;

};


#endif // HALLMEASURE_H
