/**
 **********************************************************************************
 * @file   HD44780.c
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

/* Includes ---------------------------------------------------------------------*/
#include "HD44780.h"
#include <stdio.h>



/* Private Constants ------------------------------------------------------------*/

// instruction register bit positions, see HD44780U data sheet
#define HD44780_CLR               0   // DB0: clear display
#define HD44780_HOME              1   // DB1: return to home position
#define HD44780_ENTRY_MODE        2   // DB2: set entry mode
#define HD44780_ENTRY_INC         1   // DB1: 1=increment, 0=decrement
#define HD44780_ENTRY_SHIFT       0   // DB2: 1=display shift on
#define HD44780_ON                3   // DB3: turn lcd/cursor on
#define HD44780_ON_DISPLAY        2   // DB2: turn display on
#define HD44780_ON_CURSOR         1   // DB1: turn cursor on
#define HD44780_ON_BLINK          0   // DB0: blinking cursor ?
#define HD44780_MOVE              4   // DB4: move cursor/display
#define HD44780_MOVE_DISP         3   // DB3: move display (0-> cursor) ?
#define HD44780_MOVE_RIGHT        2   // DB2: move right (0-> left) ?
#define HD44780_FUNCTION          5   // DB5: function set
#define HD44780_FUNCTION_8BIT     4   // DB4: set 8BIT mode (0->4BIT mode)
#define HD44780_FUNCTION_2LINES   3   // DB3: two lines (0->one line)
#define HD44780_FUNCTION_10DOTS   2   // DB2: 5x10 font (0->5x7 font)
#define HD44780_CGRAM             6   // DB6: set CG RAM address
#define HD44780_DDRAM             7   // DB7: set DD RAM address
#define HD44780_BUSY              7   // DB7: LCD is busy

// function set: set interface data length and number of display lines
#define Hd44780_FUNCTION_4BIT_1LINE  0x20 // 4-bit interface, single line, 5x7 dots
#define Hd44780_FUNCTION_4BIT_2LINES 0x28 // 4-bit interface, dual line,   5x7 dots
#define Hd44780_FUNCTION_8BIT_1LINE  0x30 // 8-bit interface, single line, 5x7 dots
#define Hd44780_FUNCTION_8BIT_2LINES 0x38 // 8-bit interface, dual line,   5x7 dots

#define HD44780_LINE_LENGTH  0x40 // internal line length of the display
#define HD44780_START_LINE1  0x00 // DDRAM address of first char of line 1
#define HD44780_START_LINE2  0x40 // DDRAM address of first char of line 2
#define HD44780_START_LINE3  0x14 // DDRAM address of first char of line 3
#define HD44780_START_LINE4  0x54 // DDRAM address of first char of line 4

#define HD44780_FUNCTION_DEFAULT  Hd44780_FUNCTION_4BIT_2LINES
#define HD44780_MODE_DEFAULT  ((1<<HD44780_ENTRY_MODE) | (1<<HD44780_ENTRY_INC))



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
HD44780_ToggleE(HD44780_Handler_t *Handler)
{
  Handler->PlatformSetValE(1);
  Handler->PlatformDelayUs(1);
  Handler->PlatformSetValE(0);
}

static uint8_t
HD44780_Read(HD44780_Handler_t *Handler, uint8_t RS)
{
  uint8_t data = 0;

  if (RS)
    Handler->PlatformSetValRs(1); // RS=1: read data
  else
    Handler->PlatformSetValRs(0); // RS=0: read busy flag

  Handler->PlatformSetValRw(1);   // RW=1  read mode

  // Read high nibble first
  Handler->PlatformSetDirData(0);
  Handler->PlatformSetValE(1);
  Handler->PlatformDelayUs(1);
  data = Handler->PlatformReadData() & 0xF0;
  Handler->PlatformSetValE(0);

  Handler->PlatformDelayUs(1);

  // Read low nibble
  Handler->PlatformSetValE(1);
  Handler->PlatformDelayUs(1);
  data |= Handler->PlatformReadData() >> 4;
  Handler->PlatformSetValE(0);

  return data;
}

static void
HD44780_Write(HD44780_Handler_t *Handler, uint8_t Data, uint8_t RS)
{
  if (RS)
  {
    // Write data
    Handler->PlatformSetValRs(1);
  }
  else
  {
    // Write instruction
    Handler->PlatformSetValRs(0);
  }

  // Write mode
  Handler->PlatformSetValRw(0);
  Handler->PlatformSetDirData(1);

  // Write high nibble first
  Handler->PlatformWriteData(Data & 0xF0);
  HD44780_ToggleE(Handler);

  // Write low nibble
  Handler->PlatformWriteData((Data << 4) & 0xF0);
  HD44780_ToggleE(Handler);

  // All data pins high (inactive)
  Handler->PlatformWriteData(0xFF);
}

static uint8_t
HD44780_WaitBusy(HD44780_Handler_t *Handler)
{
  register uint8_t c;

  // Wait until busy flag is cleared
  while ((c = HD44780_Read(Handler, 0)) & (1 << HD44780_BUSY))
    Handler->PlatformDelayMs(1);

  // The address counter is updated 4us after the busy flag is cleared
  Handler->PlatformDelayUs(2);

  // Now read the address counter
  return (HD44780_Read(Handler, 0)); // Return address counter
}

