/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MCP9808_ADDR (0x18 << 1)
#define MCP9808_REG_CONFIG 0x01
#define MCP9808_REG_UPPER  0x02
#define MCP9808_REG_LOWER  0x03
#define MCP9808_REG_CRIT   0x04
#define MCP9808_REG_TEMP   0x05

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */
// 32x32 Güneş (Sıcak > 25°C)
const uint8_t sun_32[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00,
	0x00, 0x07, 0xe0, 0x00, 0x40, 0x00, 0x00, 0x02, 0x60, 0x00, 0x00, 0x06, 0x30, 0x0f, 0xf0, 0x0c,
	0x18, 0x3f, 0xfc, 0x18, 0x0c, 0x78, 0x1e, 0x30, 0x06, 0x70, 0x0e, 0x60, 0x03, 0xe0, 0x07, 0xc0,
	0x1f, 0xc0, 0x03, 0xf8, 0x1f, 0xc0, 0x03, 0xf8, 0x1f, 0xc0, 0x03, 0xf8, 0x1f, 0xc0, 0x03, 0xf8,
	0x03, 0xe0, 0x07, 0xc0, 0x06, 0x70, 0x0e, 0x60, 0x0c, 0x78, 0x1e, 0x30, 0x18, 0x3f, 0xfc, 0x18,
	0x30, 0x0f, 0xf0, 0x0c, 0x60, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x02, 0x00, 0x07, 0xe0, 0x00,
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 32x32 Bulut (Ilık 10°C - 25°C)
const uint8_t cloud_32[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x3f, 0xfc, 0x00,
	0x00, 0x7f, 0xfe, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 0x07, 0xff, 0xff, 0xf0,
	0x1f, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x7f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 32x32 Kardan Adam (Soğuk < 10°C)
const uint8_t snowman_32[] = {
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x07, 0xe0, 0x00,
	0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x19, 0x98, 0x00, 0x00, 0x1f, 0xf8, 0x00,
	0x00, 0x0f, 0xf0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00,
	0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0,
	0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0x80,
	0x01, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t mcp_buf[2];      // Sensörden gelen 2 byte veri için
float temperature = 0.0; // İşlenmiş sıcaklık değeri
char display_buf[16];    // Ekranda yazdırılacak metin için
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void MCP9808_SetLimits(int8_t low, int8_t up, int8_t crit) {
    uint8_t buf[3];


    buf[0] = MCP9808_REG_LOWER;
    buf[1] = (uint8_t)(low >> 4); // Sıcaklık formatı gereği kaydırma
    buf[2] = (uint8_t)(low << 4);
    HAL_I2C_Master_Transmit(&hi2c1, MCP9808_ADDR, buf, 3, 100);


    buf[0] = MCP9808_REG_UPPER;
    buf[1] = (uint8_t)(up >> 4);
    buf[2] = (uint8_t)(up << 4);
    HAL_I2C_Master_Transmit(&hi2c1, MCP9808_ADDR, buf, 3, 100);


    buf[0] = MCP9808_REG_CRIT;
    buf[1] = (uint8_t)(crit >> 4);
    buf[2] = (uint8_t)(crit << 4);
    HAL_I2C_Master_Transmit(&hi2c1, MCP9808_ADDR, buf, 3, 100);
}

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
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();

  //Acilis mesaji
  ssd1306_SetCursor(2, 0); //
  ssd1306_WriteString("MCP9808 Hazir", Font_7x10, White);
  ssd1306_UpdateScreen();
  HAL_Delay(1000);
  //Limits
  MCP9808_SetLimits(15, 24, 27);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // --- 1. MCP9808'DEN VERİ OKUMA ---
	        uint8_t data[2];
	        float current_temp = 0;
	        uint8_t reg = MCP9808_REG_TEMP;
	        uint8_t raw_upper_bits;

	        HAL_I2C_Master_Transmit(&hi2c1, MCP9808_ADDR, &reg, 1, 100);
	        if (HAL_I2C_Master_Receive(&hi2c1, MCP9808_ADDR, data, 2, 100) == HAL_OK) {
	        	raw_upper_bits = data[0] & 0xE0;

	            data[0] = data[0] & 0x1F;
	            if ((data[0] & 0x10) == 0x10) {
	                data[0] = data[0] & 0x0F;
	                current_temp = 256.0 - (data[0] * 16.0 + data[1] / 16.0);
	            } else {
	                current_temp = (data[0] * 16.0 + data[1] / 16.0);
	            }
	        }

	        // --- 2. EKRANI TEMİZLE ---
	        ssd1306_Fill(Black);

	                if (raw_upper_bits & 0x80) { // Bit 15: TA >= TCRIT
	                    ssd1306_SetCursor(50, 2);
	                    ssd1306_WriteString("!!! KRITIK !!!", Font_7x10, White);
	                }
	                else if (raw_upper_bits & 0x40) { // Bit 14: TA > TUPPER
	                    ssd1306_SetCursor(50, 2);
	                    ssd1306_WriteString("YUKSEK ISI!", Font_7x10, White);
	                }
	                else if (raw_upper_bits & 0x20) { // Bit 13: TA < TLOWER
	                    ssd1306_SetCursor(50, 2);
	                    ssd1306_WriteString("DUSUK ISI!", Font_7x10, White);
	                }

	        char temp_text[16];

	        // --- 3. SICAKLIĞA GÖRE SİMGE VE DURUM SEÇİMİ ---
	        const uint8_t* selected_icon;
	        char* status_msg;

	        if (current_temp < 16.0) {
	            selected_icon = snowman_32;
	            status_msg = "SOGUK";
	        }
	        else if (current_temp >= 16.0 && current_temp <= 25.0) {
	            selected_icon = cloud_32;
	            status_msg = "ILIK";
	        }
	        else {
	            selected_icon = sun_32;
	            status_msg = "SICAK";
	        }

	        // --- 4. EKRAN TASARIMI ÇİZİMİ ---

	        // x: 4, y: 16 (32x32 ikon için 64 yüksekliğin ortası)
	        ssd1306_DrawBitmap(4, 16, selected_icon, 32, 32, White);

	        // 3. Durum Metni (Üst sol köşe)
	        ssd1306_SetCursor(4, 2);
	        ssd1306_WriteString(status_msg, Font_7x10, White);

	        // 4. Ayırıcı dikey çizgi (Tasarımı bölmek için)
	        ssd1306_Line(42, 0, 42, 63, White);



	        int int_part = (int)current_temp;
	        int dec_part = (int)((current_temp - int_part) * 10);
	        sprintf(temp_text, "%d.%d °C", int_part, abs(dec_part));

	        ssd1306_SetCursor(50, 25);
	        ssd1306_WriteString(temp_text, Font_11x18, White);

	        ssd1306_SetCursor(105, 25);
	        ssd1306_WriteString("C", Font_11x18, White);
	        ssd1306_SetCursor(100, 20);
	        ssd1306_WriteString("o", Font_7x10, White);

	        // --- 5. GÜNCELLE ---
	        ssd1306_UpdateScreen();


	        HAL_Delay(1000);
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
