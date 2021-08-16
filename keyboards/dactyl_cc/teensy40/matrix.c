/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"

#include "teensy40.h"
#include "config.h"

I2CConfig I2CD_Config;
extern I2CDriver I2CD3;
I2CDriver *_driver = NULL;
static uint8_t expander_reset_loop = 0;
msg_t expander_status = MSG_OK;
uint8_t expander_input_pin_mask = 0;
bool i2c_initialized = false;


#define SPLIT_MUTABLE const
#define SPLIT_MUTABLE_ROW const
#define SPLIT_MUTABLE_COL const


static SPLIT_MUTABLE_ROW pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static SPLIT_MUTABLE_COL pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;


/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
extern matrix_row_t matrix[MATRIX_ROWS];      // debounced values

// user-defined overridable functions
__attribute__((weak)) void matrix_init_pins(void);
__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col);
#ifdef SPLIT_KEYBOARD
__attribute__((weak)) void matrix_slave_scan_kb(void) { matrix_slave_scan_user(); }
__attribute__((weak)) void matrix_slave_scan_user(void) {}
#endif

static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON { setPinInputHigh(pin); }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return readPin(pin);
    } else {
        return 1;
    }
}

// matrix code

#if (DIODE_DIRECTION == COL2ROW)
static const uint8_t expander_col_pins[MATRIX_COLS] = MATRIX_EXPANDER_COL_PINS;
static bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinOutput_writeLow(pin);
        return true;
    }
    return false;
}

static void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinInputHigh_atomic(pin);
    }
}

static void unselect_rows(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        unselect_row(x);
    }
}

__attribute__((weak)) void matrix_init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(col_pins[x]);
        }
    }
}

__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) {  // Select row
        return;                      // skip NO_PIN row
    }
    matrix_output_select_delay();

    // For each col...
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
        uint8_t pin_state = readMatrixPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
    }

    // Unselect row
    unselect_row(current_row);
    matrix_output_unselect_delay(current_row, current_row_value != 0);  // wait for all Col signals to go HIGH

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

#elif (DIODE_DIRECTION == ROW2COL)
static const uint8_t expander_row_pins[MATRIX_ROWS] = MATRIX_EXPANDER_ROW_PINS;
static bool select_col(uint8_t col) {
    if (col < 6)
    {
        pin_t pin = col_pins[col];
        if (pin != NO_PIN) {
            setPinOutput_writeLow(pin);
            return true;
        }
    }
    else if (col < 12)
    {
        _driver->reglen = 1;
        _driver->reg = EXPANDER_COL_REGISTER;
        uint8_t buff = 0xff & ~(1<<(col-6));
        i2cMasterTransmitTimeout(_driver, _driver->addr, &buff, 1, NULL, 0, 10);
        return true;
    }
    return false;
}

static void unselect_col(uint8_t col) {

    if (col < 6)
    {
        pin_t pin = col_pins[col];
        if (pin != NO_PIN) {
            setPinInputHigh_atomic(pin);
        }
    } else if (col < 12)
    {
        _driver->reglen = 1;
        _driver->reg = EXPANDER_COL_REGISTER;
        uint8_t buff = 0;
        i2cMasterTransmitTimeout(_driver, _driver->addr, &buff, 1, NULL, 0, 10);
    }
}

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        unselect_col(x);
    }
}

__attribute__((weak)) void matrix_init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        if (row_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(row_pins[x]);
        }
    }
}

__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    bool key_pressed = false;
    uint8_t column_state = 0;

    // Select col
    if (!select_col(current_col)) {  // select col
        return;                      // skip NO_PIN col
    }
    matrix_output_select_delay();
    if (current_col < 6)
    {
        for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++)
        {
            column_state |= ((readMatrixPin(row_pins[row_index]) == 0) << row_index);
        }
    } else if (current_col < 12)
    {
        _driver->reglen = 1;
        _driver->reg = EXPANDER_ROW_REGISTER;

        expander_status = i2cMasterReceiveTimeout(_driver, _driver->addr, &column_state, 1, 10);
        if (expander_status == MSG_OK)
        {
            column_state = (~column_state) & expander_input_pin_mask;
        } else { column_state = 0; }

    }

        // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Check row pin state
        if (column_state & (1 << row_index)) {
            // Pin LO, set col bit
            current_matrix[row_index] |= (MATRIX_ROW_SHIFTER << current_col);
            key_pressed = true;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~(MATRIX_ROW_SHIFTER << current_col);
        }
    }
    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed);  // wait for all Row signals to go HIGH
}
#endif

