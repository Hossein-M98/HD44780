/**
 **********************************************************************************
 * @file   HD44780_platform.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  This file contains the platform specific functions for HD44780 driver
 **********************************************************************************
 *
 * Copyright (c) 2023 Hossein.M (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */

/* Includes ---------------------------------------------------------------------*/
#include "HD44780_platform.h"
#if defined(HD44780_PLATFORM_STM32_HAL)
#include "main.h"
#elif defined(HD44780_PLATFORM_ESP32_IDF)
#include "sdkconfig.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#elif defined(HD44780_PLATFORM_AVR)
#include <avr/io.h>
#include <util/delay.h>
#endif


/* Private Macro ----------------------------------------------------------------*/
#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif

#ifndef cbi
#define cbi(reg,bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg,bit) reg |= (_BV(bit))
#endif

#ifndef CHECKBIT
#define CHECKBIT(reg,bit) ((reg & _BV(bit)) ? 1 : 0)
#endif


/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

#if defined(HD44780_PLATFORM_STM32_HAL)
static void
SetGPIO_OUT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
static void
SetGPIO_IN(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
#elif defined(HD44780_PLATFORM_ESP32_IDF)
static void
SetGPIO_OUT(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_OUTPUT);
}
static void
SetGPIO_IN(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_INPUT);
  gpio_set_pull_mode(GPIO_Pad, GPIO_FLOATING);
}
#endif

static void
PlatformInitRsRwE(void)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  SetGPIO_OUT(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_PIN);
  SetGPIO_OUT(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_PIN);
  SetGPIO_OUT(HD44780_E_GPIO_PORT, HD44780_E_GPIO_PIN);
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  SetGPIO_OUT(HD44780_RS_GPIO);
  SetGPIO_OUT(HD44780_RW_GPIO);
  SetGPIO_OUT(HD44780_E_GPIO);
#elif defined(HD44780_PLATFORM_AVR)
  sbi(HD44780_RS_GPIO_DDR, HD44780_RS_GPIO_BIT);
  sbi(HD44780_RW_GPIO_DDR, HD44780_RW_GPIO_BIT);
  sbi(HD44780_E_GPIO_DDR, HD44780_E_GPIO_BIT);
#endif
}

static void
PlatformSetValRs(uint8_t Level)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  HAL_GPIO_WritePin(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_PIN, Level);
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  gpio_set_level(HD44780_RS_GPIO, Level);
#elif defined(HD44780_PLATFORM_AVR)
  if (Level)
    sbi(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_BIT);
  else
    cbi(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_BIT);
#endif
}

static void
PlatformSetValRw(uint8_t Level)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  HAL_GPIO_WritePin(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_PIN, Level);
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  gpio_set_level(HD44780_RW_GPIO, Level);
#elif defined(HD44780_PLATFORM_AVR)
  if (Level)
    sbi(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_BIT);
  else
    cbi(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_BIT);
#endif
}

static void
PlatformSetValE(uint8_t Level)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  HAL_GPIO_WritePin(HD44780_E_GPIO_PORT, HD44780_E_GPIO_PIN, Level);
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  gpio_set_level(HD44780_E_GPIO, Level);
#elif defined(HD44780_PLATFORM_AVR)
  if (Level)
    sbi(HD44780_E_GPIO_PORT, HD44780_E_GPIO_BIT);
  else
    cbi(HD44780_E_GPIO_PORT, HD44780_E_GPIO_BIT);
#endif
}

static void
PlatformSetDirData(uint8_t Dir)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  if (Dir)
  {
    SetGPIO_OUT(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_PIN);
    SetGPIO_OUT(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_PIN);
    SetGPIO_OUT(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_PIN);
    SetGPIO_OUT(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_PIN);
  }
  else
  {
    SetGPIO_IN(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_PIN);
    SetGPIO_IN(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_PIN);
    SetGPIO_IN(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_PIN);
    SetGPIO_IN(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_PIN);
  }
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  if (Dir)
  {
    SetGPIO_OUT(HD44780_D4_GPIO);
    SetGPIO_OUT(HD44780_D5_GPIO);
    SetGPIO_OUT(HD44780_D6_GPIO);
    SetGPIO_OUT(HD44780_D7_GPIO);
  }
  else
  {
    SetGPIO_IN(HD44780_D4_GPIO);
    SetGPIO_IN(HD44780_D5_GPIO);
    SetGPIO_IN(HD44780_D6_GPIO);
    SetGPIO_IN(HD44780_D7_GPIO);
  }
