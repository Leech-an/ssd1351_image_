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
#include "rtc.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1351.h"
#include <stdio.h>
#include "unio_logo_128_rgb565.h"
#include "fonts.h"
#include <string.h>

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
#define CLOCK_Font_16x26
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void RTC_SetFromBuildTime(void);

static void ShowClockLoop(void);
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
  MX_SPI1_Init();
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_BACKUPRESET_FORCE();
  __HAL_RCC_BACKUPRESET_RELEASE();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  RTC_SetFromBuildTime();
	SSD1351_Unselect();
	HAL_Delay(10);
	SSD1351_Init();
	SSD1351_DrawImage(0, 0, 128, 128,(const uint16_t*) unio_logo_fixed_128_map);
	HAL_Delay(3000);


	ShowClockLoop();



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void RTC_SetFromBuildTime(void)
{
    // 이미 설정했으면 패스
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == 0xBEEF) return;

    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    // __DATE__ 예: "Aug 14 2025"
    // __TIME__ 예: "16:52:31"
    char monStr[4] = {0};
    int day=0, year=0, hh=0, mm=0, ss=0;

    if (sscanf(__DATE__, "%3s %d %d", monStr, &day, &year) != 3) return;
    if (sscanf(__TIME__, "%d:%d:%d", &hh, &mm, &ss) != 3) return;

    // 월 문자열 → 숫자
    const char *mons = "JanFebMarAprMayJunJulAugSepOctNovDec";
    const char *p = strstr(mons, monStr);
    if (!p) return;
    int month = (int)((p - mons) / 3) + 1;

    // 시간 설정
    sTime.Hours   = (uint8_t)hh;
    sTime.Minutes = (uint8_t)mm;
    sTime.Seconds = (uint8_t)ss;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    // 날짜 설정 (YY 기준)
    sDate.Year  = (uint8_t)(year - 2000);
    sDate.Month = (uint8_t)month;
    sDate.Date  = (uint8_t)day;
    sDate.WeekDay = RTC_WEEKDAY_MONDAY; // 굳이 정확치 않아도 됨
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    // “설정 완료” 마킹
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0xBEEF);
}


static void ShowClockLoop(void)
{
    char buf[16];
    RTC_TimeTypeDef t;
    RTC_DateTypeDef d;

    // "HH:MM:SS" = 8글자 → 중앙 정렬 좌표
    uint16_t w = Font_16x26.width * 8;
    uint16_t h = Font_16x26.height;
    uint16_t x = (128 - w) / 2;
    uint16_t y = (128 - h) / 2;

    while (1) {
        HAL_RTC_GetTime(&hrtc, &t, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &d, RTC_FORMAT_BIN); // 반드시 Time 다음에 호출

        snprintf(buf, sizeof(buf), "%02u:%02u:%02u",
                 t.Hours, t.Minutes, t.Seconds);

        // 이전 숫자 영역만 지우고 → 새로 쓰기(잔상 방지)
        SSD1351_FillRectangle(x, y, w, h, SSD1351_BLACK);
        SSD1351_WriteString(x, y, buf, Font_16x26,
                            SSD1351_WHITE, SSD1351_BLACK);

        HAL_Delay(200); // 5Hz 갱신(원하면 1000으로)
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
	while (1) {
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
