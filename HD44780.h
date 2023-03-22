/**
 **********************************************************************************
 * @file   HD44780.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  Basic routines for interfacing a HD44780-based text LCD display
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
#ifndef _HD44780_H_
#define _HD44780_H_

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>


/* Exported Constants -----------------------------------------------------------*/
/**
 * @brief  HD44780U LCD controller commands
 */
#define HD44780_DISP_OFF             0x08  // display off
#define HD44780_DISP_ON              0x0C  // display on, cursor off
#define HD44780_DISP_ON_BLINK        0x0D  // display on, cursor off, blink char
#define HD44780_DISP_ON_CURSOR       0x0E  // display on, cursor on
#define HD44780_DISP_ON_CURSOR_BLINK 0x0F  // display on, cursor on, blink char

/**
 * @brief  Entry mode set commands: display shift on/off, dec/inc cursor move
 *         direction
*/
#define HD44780_ENTRY_DEC       0x04  // display shift off, dec cursor move dir
#define HD44780_ENTRY_DEC_SHIFT 0x05  // display shift on,  dec cursor move dir
#define HD44780_ENTRY_INC_      0x06  // display shift off, inc cursor move dir
#define HD44780_ENTRY_INC_SHIFT 0x07  // display shift on,  inc cursor move dir

/**
 * @brief  Cursor or display shift commands: move cursor or shift display 
*/
#define HD44780_MOVE_CURSOR_LEFT  0x10  // move cursor left  (decrement)
#define HD44780_MOVE_CURSOR_RIGHT 0x14  // move cursor right (increment)
#define HD44780_MOVE_DISP_LEFT    0x18  // shift display left
#define HD44780_MOVE_DISP_RIGHT   0x1C  // shift display right


/* Exported Data Types ----------------------------------------------------------*/
/**
 * @brief  Library functions result data type
 */
typedef enum HD44780_Result_e
{
  HD44780_OK   = 0,
  HD44780_FAIL = 1,
} HD44780_Result_t;

/**
 * @brief  Handler data type
 * @note   User must initialize this this functions before using library:
 *         - PlatformInitRsRwE
 *         - PlatformSetValRs
 *         - PlatformSetValRw
 *         - PlatformSetValE
 *         - PlatformSetDirData
 *         - PlatformWriteData
 *         - PlatformReadData
 *         - PlatformDelayMs
 *         - PlatformDelayUs
 */
typedef struct HD44780_Handler_s
{
  uint8_t Lines;
  uint8_t Length;
  
  void (*PlatformInitRsRwE)(void);
  void (*PlatformSetValRs)(uint8_t);
  void (*PlatformSetValRw)(uint8_t);
  void (*PlatformSetValE)(uint8_t);
  void (*PlatformSetDirData)(uint8_t);
  void (*PlatformWriteData)(uint8_t);
  uint8_t (*PlatformReadData)(void);
  void (*PlatformDelayMs)(uint16_t);
  void (*PlatformDelayUs)(uint16_t);
} HD44780_Handler_t;



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  
 * @param  Handler: Pointer to handler
 * @param  Lines: Number of lines
 * @param  Length: Number of characters in a line
 * @param  DispAttr: Display attributes
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Init(HD44780_Handler_t *Handler,
             uint8_t Lines, uint8_t Length,
             uint8_t DispAttr);


/**
 * @brief  Clear display and set cursor to home position
 * @param  Handler: Pointer to handler
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_ClearScreen(HD44780_Handler_t *Handler);


/**
 * @brief  Set cursor to home position
 * @param  Handler: Pointer to handler
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Home(HD44780_Handler_t *Handler);


/**
 * @brief  Set cursor to specified position
 * @param  Handler: Pointer to handler
 * @param  X: X position
 * @param  Y: Y position
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_GoToXY(HD44780_Handler_t *Handler, uint8_t X, uint8_t Y);


/**
 * @brief  Display character at current cursor position
 * @param  Handler: Pointer to handler
 * @param  C: Character to display
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Putc(HD44780_Handler_t *Handler, char C);


/**
 * @brief  Display string without auto linefeed
 * @param  Handler: Pointer to handler
 * @param  Str: String to display
 * @param  Len: Length of string. If 0, string will be displayed until null
 *              character.
 * 
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Puts(HD44780_Handler_t *Handler, char *Str, uint8_t Len);


/**
 * @brief  Send LCD controller instruction command
 * @param  Handler: Pointer to handler
 * @param  Cmd: Command to send
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Command(HD44780_Handler_t *Handler, uint8_t Cmd);


/**
 * @brief  Send data byte to LCD controller.
 * @note   Similar to HD44780_Putc(), but without interpreting LF
 * @param  Handler: Pointer to handler
 * @param  Data: Data to send
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Data(HD44780_Handler_t *Handler, uint8_t Data);


#ifdef __cplusplus
}
#endif

#endif  //! _HD44780_H_