static inline void
HD44780_NewLine(HD44780_Handler_t *Handler, uint8_t Pos)
{
  register uint8_t addressCounter;

  if (Pos < HD44780_START_LINE3)
    addressCounter = HD44780_START_LINE2;
  else if ((Pos >= HD44780_START_LINE2) && (Pos < HD44780_START_LINE4))
    addressCounter = HD44780_START_LINE3;
  else if ((Pos >= HD44780_START_LINE3) && (Pos < HD44780_START_LINE2))
    addressCounter = HD44780_START_LINE4;
  else
    addressCounter = HD44780_START_LINE1;

  HD44780_Command(Handler, (1 << HD44780_DDRAM) + addressCounter);
}



/**
 ==================================================================================
                            ##### Public Functions #####                           
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
             uint8_t DispAttr)
{

  if (Handler->PlatformInitRsRwE)
    Handler->PlatformInitRsRwE();

  if (Handler->PlatformSetValRs == NULL ||
      Handler->PlatformSetValRw == NULL ||
      Handler->PlatformSetValE == NULL ||
      Handler->PlatformSetDirData == NULL ||
      Handler->PlatformWriteData == NULL ||
      Handler->PlatformReadData == NULL ||
      Handler->PlatformDelayMs == NULL ||
      Handler->PlatformDelayUs == NULL)
    return HD44780_FAIL;

  Handler->Lines = Lines;
  Handler->Length = Length;

  // Change to 4 bit I/O mode
  HD44780_Write(Handler, 0x33, 0);
  HD44780_Write(Handler, 0x32, 0);

  HD44780_Command(Handler, HD44780_FUNCTION_DEFAULT); // function set: display lines
  HD44780_Command(Handler, HD44780_DISP_OFF);         // display off
  HD44780_ClearScreen(Handler);                   // display clear
  HD44780_Command(Handler, HD44780_MODE_DEFAULT);     // set entry mode
  HD44780_Command(Handler, DispAttr);             // display/cursor control

  return HD44780_OK;
}

/**
 * @brief  Clear display and set cursor to home position
 * @param  Handler: Pointer to handler
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_ClearScreen(HD44780_Handler_t *Handler)
{
  HD44780_Command(Handler, 1 << HD44780_CLR);

  return HD44780_OK;
}

/**
 * @brief  Set cursor to home position
 * @param  Handler: Pointer to handler
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Home(HD44780_Handler_t *Handler)
{
  HD44780_Command(Handler, 1 << HD44780_HOME);

  return HD44780_OK;
}

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
HD44780_GoToXY(HD44780_Handler_t *Handler, uint8_t X, uint8_t Y)
{
  switch (Y)
  {
  case 0:
    HD44780_Command(Handler, (1 << HD44780_DDRAM) + HD44780_START_LINE1 + X);
    break;

  case 1:
    HD44780_Command(Handler, (1 << HD44780_DDRAM) + HD44780_START_LINE2 + X);
    break;

  case 2:
    HD44780_Command(Handler, (1 << HD44780_DDRAM) + HD44780_START_LINE3 + X);
    break;

  case 3:
    HD44780_Command(Handler, (1 << HD44780_DDRAM) + HD44780_START_LINE4 + X);
    break;
  }

  return HD44780_OK;
}

/**
 * @brief  Display character at current cursor position
 * @param  Handler: Pointer to handler
 * @param  C: Character to display
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Putc(HD44780_Handler_t *Handler, char C)
{
  uint8_t pos;

  pos = HD44780_WaitBusy(Handler);   // read busy-flag and address counter
  if (C == '\n')
  {
    HD44780_NewLine(Handler, pos);
  }
  else
  {
    HD44780_Write(Handler, C, 1);
  }

  return HD44780_OK;
}

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
HD44780_Puts(HD44780_Handler_t *Handler, char *Str, uint8_t Len)
{
  register char c;

  if (Len == 0)
  {
    while ((c = *Str++))
    {
      HD44780_Putc(Handler, c);
    }
  }
  else
  {
    while (Len--)
    {
      c = *Str++;
      if (c == 0)
        break;
      HD44780_Putc(Handler, c);
    }
  }

  return HD44780_OK;
}

/**
 * @brief  Send LCD controller instruction command
 * @param  Handler: Pointer to handler
 * @param  Cmd: Command to send
 * @retval HD44780_Result_t
 *         - HD44780_OK: Operation was successful.
 *         - HD44780_FAIL: Failed to send or receive data.
 */
HD44780_Result_t
HD44780_Command(HD44780_Handler_t *Handler, uint8_t Cmd)
{
  HD44780_WaitBusy(Handler);
  HD44780_Write(Handler, Cmd, 0);

  return HD44780_OK;
}

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
HD44780_Data(HD44780_Handler_t *Handler, uint8_t Data)
{
  HD44780_WaitBusy(Handler);
  HD44780_Write(Handler, Data, 1);

  return HD44780_OK;
}
