#include "accmeasure.h"

AccMeasure::AccMeasure() : QObject ()
{
    fd = wiringPiI2CSetup(DEVICE_ID);
    if(fd < 0)
        qDebug() << "ERROR: wiringPi i2c not initialized!";

    wiringPiI2CWriteReg8(fd, POWER_CTL, 0x8);   // set Measure bit to start measuring

};

AccMeasure::~AccMeasure() {}

int AccMeasure::getAcc(bool degOrGrad)
{
    int AccX = wiringPiI2CReadReg16(fd, X0_REG);    // reading 16bits from address of 8bit register -> X1 REG is also being read i.e. upper 8 bits of data
    //int AccY = wiringPiI2CReadReg16(fd, Y0_REG);
    int AccZ = wiringPiI2CReadReg16(fd, Z0_REG);
    AccX = -(~int16_t(AccX) + 1);                   // data is in 2's complement, convert it
    //AccY = -(~int16_t(AccY) + 1);
    AccZ = -(~int16_t(AccZ) + 1);

   // double pitch = atan2((double(AccX) * 0.0312), sqrt(pow(double(AccY)*0.0312, 2) + pow(double(AccZ)*0.0312, 2))) * 180 / M_PI;    // 3D, in degrees
    double pitch = atan2((double(AccX) * 0.0043), (double(AccZ) * 0.0037)) * 180 / M_PI;    // 2D, degrees, 2g->0.0039, 16g->0.0312
    double gradient = tan(pitch * M_PI / 180) * 100;    // in precentage

  //  qDebug() << pitch << "   " << gradient;

    if(!degOrGrad)
        return int(pitch);
    else {
        return int(gradient);
    }

}
