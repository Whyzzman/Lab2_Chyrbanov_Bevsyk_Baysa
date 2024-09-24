/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private variables ---------------------------------------------------------*/
uint32_t currentClick = 0;
uint32_t prevClick = 0;
#define debounceTime 200 // Час для усунення брязкоту (в мілісекундах)
uint8_t ledState = 0; // Стан світлодіодів (0 - вимкнені, 1 - всі увімкнені)
uint8_t currentLED = 0; // Поточний активний світлодіод
volatile uint8_t buttonPressed = 0; // Флаг для натискання кнопки

#define NUM_OF_LEDS 4 // Кількість світлодіодів
GPIO_TypeDef* GPIOx = GPIOD; // Порт для світлодіодів
uint16_t LED_PINS[NUM_OF_LEDS] = { GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 }; // Піни для кожного світлодіода

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init(); // Ініціалізація HAL

  MX_GPIO_Init(); // Ініціалізація GPIO

  // Вимкнути всі світлодіоди на початку
  HAL_GPIO_WritePin(GPIOx, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);

  /* Infinite loop */
  while (1)
  {
    // Перевірка натискання кнопки
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
    {
      HAL_Delay(debounceTime); // Затримка для уникнення дебаунсу кнопки

      // Логіка почергового вмикання діодів
      if (ledState < NUM_OF_LEDS)
      {
        HAL_GPIO_WritePin(GPIOx, LED_PINS[ledState], GPIO_PIN_SET); // Увімкнути поточний діод
        ledState++;
      }
      else
      {
        // Вимкнути всі діоди і повернути стан до початкового
        HAL_GPIO_WritePin(GPIOx, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
        ledState = 0; // Скинути стан
      }

      // Чекати, поки кнопка буде відпущена
      while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET);
    }
  }
}

/* USER CODE BEGIN 4 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING; // Налаштування переривання
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/* SystemClock_Config and assert_failed functions remain unchanged */