#elif defined(HD44780_PLATFORM_AVR)
  if (Dir)
  {
    sbi(HD44780_D4_GPIO_DDR, HD44780_D4_GPIO_BIT);
    sbi(HD44780_D5_GPIO_DDR, HD44780_D5_GPIO_BIT);
    sbi(HD44780_D6_GPIO_DDR, HD44780_D6_GPIO_BIT);
    sbi(HD44780_D7_GPIO_DDR, HD44780_D7_GPIO_BIT);
  }
  else
  {
    cbi(HD44780_D4_GPIO_DDR, HD44780_D4_GPIO_BIT);
    cbi(HD44780_D5_GPIO_DDR, HD44780_D5_GPIO_BIT);
    cbi(HD44780_D6_GPIO_DDR, HD44780_D6_GPIO_BIT);
    cbi(HD44780_D7_GPIO_DDR, HD44780_D7_GPIO_BIT);
  }
#endif
}

static void
PlatformWriteData(uint8_t Data)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  HAL_GPIO_WritePin(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_PIN, (Data & 0x10));
  HAL_GPIO_WritePin(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_PIN, (Data & 0x20));
  HAL_GPIO_WritePin(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_PIN, (Data & 0x40));
  HAL_GPIO_WritePin(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_PIN, (Data & 0x80));
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  gpio_set_level(HD44780_D4_GPIO, (Data & 0x10));
  gpio_set_level(HD44780_D5_GPIO, (Data & 0x20));
  gpio_set_level(HD44780_D6_GPIO, (Data & 0x40));
  gpio_set_level(HD44780_D7_GPIO, (Data & 0x80));
#elif defined(HD44780_PLATFORM_AVR)
  if (Data & 0x10)
    sbi(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_BIT);
  else
    cbi(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_BIT);

  if (Data & 0x20)
    sbi(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_BIT);
  else
    cbi(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_BIT);

  if (Data & 0x40)
    sbi(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_BIT);
  else
    cbi(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_BIT);

  if (Data & 0x80)
    sbi(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_BIT);
  else
    cbi(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_BIT);
#endif
}

static uint8_t
PlatformReadData(void)
{
  uint8_t Data = 0;

#if defined(HD44780_PLATFORM_STM32_HAL)
  if (HAL_GPIO_ReadPin(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_PIN))
    Data |= 0x10;
  if (HAL_GPIO_ReadPin(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_PIN))
    Data |= 0x20;
  if (HAL_GPIO_ReadPin(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_PIN))
    Data |= 0x40;
  if (HAL_GPIO_ReadPin(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_PIN))
    Data |= 0x80;
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  if (gpio_get_level(HD44780_D4_GPIO))
    Data |= 0x10;
  if (gpio_get_level(HD44780_D5_GPIO))
    Data |= 0x20;
  if (gpio_get_level(HD44780_D6_GPIO))
    Data |= 0x40;
  if (gpio_get_level(HD44780_D7_GPIO))
    Data |= 0x80;
#elif defined(HD44780_PLATFORM_AVR)
  if (CHECKBIT(HD44780_D4_GPIO_PIN, HD44780_D4_GPIO_BIT))
    Data |= 0x10;
  if (CHECKBIT(HD44780_D5_GPIO_PIN, HD44780_D5_GPIO_BIT))
    Data |= 0x20;
  if (CHECKBIT(HD44780_D6_GPIO_PIN, HD44780_D6_GPIO_BIT))
    Data |= 0x40;
  if (CHECKBIT(HD44780_D7_GPIO_PIN, HD44780_D7_GPIO_BIT))
    Data |= 0x80;
#endif

  return Data;
}

static void
PlatformDelayMs(uint16_t Delay)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  HAL_Delay(Delay);
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  vTaskDelay(Delay / portTICK_PERIOD_MS);
#elif defined(HD44780_PLATFORM_AVR)
  for (; Delay > 0; Delay--)
    _delay_ms(1);
#endif
}

static void
PlatformDelayUs(uint16_t Delay)
{
#if defined(HD44780_PLATFORM_STM32_HAL)
  uint32_t clk_freq = HAL_RCC_GetSysClockFreq();
  uint32_t ticks = (Delay * clk_freq) / 1000000;
  for (uint32_t i = 0; i < ticks; i++)
  {
    __NOP();
  }
#elif defined(HD44780_PLATFORM_ESP32_IDF)
  ets_delay_us(Delay);
#elif defined(HD44780_PLATFORM_AVR)
  for (; Delay > 0; Delay--)
    _delay_us(1);
#endif
}




/**
 ==================================================================================
                         ##### Public Functions #####                              
 ==================================================================================
 */

void
HD44780_Platform_Init(HD44780_Handler_t *Handler)
{
  Handler->PlatformInitRsRwE = PlatformInitRsRwE;
  Handler->PlatformSetValRs = PlatformSetValRs;
  Handler->PlatformSetValRw = PlatformSetValRw;
  Handler->PlatformSetValE = PlatformSetValE;
  Handler->PlatformSetDirData = PlatformSetDirData;
  Handler->PlatformWriteData = PlatformWriteData;
  Handler->PlatformReadData = PlatformReadData;
  Handler->PlatformDelayMs = PlatformDelayMs;
  Handler->PlatformDelayUs = PlatformDelayUs;
}