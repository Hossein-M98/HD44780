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
#include <avr/io.h>
#include <util/delay.h>


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

static void
PlatformInitRsRwE(void)
{
  sbi(HD44780_RS_GPIO_DDR, HD44780_RS_GPIO_BIT);
  sbi(HD44780_RW_GPIO_DDR, HD44780_RW_GPIO_BIT);
  sbi(HD44780_E_GPIO_DDR, HD44780_E_GPIO_BIT);
}

static void
PlatformSetValRs(uint8_t Level)
{
  if (Level)
    sbi(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_BIT);
  else
    cbi(HD44780_RS_GPIO_PORT, HD44780_RS_GPIO_BIT);
}

static void
PlatformSetValRw(uint8_t Level)
{
  if (Level)
    sbi(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_BIT);
  else
    cbi(HD44780_RW_GPIO_PORT, HD44780_RW_GPIO_BIT);
}

static void
PlatformSetValE(uint8_t Level)
{
  if (Level)
    sbi(HD44780_E_GPIO_PORT, HD44780_E_GPIO_BIT);
  else
    cbi(HD44780_E_GPIO_PORT, HD44780_E_GPIO_BIT);
}

static void
PlatformSetDirData(uint8_t Dir)
{
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
}

static void
PlatformWriteData(uint8_t Data)
{
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
}

static uint8_t
PlatformReadData(void)
{
  uint8_t Data = 0;

  if (CHECKBIT(HD44780_D4_GPIO_PIN, HD44780_D4_GPIO_BIT))
    Data |= 0x10;
  if (CHECKBIT(HD44780_D5_GPIO_PIN, HD44780_D5_GPIO_BIT))
    Data |= 0x20;
  if (CHECKBIT(HD44780_D6_GPIO_PIN, HD44780_D6_GPIO_BIT))
    Data |= 0x40;
  if (CHECKBIT(HD44780_D7_GPIO_PIN, HD44780_D7_GPIO_BIT))
    Data |= 0x80;

  return Data;
}

static void
PlatformDelayMs(uint16_t Delay)
{
  for (; Delay > 0; Delay--)
    _delay_ms(1);
}

static void
PlatformDelayUs(uint16_t Delay)
{
  for (; Delay > 0; Delay--)
    _delay_us(1);
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
