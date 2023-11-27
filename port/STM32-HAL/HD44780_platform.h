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
#define HD44780_RS_GPIO_PORT        GPIOA
#define HD44780_RS_GPIO_PIN         GPIO_PIN_0
#define HD44780_RW_GPIO_PORT        GPIOA
#define HD44780_RW_GPIO_PIN         GPIO_PIN_1
#define HD44780_E_GPIO_PORT         GPIOA
#define HD44780_E_GPIO_PIN          GPIO_PIN_2
#define HD44780_D4_GPIO_PORT        GPIOA
#define HD44780_D4_GPIO_PIN         GPIO_PIN_3
#define HD44780_D5_GPIO_PORT        GPIOA
#define HD44780_D5_GPIO_PIN         GPIO_PIN_4
#define HD44780_D6_GPIO_PORT        GPIOA
#define HD44780_D6_GPIO_PIN         GPIO_PIN_5
#define HD44780_D7_GPIO_PORT        GPIOA
#define HD44780_D7_GPIO_PIN         GPIO_PIN_6



/**
 ==================================================================================
                             ##### Functions #####                                 
 ==================================================================================
 */

/**
 * @brief  Initialize platform device to communicate HD44780.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
HD44780_Platform_Init(HD44780_Handler_t *Handler);


#ifdef __cplusplus
}
#endif


#endif //! _HD44780_PLATFORM_H_
