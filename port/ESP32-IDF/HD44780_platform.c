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
#include "sdkconfig.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

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


static void
PlatformInitRsRwE(void)
{
  SetGPIO_OUT(HD44780_RS_GPIO);
  SetGPIO_OUT(HD44780_RW_GPIO);
  SetGPIO_OUT(HD44780_E_GPIO);
}

static void
PlatformSetValRs(uint8_t Level)
{
  gpio_set_level(HD44780_RS_GPIO, Level);
}

static void
PlatformSetValRw(uint8_t Level)
{
  gpio_set_level(HD44780_RW_GPIO, Level);
}

static void
PlatformSetValE(uint8_t Level)
{
  gpio_set_level(HD44780_E_GPIO, Level);
}

static void
PlatformSetDirData(uint8_t Dir)
{
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
}

static void
PlatformWriteData(uint8_t Data)
{
  gpio_set_level(HD44780_D4_GPIO, (Data & 0x10));
  gpio_set_level(HD44780_D5_GPIO, (Data & 0x20));
  gpio_set_level(HD44780_D6_GPIO, (Data & 0x40));
  gpio_set_level(HD44780_D7_GPIO, (Data & 0x80));
}

static uint8_t
PlatformReadData(void)
{
  uint8_t Data = 0;

  if (gpio_get_level(HD44780_D4_GPIO))
    Data |= 0x10;
  if (gpio_get_level(HD44780_D5_GPIO))
    Data |= 0x20;
  if (gpio_get_level(HD44780_D6_GPIO))
    Data |= 0x40;
  if (gpio_get_level(HD44780_D7_GPIO))
    Data |= 0x80;

  return Data;
}

static void
PlatformDelayMs(uint16_t Delay)
{
  vTaskDelay(Delay / portTICK_PERIOD_MS);
}

static void
PlatformDelayUs(uint16_t Delay)
{
  ets_delay_us(Delay);
}



/**
 ==================================================================================
                         ##### Public Functions #####                              
 ==================================================================================
 */

/**
 * @brief  Initialize platform device to communicate HD44780.
 * @param  Handler: Pointer to handler
 * @retval None
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
