/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Select the exercise: 4 = HH:MM clock, 5 = right-shifting digits.
#define LAB2_EXERCISE 4U
#define EX5_DIGIT_COUNT 4U
#define MAIN_TASK_PERIOD_MS 50U
#define EX5_SHIFT_TICKS (1000U / MAIN_TASK_PERIOD_MS)
#define EX4_SECOND_TICKS (1000U / MAIN_TASK_PERIOD_MS)
// A 2Hz full blink cycle is 500ms: toggle the colon every 250ms.
#define EX4_COLON_TICKS (250U / MAIN_TASK_PERIOD_MS)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static const uint8_t ex5_digits[EX5_DIGIT_COUNT] = {1, 5, 4, 7};
static uint8_t ex5_offset = 0;
static uint8_t ex5_ticks = 0;

static uint8_t ex4_hour = 0;
static uint8_t ex4_minute = 0;
static uint8_t ex4_second = 0;
static uint8_t ex4_second_ticks = 0;
static uint8_t ex4_colon_ticks = 0;
static uint8_t ex4_colon = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init();
void test_LedDebug();
void test_LedY0();
void test_LedY1();
static void ex5_show_digits(void);
static void ex5_update(void);
static void ex4_init(void);
static void ex4_show_time(void);
static void ex4_update(void);
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
  MX_TIM2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  system_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  while(!flag_timer2);
	  flag_timer2 = 0;
	  // Run only the selected exercise; both use the same 50ms timer.
	  if(LAB2_EXERCISE == 4U){
		  ex4_update();
	  } else {
		  ex5_update();
	  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void system_init(){
	  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
	  led7_init();
	  if(LAB2_EXERCISE == 4U){
		  ex4_init();
	  } else {
		  led7_SetColon(0);
		  ex5_show_digits();
	  }
	  setTimer2(MAIN_TASK_PERIOD_MS);
	  // Start scanning only after the display and software timer are ready.
	  timer_init();
}

uint8_t count_led_debug = 0;
uint8_t count_led_Y0 = 0;
uint8_t count_led_Y1 = 0;

void test_LedDebug(){
	count_led_debug = (count_led_debug + 1)%20;
	if(count_led_debug == 0){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
	}
}

void test_LedY0(){
	count_led_Y0 = (count_led_Y0+ 1)%100;
	if(count_led_Y0 > 40){
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1);
	} else {
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	}
}

void test_LedY1(){
	count_led_Y1 = (count_led_Y1+ 1)%40;
	if(count_led_Y1 > 10){
		HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	} else {
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y1_Pin, 1);
	}
}

static void ex5_show_digits(void){
	uint32_t primask = __get_PRIMASK();
	// Keep the scan ISR from reading a partially updated display buffer.
	__disable_irq();
	for(uint8_t position = 0; position < EX5_DIGIT_COUNT; position++){
		uint8_t source = (position + EX5_DIGIT_COUNT - ex5_offset) % EX5_DIGIT_COUNT;
		led7_SetDigit(ex5_digits[source], position, 0);
	}
	__set_PRIMASK(primask);
}

static void ex5_update(void){
	ex5_ticks++;
	if(ex5_ticks == EX5_SHIFT_TICKS){
		ex5_ticks = 0;
		ex5_offset = (ex5_offset + 1U) % EX5_DIGIT_COUNT;
		ex5_show_digits();
	}
}

static void ex4_init(void){
	// Start at 00:00:00 after reset; the clock uses a 24-hour format.
	ex4_hour = 0;
	ex4_minute = 0;
	ex4_second = 0;
	ex4_second_ticks = 0;
	ex4_colon_ticks = 0;
	ex4_colon = 1;
	ex4_show_time();
}

static void ex4_show_time(void){
	uint32_t primask = __get_PRIMASK();
	// The scan ISR shares both the digit buffer and the colon SPI bit.
	__disable_irq();
	led7_SetDigit(ex4_hour / 10U, 0, 0);
	led7_SetDigit(ex4_hour % 10U, 1, 0);
	led7_SetDigit(ex4_minute / 10U, 2, 0);
	led7_SetDigit(ex4_minute % 10U, 3, 0);
	led7_SetColon(ex4_colon);
	__set_PRIMASK(primask);
}

static void ex4_update(void){
	ex4_second_ticks++;
	if(ex4_second_ticks == EX4_SECOND_TICKS){
		ex4_second_ticks = 0;
		ex4_second++;
		if(ex4_second == 60U){
			ex4_second = 0;
			ex4_minute++;
			if(ex4_minute == 60U){
				ex4_minute = 0;
				ex4_hour = (ex4_hour + 1U) % 24U;
			}
		}
	}

	ex4_colon_ticks++;
	if(ex4_colon_ticks == EX4_COLON_TICKS){
		ex4_colon_ticks = 0;
		ex4_colon ^= 1U;
		// Minute changes coincide with this refresh (20 ticks = 4 x 5).
		ex4_show_time();
	}
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
#ifdef USE_FULL_ASSERT
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
