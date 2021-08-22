#ifndef ACCMEASURE_H
#define ACCMEASURE_H

#include <QtCore/QObject>
#include <wiringPiI2C.h>
#include <QDebug>
#include <math.h>

#define DEVICE_ID 0x53   // ADXL345 I2C device id
#define POWER_CTL 0x2D   // register used for standby, sleep etc. functions
#define DATA_FORMAT 0x31 // data format register
#define X0_REG 0x32      // first byte of X-axis data
#define Y0_REG 0x34      // first byte of Y-axis data
#define Z0_REG 0x36      // first byte of Z-axis data

class AccMeasure : public QObject
{
    Q_OBJECT

public:

    AccMeasure();
    ~AccMeasure();

    Q_INVOKABLE int getAcc(bool);

private:

    int fd;

};



#endif // ACCMEASURE_H
