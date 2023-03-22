# HD44780 Library
Basic routines for interfacing a HD44780-based text LCD display.
Note that this library only supports 4-bit mode at this time.


## Hardware Support
It is easy to port this library to any platform. But now it is ready for use in:
- STM32 (HAL)
- ESP32 (esp-idf)
- AVR (ATmega32)

## How To Use
1. Add `HD44780.h` and `HD44780.c` files to your project.  It is optional to use `HD44780_platform.h` and `HD44780_platform.c` files (open and config `HD44780_platform.h` file).
2. Initialize platform-dependent part of handler.
4. Call `HD44780_Init()`.
5. Call other functions and enjoy.

## Example
<details>
<summary>Using HD44780_platform files</summary>

```c
#include <stdio.h>
#include "HD44780.h"
#include "HD44780_platform.h"

int main(void)
{
  HD44780_Handler_t Handler = {0};

  HD44780_Platform_Init(&Handler);
  HD44780_Init(&Handler, 2, 16, HD44780_DISP_ON);
  HD44780_ClearScreen(&Handler);
  HD44780_GoToXY(&Handler, 3, 0);
  HD44780_Puts(&Handler, "github.com", 0);
  HD44780_GoToXY(&Handler, 2, 1);
  HD44780_Puts(&Handler, "Hossein-M98", 0);
  HD44780_Command(&Handler, HD44780_DISP_ON_CURSOR_BLINK);

  while (1)
  {
  }

  return 0;
}
```
</details>


<details>
<summary>Without using HD44780_platform files (ATmega32)</summary>

```c
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "HD44780.h"

/*
 * We consider that the data pins are connected to the
 * last four bits of the port.
 * Also, we consider that the control pins are connected
 * to the first three bits of the port.
 */
#define HD44780_PORT    PORTB
#define HD44780_PIN     PINB
#define HD44780_DDR     DDRB

static void
PlatformInitRsRwE(void)
{
  HD44780_DDR |= (1 << 0) | (1 << 1) | (1 << 2);
}

static void
PlatformSetValRs(uint8_t Level)
{
  if (Level)
    HD44780_PORT |= (1 << 0);
  else
    HD44780_PORT &= ~(1 << 0);
}

static void
PlatformSetValRw(uint8_t Level)
{
  if (Level)
    HD44780_PORT |= (1 << 1);
  else
    HD44780_PORT &= ~(1 << 1);
}

static void
PlatformSetValE(uint8_t Level)
{
  if (Level)
    HD44780_PORT |= (1 << 2);
  else
    HD44780_PORT &= ~(1 << 2);
}

static void
PlatformSetDirData(uint8_t Dir)
{
  if (Dir)
    HD44780_DDR |= 0xF0;
  else
    HD44780_DDR &= ~0xF0;
}

static void
PlatformWriteData(uint8_t Data)
{
  HD44780_PORT = (HD44780_PORT & 0x0F) | (Data & 0xF0);
}

static uint8_t
PlatformReadData(void)
{
  return (HD44780_PIN & 0xF0);
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


int main(void)
{
  HD44780_Handler_t Handler = {0};

  Handler.PlatformInitRsRwE = PlatformInitRsRwE;
  Handler.PlatformSetValRs = PlatformSetValRs;
  Handler.PlatformSetValRw = PlatformSetValRw;
  Handler.PlatformSetValE = PlatformSetValE;
  Handler.PlatformSetDirData = PlatformSetDirData;
  Handler.PlatformWriteData = PlatformWriteData;
  Handler.PlatformReadData = PlatformReadData;
  Handler.PlatformDelayMs = PlatformDelayMs;
  Handler.PlatformDelayUs = PlatformDelayUs;

  HD44780_Init(&Handler, 2, 16, HD44780_DISP_ON);
  HD44780_ClearScreen(&Handler);
  HD44780_GoToXY(&Handler, 3, 0);
  HD44780_Puts(&Handler, "github.com", 0);
  HD44780_GoToXY(&Handler, 2, 1);
  HD44780_Puts(&Handler, "Hossein-M98", 0);
  HD44780_Command(&Handler, HD44780_DISP_ON_CURSOR_BLINK);

  while (1)
  {
  }

  return 0;
}
```
</details>
