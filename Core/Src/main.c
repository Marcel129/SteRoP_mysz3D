/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "MPU6050_mylib.h"
//#include "usbd_cdc_if.h" // Plik bedacy interfejsem uzytkownika do kontrolera USB
#define UART_BUFF_SIZE 20
#define MIN_FREQ_VAL 1
#define MAX_FREQ_VAL 100

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//funkcja nadpisująca _write, umozliwia korzystanie z fukcji printf do komunikacji przez UART z komputerem
//int _write(int file, char *ptr, int len)
//{
//	  HAL_UART_Transmit(&huart2 , ptr , len , 50 ) ;
//
//	return len;
//}
//
//
//volatile uint8_t uart_rx_buffer, finishDataReading = 0;
//uint8_t buffer[UART_BUFF_SIZE+1];
//int lineLength = 0;
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  if (huart == &huart2) {
//	//if(buffer[lineLength]=='X' && buffer[lineLength+1]=='X') finishDataReading = 1;
//	if(uart_rx_buffer == 'w'){
//		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//	}
//	else if(uart_rx_buffer == 'e'){
//		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
//	}
//	HAL_UART_Receive_IT(&huart2, &uart_rx_buffer, 1);
//	buffer[lineLength] = uart_rx_buffer;
//
//	if(buffer[lineLength]=='\n'){
//		finishDataReading=1;
//	}
//	lineLength++;
//  }
//}

uint8_t ReceivedData[40], message[40] = "Send measure\n"; // Tablica przechowujaca odebrane dane
uint8_t ReceivedDataFlag = 0; // Flaga informujaca o odebraniu danych

uint64_t RxpipeAddrs = 0x11223344AA;
char myRxData[50];
char myAckPayload[32] = "Ack by STMF7!";

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  uint8_t DataToSend[40] = {0}; // Tablica zawierajaca dane do wyslania
  uint8_t MessageLength = 0; // Zawiera dlugosc wysylanej wiadomosci
  const uint8_t mes_count = 10;

  //struktura przechowująca dane odczytane z mierników
  MPU6050_data sensor_data, tmp_data;
  sensor_data.acc_x = 0;
  sensor_data.acc_y = 0;
  sensor_data.acc_z = 0;
  sensor_data.gyro_x = 0;
  sensor_data.gyro_y = 0;
  sensor_data.gyro_z = 0;

  //częstotliwość pomiarów w Hz, domyślnie 5; wartość można edytować z poziomu komputera
  uint32_t measurementsFreq = 2, prevTime = HAL_GetTick(), recVal=0;


  //inicjalizacja czujnika MPU6050
  MPU6050_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

//	if(ReceivedDataFlag == 1){
//
//		ReceivedDataFlag = 0;
//
//		if(*ReceivedData == *message) {
	  for(int i=0;i<mes_count;i++){
		  MPU6050_measure(&tmp_data);
		  sensor_data.acc_x += tmp_data.acc_x;
		  sensor_data.acc_y += tmp_data.acc_y;
		  sensor_data.acc_z += tmp_data.acc_z;
		  sensor_data.gyro_x += tmp_data.gyro_x;
		  sensor_data.gyro_y += tmp_data.gyro_y;
		  sensor_data.gyro_z += tmp_data.gyro_z;
		  HAL_Delay(5);
	  }
	  	  	  sensor_data.acc_x /= (float)mes_count;
	  		  sensor_data.acc_y /= (float)mes_count;
	  		  sensor_data.acc_z /= (float)mes_count;
	  		  sensor_data.gyro_x /= (float)mes_count;
	  		  sensor_data.gyro_y /= (float)mes_count;
	  		  sensor_data.gyro_z /= (float)mes_count;

			MessageLength = sprintf(DataToSend, "%0.2f %0.2f %0.2f %0.2f %0.2f %0.2f \n\r", sensor_data.acc_x,sensor_data.acc_y,sensor_data.acc_z,sensor_data.gyro_x,sensor_data.gyro_y,sensor_data.gyro_z);

			CDC_Transmit_FS(DataToSend, MessageLength);

			  sensor_data.acc_x = 0;
			  sensor_data.acc_y = 0;
			  sensor_data.acc_z = 0;
			  sensor_data.gyro_x = 0;
			  sensor_data.gyro_y = 0;
			  sensor_data.gyro_z = 0;
//		}
//
//		// Wyczyszczenie tablicy odebranych danych
//		uint8_t iter;
//		for(iter = 0; iter<40; ++iter){
//			ReceivedData[iter] = 0;
//		}
//	}


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

