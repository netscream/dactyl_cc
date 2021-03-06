/* Copyright 2020 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "teensy40.h"

#include "mcuconf.h"
//#include "print.h"
#include "hal.h"

//extern I2CDriver I2CD3;
//I2CConfig I2CD_Config;

void matrix_init_kb(void)
{
     //debug_enable = true;
     //debug_matrix = true;
     //debug_keyboard = true;
     //debug_mouse = true;
    matrix_init_user();

// Turn on the Teensy 4.x Power LED:
#define LED_POWER LINE_PIN13
    setPinOutput(LED_POWER);
    writePinHigh(LED_POWER);

    //mcp23017_Init(&I2CD3, mcp23017_i2c_addr1);
}

// delay_inline sleeps for |cycles| (e.g. sleeping for F_CPU will sleep 1s).
// delay_inline assumes the cycle counter has already been initialized and
// should not be modified, i.e. it is safe to call during keyboard matrix scan.
//
// ChibiOS enables the cycle counter in chcore_v7m.c:
// https://github.com/ChibiOS/ChibiOS/blob/b63023915c304092acb9f33bbab40f3ec07a7f0e/os/common/ports/ARMCMx/chcore_v7m.c#L263
static void delay_inline(const uint32_t cycles)
{
    //print("Delay Inline\n");
    const uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles) {
        // busy-loop until time has passed
    }
}

void matrix_output_unselect_delay(uint8_t line, bool key_pressed)
{
    //print("Matrix output unselect delay!\n");
    // Use the cycle counter to do precise timing in microseconds. The ChibiOS
    // thread sleep functions only allow sleep durations starting at 1 tick, which
    // is 100??s in our configuration.

    // Empirically: e.g. 5??s is not enough, will result in keys that don???t work
    // and ghost key presses. 10??s seems to work well.

    // 600 cycles at 0.6 cycles/ns == 1??s
    const uint32_t cycles_per_us = 600;
    delay_inline(10 * cycles_per_us);
}
