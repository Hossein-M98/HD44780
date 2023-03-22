/**
 **********************************************************************************
 * @file   HD44780_platform.h
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

/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef	_HD44780_PLATFORM_H_
#define _HD44780_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include "HD44780.h"


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Specify the target platform
 * @note   Uncomment the line below according to the target platform
 */ 
#define HD44780_PLATFORM_STM32_HAL
// #define HD44780_PLATFORM_ESP32_IDF
// #define HD44780_PLATFORM_AVR


#if defined(HD44780_PLATFORM_STM32_HAL)
// #define HD44780_RS_GPIO_PORT        GPIOA
// #define HD44780_RS_GPIO_PIN         GPIO_PIN_0
// #define HD44780_RW_GPIO_PORT        GPIOA
// #define HD44780_RW_GPIO_PIN         GPIO_PIN_1
// #define HD44780_E_GPIO_PORT         GPIOA
// #define HD44780_E_GPIO_PIN          GPIO_PIN_2
// #define HD44780_D4_GPIO_PORT        GPIOA
// #define HD44780_D4_GPIO_PIN         GPIO_PIN_3
// #define HD44780_D5_GPIO_PORT        GPIOA
// #define HD44780_D5_GPIO_PIN         GPIO_PIN_4
// #define HD44780_D6_GPIO_PORT        GPIOA
// #define HD44780_D6_GPIO_PIN         GPIO_PIN_5
// #define HD44780_D7_GPIO_PORT        GPIOA
// #define HD44780_D7_GPIO_PIN         GPIO_PIN_6
#define HD44780_RS_GPIO_PORT        LCD_RS_GPIO_Port
#define HD44780_RS_GPIO_PIN         LCD_RS_Pin
#define HD44780_RW_GPIO_PORT        LCD_RW_GPIO_Port
#define HD44780_RW_GPIO_PIN         LCD_RW_Pin
#define HD44780_E_GPIO_PORT         LCD_E_GPIO_Port
#define HD44780_E_GPIO_PIN          LCD_E_Pin
#define HD44780_D4_GPIO_PORT        LCD_D4_GPIO_Port
#define HD44780_D4_GPIO_PIN         LCD_D4_Pin
#define HD44780_D5_GPIO_PORT        LCD_D5_GPIO_Port
#define HD44780_D5_GPIO_PIN         LCD_D5_Pin
#define HD44780_D6_GPIO_PORT        LCD_D6_GPIO_Port
#define HD44780_D6_GPIO_PIN         LCD_D6_Pin
#define HD44780_D7_GPIO_PORT        LCD_D7_GPIO_Port
#define HD44780_D7_GPIO_PIN         LCD_D7_Pin
#elif defined(HD44780_PLATFORM_ESP32_IDF)
#define HD44780_RS_GPIO             GPIO_NUM_10
#define HD44780_RW_GPIO             GPIO_NUM_11
#define HD44780_E_GPIO              GPIO_NUM_12
#define HD44780_D4_GPIO             GPIO_NUM_13
#define HD44780_D5_GPIO             GPIO_NUM_14
#define HD44780_D6_GPIO             GPIO_NUM_15
#define HD44780_D7_GPIO             GPIO_NUM_16
#elif defined(HD44780_PLATFORM_AVR)
#define HD44780_RS_GPIO_PORT        PORTA
#define HD44780_RS_GPIO_DDR         DDRA
#define HD44780_RS_GPIO_BIT         0
#define HD44780_RW_GPIO_PORT        PORTA
#define HD44780_RW_GPIO_DDR         DDRA
#define HD44780_RW_GPIO_BIT         1
#define HD44780_E_GPIO_PORT         PORTA
#define HD44780_E_GPIO_DDR          DDRA
#define HD44780_E_GPIO_BIT          2
#define HD44780_D4_GPIO_PORT        PORTA
#define HD44780_D4_GPIO_PIN         PINA
#define HD44780_D4_GPIO_DDR         DDRA
#define HD44780_D4_GPIO_BIT         4
#define HD44780_D5_GPIO_PORT        PORTA
#define HD44780_D5_GPIO_PIN         PINA
#define HD44780_D5_GPIO_DDR         DDRA
#define HD44780_D5_GPIO_BIT         5
#define HD44780_D6_GPIO_PORT        PORTA
#define HD44780_D6_GPIO_PIN         PINA
#define HD44780_D6_GPIO_DDR         DDRA
#define HD44780_D6_GPIO_BIT         6
#define HD44780_D7_GPIO_PORT        PORTA
#define HD44780_D7_GPIO_PIN         PINA
#define HD44780_D7_GPIO_DDR         DDRA
#define HD44780_D7_GPIO_BIT         7
#endif


/**
 ==================================================================================
                             ##### Functions #####                                 
 ==================================================================================
 */

void
HD44780_Platform_Init(HD44780_Handler_t *Handler);


#ifdef __cplusplus
}
#endif


#endif //! _HD44780_PLATFORM_H_
