#include "MPU6050_mylib.h"

int8_t MPU6050_init(){
	uint8_t Data_conf = 0, acc_2g = 0x00, gyro_2g=0x00;
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_RA_WHO_AM_I, 1, &Data_conf, 1, 100);
	if(Data_conf != MPU6050_WHO_AM_I_DEF_VAL){
		return Data_conf;
	}
	//ustawienie czujnika na skalach +-2g i +-250 deg/s2
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_RA_ADRR, MPU6050_RA_ACCEL_CONFIG, 1, &acc_2g, 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_RA_ADRR, MPU6050_RA_GYRO_CONFIG, 1, &gyro_2g, 1, 100);
	return 0;
}

//odczyt danych z pojedynczej osi akcelerometru
float MPU6050_acc_mes(char axis){
	uint8_t Data [2];
	int16_t Axis = 0;

	if(axis == 'x'){
		HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_ACCEL_XOUT_H_16, 1, &Data, 2, 100);
	}
	else if(axis == 'y'){
		HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_ACCEL_YOUT_H_16, 1, &Data, 2, 100);
	}
	else if(axis == 'z'){
		HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_ACCEL_ZOUT_H_16, 1, &Data, 2, 100);
	}
	else{
		return -1;
	}

	Axis = ((Data[0] << 8) | Data[1]);
	return ((float)Axis*2.0*GRAVITY)/(float)INT16_MAX;
}

//odczyt danych z pojedynczej osi zyroskopu
float MPU6050_gyro_mes(char axis){
	uint8_t Data [2];
	int16_t Axis = 0;

	if(axis == 'x'){
		HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_GYRO_XOUT_H_16, 1, &Data, 2, 100);
	}
	else if(axis == 'y'){
		HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_GYRO_YOUT_H_16, 1, &Data, 2, 100);
	}
	else if(axis == 'z'){
		HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_GYRO_ZOUT_H_16, 1, &Data, 2, 100);
	}
	else{
		return -1;
	}

	Axis = ((Data[0] << 8) | Data[1]);
	return ((float)Axis*250.0)/(float) INT16_MAX;
}

void MPU6050_measure(MPU6050_data* mes){
	uint8_t Data [14];//bufor danych
	int16_t Axis;//zmienna pomocnicza do konwersji odczytu na wartości fizyczne

	HAL_I2C_Mem_Read(&hi2c1, MPU6050_RA_ADRR, MPU6050_ACCEL_XOUT_H_16, 1, &Data, 14, 100);//odczyt danych z czujnika

	//konwersja danych z akcelerometru
	Axis = 0;
	Axis = ((Data[0] << 8) | Data[1]);//konwersja odczytu do liczby 16-to bitowej
	mes->acc_x = ((float)Axis*2.0*GRAVITY)/(float)INT16_MAX;//przeliczenie odczytu na wartości fizyczne

	Axis = 0;
	Axis = ((Data[2] << 8) | Data[3]);
	mes->acc_y = ((float)Axis*2.0*GRAVITY)/(float)INT16_MAX;

	Axis = 0;
	Axis = ((Data[4] << 8) | Data[5]);
	mes->acc_z = ((float)Axis*2.0*GRAVITY)/(float)INT16_MAX;

	//konwersja danych z żyroskopu
	Axis = 0;
	Axis = ((Data[8] << 8) | Data[9]);
	mes->gyro_x = ((float)Axis*250.0)/(float) INT16_MAX;

	Axis = 0;
	Axis = ((Data[10] << 8) | Data[11]);
	mes->gyro_y = ((float)Axis*250.0)/(float) INT16_MAX;

	Axis = 0;
	Axis = ((Data[12] << 8) | Data[13]);
	mes->gyro_z = ((float)Axis*250.0)/(float) INT16_MAX;
}


