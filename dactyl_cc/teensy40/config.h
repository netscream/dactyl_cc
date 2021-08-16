/* Copyright 2021 QMK
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

#pragma once

/* USB Device descriptor parameter */
#undef VENDOR_ID
#define VENDOR_ID 0x1209 // Generic
#undef PRODUCT_ID
#define PRODUCT_ID 0xbb00 // Keyplus split keyboard firmware
#undef DEVICE_VER
#define DEVICE_VER 0x0001
#undef MANUFACTURER
#define MANUFACTURER "https://github.com/netscream"
#undef PRODUCT
#define PRODUCT "dactyl_cc"

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 12

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
 */

#define EXPANDER_COL_REGISTER GPIOB
#define EXPANDER_ROW_REGISTER GPIOA
#define MATRIX_EXPANDER_COL_PINS {0, 1, 2, 3, 4, 5}
#define MATRIX_EXPANDER_ROW_PINS {0, 1, 2, 3, 4, 5}


#define MATRIX_ROW_PINS \
{                       \
            LINE_PIN23, \
            LINE_PIN22, \
            LINE_PIN21, \
            LINE_PIN20, \
            LINE_PIN19, \
            LINE_PIN18  \
}

#define MATRIX_COL_PINS  \
    {                    \
            LINE_PIN5,   \
            LINE_PIN4,   \
            LINE_PIN3,   \
            LINE_PIN2,   \
            LINE_PIN1,   \
            LINE_PIN0,   \
            NO_PIN,      \
            NO_PIN,      \
            NO_PIN,      \
            NO_PIN,      \
            NO_PIN,      \
            NO_PIN       \
    }

#define UNUSED_PINS {LINE_PIN}

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* Well-worn Cherry MX key switches can bounce for up to 20ms, despite the
 * Cherry data sheet specifying 5ms. Because we use the sym_eager_pk debounce
 * algorithm, this debounce latency only affects key releases (not key
 * presses). */
#undef DEBOUNCE
#define DEBOUNCE 10

#define IGNORE_MOD_TAP_INTERRUPT

// Reduce input latency by lowering the USB polling interval
// from its 10ms default to the 125μs minimum that USB 2.x (High Speed) allows:
#define USB_POLLING_INTERVAL_MS 1

/* We use the i.MX RT1060 high-speed GPIOs (GPIO6-9) which are connected to the
 * AHB bus (AHB_CLK_ROOT), which runs at the same speed as the ARM Core Clock,
 * i.e. 600 MHz. See MIMXRT1062, page 949, 12.1 Chip-specific GPIO information.
 * No additional delay is necessary. */

// in clock cycles
#define GPIO_INPUT_PIN_DELAY 0

#define LED_PIN_ON_STATE 0
#define LED_NUM_LOCK_PIN LINE_PIN26
#define LED_CAPS_LOCK_PIN LINE_PIN12
#define LED_SCROLL_LOCK_PIN LINE_PIN25
#define LED_COMPOSE_PIN LINE_PIN24

#define MAGIC_KEY_SWITCH_LAYER_WITH_FKEYS 0
#define MAGIC_KEY_SWITCH_LAYER_WITH_NKEYS 1
#define MAGIC_KEY_SWITCH_LAYER_WITH_CUSTOM 0
#define MAGIC_KEY_HELP1          Q
#define MAGIC_KEY_HELP2          W
#define MAGIC_KEY_DEBUG          Z
#define MAGIC_KEY_DEBUG_MATRIX   X
#define MAGIC_KEY_DEBUG_KBD      C
#define MAGIC_KEY_DEBUG_MOUSE    V
#define MAGIC_KEY_VERSION        T
#define MAGIC_KEY_STATUS         E
#define MAGIC_KEY_CONSOLE        A
#define MAGIC_KEY_LAYER0_ALT1    ESC
#define MAGIC_KEY_LAYER0_ALT2    GRAVE
#define MAGIC_KEY_LAYER0         0
#define MAGIC_KEY_LAYER1         1
#define MAGIC_KEY_LAYER2         2
#define MAGIC_KEY_LAYER3         3
#define MAGIC_KEY_LAYER4         4
#define MAGIC_KEY_LAYER5         5
#define MAGIC_KEY_LAYER6         6
#define MAGIC_KEY_LAYER7         7
#define MAGIC_KEY_LAYER8         8
#define MAGIC_KEY_LAYER9         9
#define MAGIC_KEY_BOOTLOADER     B
#define MAGIC_KEY_LOCK           CAPS
#define MAGIC_KEY_EEPROM         S
#define MAGIC_KEY_NKRO           D
#define MAGIC_KEY_SLEEP_LED      F
