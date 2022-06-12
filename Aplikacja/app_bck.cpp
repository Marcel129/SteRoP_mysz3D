#include "app_bck.hh"

mouse::mouse(){
    error.acc_x = ACC_X_ERROR;
    error.acc_y = ACC_Y_ERROR;
    error.acc_z = ACC_Z_ERROR;
    error.gyro_x = GYRO_X_ERROR;
    error.gyro_y = GYRO_Y_ERROR;
    error.gyro_z = GYRO_Z_ERROR;

    this->buffSize = BUFF_SIZE;
    this->buff = (char*)malloc((buffSize +1)*sizeof(char));
}


int mouse::connect_mouse()
{
    hSerial = CreateFile("COM4",
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        return COM_PORT_ERROR;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        return SERIAL_SETTINGS_ERROR;
    }
    dcbSerialParams.BaudRate = CBR_19200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        return SERIAL_SETTINGS_ERROR;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 5;
    timeouts.ReadTotalTimeoutConstant = 5;
    timeouts.ReadTotalTimeoutMultiplier = 5;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        return SERIAL_SETTINGS_ERROR;
    }

    //*hSerialOrg = hSerial;
    return SERIAL_OK_STATUS;
}

void mouse::readLine()
{
    DWORD dwBytesRead = 0;
    int a = 0;
    char smiec, tmpVal;

    for (int i = 0; i <= buffSize; i++)
    {
        *(buff + i) = 0;
    }

    do
    {
        ReadFile(hSerial, &tmpVal, 1, &dwBytesRead, NULL);
        if (tmpVal != '\n')
            buff[a] = tmpVal;
        a++;
    } while (tmpVal != '\n' && a - 1 <= buffSize);
    ReadFile(hSerial, &smiec, 1, &dwBytesRead, NULL);
}

MPU6050_data mouse::getMeasure(){
    MPU6050_data d;
    readLine();
    d.convertData(this->buff);

    return d;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

MPU6050_data MPU6050_data::operator-(const MPU6050_data &sd2)
{

    MPU6050_data tmp;

    tmp.acc_x = this->acc_x - sd2.acc_x;
    tmp.acc_y = this->acc_y - sd2.acc_y;
    tmp.acc_z = this->acc_z - sd2.acc_z;
    tmp.gyro_x = this->gyro_x - sd2.gyro_x;
    tmp.gyro_y = this->gyro_y - sd2.gyro_y;
    tmp.gyro_z = this->gyro_z - sd2.gyro_z;

    return tmp;
}

MPU6050_data MPU6050_data::operator+(const MPU6050_data &sd2)
{

    MPU6050_data tmp;

    tmp.acc_x = this->acc_x + sd2.acc_x;
    tmp.acc_y = this->acc_y + sd2.acc_y;
    tmp.acc_z = this->acc_z + sd2.acc_z;
    tmp.gyro_x = this->gyro_x + sd2.gyro_x;
    tmp.gyro_y = this->gyro_y + sd2.gyro_y;
    tmp.gyro_z = this->gyro_z + sd2.gyro_z;

    return tmp;
}

MPU6050_data MPU6050_data::operator*(const float &sd2)
{

    MPU6050_data tmp;

    tmp.acc_x = this->acc_x * sd2;
    tmp.acc_y = this->acc_y * sd2;
    tmp.acc_z = this->acc_z * sd2;
    tmp.gyro_x = this->gyro_x * sd2;
    tmp.gyro_y = this->gyro_y * sd2;
    tmp.gyro_z = this->gyro_z * sd2;

    return tmp;
}

MPU6050_data MPU6050_data::operator/(const float &sd2)
{

    MPU6050_data tmp;

    if (sd2 != 0)
    {
        tmp.acc_x = this->acc_x / sd2;
        tmp.acc_y = this->acc_y / sd2;
        tmp.acc_z = this->acc_z / sd2;
        tmp.gyro_x = this->gyro_x / sd2;
        tmp.gyro_y = this->gyro_y / sd2;
        tmp.gyro_z = this->gyro_z / sd2;
    }

    return tmp;
}

MPU6050_data &MPU6050_data::operator+=(const MPU6050_data &sd2)
{

    this->acc_x += sd2.acc_x;
    this->acc_y += sd2.acc_y;
    this->acc_z += sd2.acc_z;
    this->gyro_x += sd2.gyro_x;
    this->gyro_y += sd2.gyro_y;
    this->gyro_z += sd2.gyro_z;

    return *this;
}

MPU6050_data &MPU6050_data::operator=(const float a)
{

    this->acc_x = a;
    this->acc_y = a;
    this->acc_z = a;
    this->gyro_x = a;
    this->gyro_y = a;
    this->gyro_z = a;

    return *this;
}

void MPU6050_data::cutError(float err)
{
    if (abs(this->acc_x) < err)
        this->acc_x = 0;
    if (abs(this->acc_y) < err)
        this->acc_y = 0;
    if (abs(this->acc_z) < err)
        this->acc_z = 0;
    if (abs(this->gyro_x) < err)
        this->gyro_x = 0;
    if (abs(this->gyro_y) < err)
        this->gyro_y = 0;
    if (abs(this->gyro_z) < err)
        this->gyro_z = 0;
}

void MPU6050_data::convertData(char *rawData)
{
    const char s[2] = " ";
    char *token = (char *)malloc(15 * sizeof(char));

    token = strtok(rawData, s);
    this->acc_x = atof(token);
    token = strtok(NULL, s);
    this->acc_y = atof(token);
    token = strtok(NULL, s);
    this->acc_z = atof(token);
    token = strtok(NULL, s);
    this->gyro_x = atof(token);
    token = strtok(NULL, s);
    this->gyro_y = atof(token);
    token = strtok(NULL, s);
    this->gyro_z = atof(token);
}