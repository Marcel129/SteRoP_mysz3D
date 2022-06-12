#include "app_bck.hh"

int main()
{
    mouse m;
    MPU6050_data data, data_sum;
    data_sum=0;

    if (m.connect_mouse() != 0)
    {
        std::cerr << "Blad polaczenia z urzadzeniem" << std::endl;
        while (1)
        {
        };
    }
    else
    {
        std::cerr << "Nawiazano polaczenie z urzadzeniem" << std::endl;
    }

    while (1)
    {
        data = m.getMeasure() - m.getError();
        std::cout << "Data: " << data.acc_x << " " << data.acc_y << " " 
            << data.acc_z << " " << data.gyro_x << " " << data.gyro_y << " " << data.gyro_z << std::endl;
        Sleep(MEASUREMENT_TIME);
    };
}
