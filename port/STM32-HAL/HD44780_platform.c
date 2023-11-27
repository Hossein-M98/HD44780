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
#include "main.h"



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

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


static void
PlatformInitRsRwE(void)
{
  SetGPIO_OUT(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_PIN);
  SetGPIO_OUT(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_PIN);
  SetGPIO_OUT(HD44780_E_GPIO_PORT, HD44780_E_GPIO_PIN);
}

static void
PlatformSetValRs(uint8_t Level)
{
  HAL_GPIO_WritePin(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_PIN, Level);
}

static void
PlatformSetValRw(uint8_t Level)
{
  HAL_GPIO_WritePin(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_PIN, Level);
}

static void
PlatformSetValE(uint8_t Level)
{
  HAL_GPIO_WritePin(HD44780_E_GPIO_PORT, HD44780_E_GPIO_PIN, Level);
}

static void
PlatformSetDirData(uint8_t Dir)
{
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
}

static void
PlatformWriteData(uint8_t Data)
{
  HAL_GPIO_WritePin(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_PIN, (Data & 0x10));
  HAL_GPIO_WritePin(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_PIN, (Data & 0x20));
  HAL_GPIO_WritePin(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_PIN, (Data & 0x40));
  HAL_GPIO_WritePin(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_PIN, (Data & 0x80));
}

static uint8_t
PlatformReadData(void)
{
  uint8_t Data = 0;

  if (HAL_GPIO_ReadPin(HD44780_D4_GPIO_PORT, HD44780_D4_GPIO_PIN))
    Data |= 0x10;
  if (HAL_GPIO_ReadPin(HD44780_D5_GPIO_PORT, HD44780_D5_GPIO_PIN))
    Data |= 0x20;
  if (HAL_GPIO_ReadPin(HD44780_D6_GPIO_PORT, HD44780_D6_GPIO_PIN))
    Data |= 0x40;
  if (HAL_GPIO_ReadPin(HD44780_D7_GPIO_PORT, HD44780_D7_GPIO_PIN))
    Data |= 0x80;

  return Data;
}

static void
PlatformDelayMs(uint16_t Delay)
{
  HAL_Delay(Delay);
}

static void
PlatformDelayUs(uint16_t Delay)
{
  uint32_t clk_freq = HAL_RCC_GetSysClockFreq();
  uint32_t ticks = (Delay * clk_freq) / 1000000;
  for (uint32_t i = 0; i < ticks; i++)
  {
    __NOP();
  }
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
