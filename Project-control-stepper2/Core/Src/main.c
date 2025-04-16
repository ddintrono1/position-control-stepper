/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include "tof_sensor.h"
#include "stepper.h"
#include "pid_controller.h"
#include "command.h"
#include "command_instructions.h"

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
Stepper nema_17;
Command g_command;
PID_Controller pid;

// UART communication data
uint8_t rx_data[20];
uint8_t cnt=0;

// PID data
float Kp = 0.1f;
float Ki = 0.0f;
float Kd = 0.0f;
float out_min = -400.0f;
float out_max = 400.0f;
float out_threshold = 0.0f;

// Control data
int16_t distance;
int16_t speed;

// Stepper motor parameters
float step_angle = 0.05f;
GPIO_TypeDef *step_port = GPIOC;
uint16_t step_pin = GPIO_PIN_10;
TIM_HandleTypeDef *step_timer = &htim2;

GPIO_TypeDef *dir_port = GPIOA;
uint16_t dir_pin = GPIO_PIN_0;

GPIO_TypeDef *ms1_port = GPIOC;
uint16_t ms1_pin = GPIO_PIN_12;
GPIO_TypeDef *ms2_port = GPIOA;
uint16_t ms2_pin = GPIO_PIN_12;
GPIO_TypeDef *ms3_port = GPIOA;
uint16_t ms3_pin = GPIO_PIN_11;

GPIO_TypeDef *enable_port = GPIOB;
uint16_t enable_pin = GPIO_PIN_12;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  // Sensor initialization with 100 mm calibration distance
  TOF_Init(200);

  // Stepper initialization, microstepping initialization
  Stepper_Init(&nema_17, step_angle,
               step_port, step_pin,
               step_timer,
               dir_port, dir_pin,
               ms1_port, ms1_pin,
               ms2_port, ms2_pin,
               ms3_port, ms3_pin,
               enable_port, enable_pin);
  Stepper_SetMicroStep(&nema_17, SIXTEENTH_STEP);

  // Controller initialization
  PID_Init(&pid, Kp, Ki, Kd, out_min, out_max, out_threshold);

  // Command initialization
  Command_Init(&g_command, &pid, &huart2);

  // Send instructions through UART
  HAL_UART_Transmit_IT(&huart2, command_help_message, sizeof(command_help_message));

  // Starting uart communication
  HAL_UART_Receive_IT(&huart2, rx_data, 1);

  // Starting control timer
  HAL_TIM_Base_Start_IT(&htim7);

  // Activating motor
  Stepper_Enable(&nema_17);
  Stepper_Start(&nema_17);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if (htim->Instance == TIM7){
		/*
		 * Timer 7 is the control timer. It actuates control action every sample time
		 */
		distance = TOF_GetFilteredDistance(0, 1500);

		// Check if the error code distance has been returned
		if (distance == -1){
			return;
		}

		speed = PID_Compute(&pid, distance, 0.01f);

		// Change rotation direction depending on speed sign
		if (speed <= 0){
			Stepper_SetDirection(&nema_17, CLOCKWISE);
		}
		else{
			Stepper_SetDirection(&nema_17, COUNTER_CLOCKWISE);
		}

		Stepper_SetSpeed(&nema_17, fabs(speed));

		}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if (huart->Instance == USART2){

		// Check if the command has been lauched
		if (*(rx_data+cnt) == '\r'){

			Command_Parse(&g_command, rx_data);
			Command_Execute(&g_command);
			cnt = -1;
			memset(rx_data,0,sizeof(rx_data));
			Command_Clear(&g_command);
		}
		cnt++;
		HAL_UART_Receive_IT(&huart2, rx_data+cnt, 1);
	}

}

void _putchar(char c) {
    ITM_SendChar(c);  // Abilita printf su ITM
}

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
