#ifndef APP_BCK_HH
#define APP_BCK_HH

#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>

#define TIMEOUT_ERROR -1
#define SERIAL_SETTINGS_ERROR -2
#define CONNECTION_ERROR -3
#define COM_PORT_ERROR -4
#define SERIAL_OK_STATUS 0

#define ACC_X_ERROR 0.22
#define ACC_Y_ERROR 0.394
#define ACC_Z_ERROR 10.08
#define GYRO_X_ERROR -1.5
#define GYRO_Y_ERROR 1.14
#define GYRO_Z_ERROR -0.72

#define BUFF_SIZE 35
#define MEASUREMENT_TIME 100//time between 2 measurements in ms

struct MPU6050_data
{
    double acc_x, acc_y, acc_z;
    double gyro_x, gyro_y, gyro_z;

    MPU6050_data operator-(const MPU6050_data &sd2);
    MPU6050_data operator+(const MPU6050_data &sd2);
    MPU6050_data operator*(const float &sd2);
    MPU6050_data operator/(const float &sd2);
    MPU6050_data &operator+=(const MPU6050_data &sd2);
    MPU6050_data &operator=(const float a);
    void cutError(float err);
    void convertData(char *rawData);
};



class mouse{
    HANDLE hSerial;
    
    MPU6050_data error;
    unsigned int freq;

    char * buff;
    unsigned int buffSize;



public:
    mouse();
    int connect_mouse();
    void readLine();
    MPU6050_data getMeasure();

    void setError(MPU6050_data e){error=e;};
    MPU6050_data getError()const{return error;};

    void setFreq(unsigned int a){freq=a;};
    unsigned int getFreq()const{return freq;};

};





#endif