void matrix_init(void)
{
    // initialize key pins
    init_expander();
    matrix_init_pins();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        raw_matrix[i] = 0;
        matrix[i]     = 0;
    }

    debounce_init(MATRIX_ROWS);

    matrix_init_quantum();
}


uint8_t matrix_scan(void) {
    if (expander_status != MSG_OK)
    {
        if (++expander_reset_loop == 0)
        {
            print("Reset I2C I/O expander\n");
            init_expander();
            if (expander_status != MSG_OK)
            {
                print("Unable to communicate to expander during reset\n");
            }
        }
    }

    matrix_row_t curr_matrix[MATRIX_ROWS] = {0};

#if (DIODE_DIRECTION == COL2ROW)
    // Set row, read cols
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        matrix_read_cols_on_row(curr_matrix, current_row);
    }
#elif (DIODE_DIRECTION == ROW2COL)
    // Set col, read rows
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
        matrix_read_rows_on_col(curr_matrix, current_col);
    }
#endif
    bool changed = memcmp(raw_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) memcpy(raw_matrix, curr_matrix, sizeof(curr_matrix));


    debounce(raw_matrix, matrix, MATRIX_ROWS, changed);
    matrix_scan_quantum();
    return (uint8_t)changed;
}

void init_expander(void) {
    if (! i2c_initialized) {
        _driver = &I2CD3;
        _driver->addr = mcp23017_i2c_addr;
        i2cInit();
        i2c_initialized = true;
        wait_us(1000000);
    }

    if (! expander_input_pin_mask) {
#if (DIODE_DIRECTION == COL2ROW)
        for (int col = 0; col < MATRIX_COLS; col++) {
            expander_input_pin_mask |= (1 << expander_col_pins[col]);
        }
#elif (DIODE_DIRECTION == ROW2COL)
        for (int row = 0; row < MATRIX_ROWS; row++) {
            expander_input_pin_mask |= (1 << expander_row_pins[row]);
        }
#endif
    }
    _driver->mconfig.baudRate_Hz = 400000;
    i2cStart(_driver, &I2CD_Config);

   uint8_t nullbuff = 0;
   _driver->reglen = 1;
   for (int i = 0; i < 22; i++)
   {
       _driver->reg = i;
       expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
       wait_us(100);
   }

    /*
    Pin direction and pull-up depends on both the diode direction
    and on whether the column register is GPIOA or GPIOB
    +-------+---------------+---------------+
    |       | ROW2COL       | COL2ROW       |
    +-------+---------------+---------------+
    | GPIOA | input, output | output, input |
    +-------+---------------+---------------+
    | GPIOB | output, input | input, output |
    +-------+---------------+---------------+
    */
   _driver->reglen = 1;
   _driver->reg = IODIRA;
#if (EXPANDER_COL_REGISTER == GPIOA)
#   if (DIODE_DIRECTION == COL2ROW)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
        _driver->reg++;
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
#   elif (DIODE_DIRECTION == ROW2COL)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
        _driver->reg++;
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
#   endif
#elif (EXPANDER_COL_REGISTER == GPIOB)
#   if (DIODE_DIRECTION == COL2ROW)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
        _driver->reg++;
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
#   elif (DIODE_DIRECTION == ROW2COL)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
        _driver->reg++;
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
#   endif
#endif

    //i2c_stop();

    // set pull-up
    // - unused  : off : 0
    // - input   : on  : 1
    // - driving : off : 0
    _driver->reglen = 1;
    _driver->reg = GPPUA;
#if (EXPANDER_COL_REGISTER == GPIOA)
#   if (DIODE_DIRECTION == COL2ROW)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
        _driver->reg++;
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
#   elif (DIODE_DIRECTION == ROW2COL)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
        _driver->reg++
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
#   endif
#elif (EXPANDER_COL_REGISTER == GPIOB)
#   if (DIODE_DIRECTION == COL2ROW)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
        _driver->reg++;
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
#   elif (DIODE_DIRECTION == ROW2COL)
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &expander_input_pin_mask, 1, NULL, 0, 10);
        _driver->reg++;
        expander_status = i2cMasterTransmitTimeout(_driver, _driver->addr, &nullbuff, 1, NULL, 0, 10);
#   endif
#endif
}
