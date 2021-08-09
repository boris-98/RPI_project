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
    static QObject *objekat;
    static char passCount;
    static int lastMillis;
    static int currentMillis;
    static double wheelRadius;

};


#endif // HALLMEASURE_H
