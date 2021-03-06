/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_i2c_lld.h
 * @brief   PLATFORM I2C subsystem low level driver header.
 *
 * @addtogroup I2C
 * @{
 */

#ifndef HAL_I2C_LLD_H
#define HAL_I2C_LLD_H
#include <stdint.h>
#include <stdbool.h>
#include "fsl_iomuxc.h"
#include "fsl_lpi2c.h"

#if (HAL_USE_I2C == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
#define MSG_I2C_ERROR (msg_t)-100
#define MSG_I2C_RESET (msg_t)-101
#define MSG_I2C_TIMEOUT (msg_t)-102

#define MSG_I2C_PinLowFlag (msg_t)-110
#define MSG_I2C_ArbitrationLostFlag (msg_t)-111
#define MSG_I2C_NackDetectFlag (msg_t)-112
#define MSG_I2C_FifoErrorFlag (msg_t)-113


/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/
#define MIMXRT1062_I2C_FAST_MODE      0
#define MIMXRT1062_I2C_FAST_PLUS_MODE 1
#define MIMXRT1062_I2C_HS_MODE        2

#define MIMXRT1062_TIMINGS_NO_PRESCALE   0
#define MIMXRT1062_TIMINGS_NO_FILTSCL    1
#define MIMXRT1062_TIMINGS_NO_FILTSDA    2
#define MIMXRT1062_TIMINGS_NO_SETHOLD    3
#define MIMXRT1062_TIMINGS_NO_CLKLO      4
#define MIMXRT1062_TIMINGS_NO_CLKHI      5
#define MIMXRT1062_TIMINGS_NO_DATAVID    6

#define MIMXRT1062_I2C_I2C1_PRIORITY           4
#define MIMXRT1062_I2C_I2C2_PRIORITY           4
#define MIMXRT1062_I2C_I2C3_PRIORITY           4

#define MIMXRT1062_I2C_TIMEOUT                 100

#define I2C_ADDR_READ  1
#define I2C_ADDR_WRITE 0
enum e_i2c_cmd
{
    i2c_transm,
    i2c_recv,
    i2c_gen_stop,
    i2c_recv_disregard,
    i2c_gen_repeat_start_transm,
    i2c_gen_repeat_start_transm_nack,
    i2c_gen_repeat_start_transm_high_speed,
    i2c_gen_repeat_start_transm_high_speed_nack
};
/**
 * @name    PLATFORM configuration options
 * @{
 */
/**
 * @brief   I2C1 driver enable switch.
 * @details If set to @p TRUE the support for I2C1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(MIMXRT1062_I2C_USE_I2C1) || defined(__DOXYGEN__)
#define MIMXRT1062_I2C_USE_I2C1                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type representing an I2C address.
 */
typedef uint16_t i2caddr_t;

/**
 * @brief   Type of I2C Driver condition flags.
 */
typedef uint32_t i2cflags_t;

/**
 * @brief   Type of I2C driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /* End of the mandatory fields.*/
  uint32_t                  clock_speed;
} I2CConfig;

/**
 * @brief   Type of a structure representing an I2C driver.
 */
typedef struct I2CDriver I2CDriver;

/**
 * @brief   Structure representing an I2C driver.
 */
struct I2CDriver {
  /**
   * @brief   Driver state.
   */
  i2cstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const I2CConfig           *config;
  /**
   * @brief   Error flags.
   */
  i2cflags_t                errors;
#if (I2C_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  mutex_t                   mutex;
#endif
#if defined(I2C_DRIVER_EXT_FIELDS)
  I2C_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  // Begin of IMXRT1062 i2c fields
  thread_reference_t        thread;

  LPI2C_Type              * i2c;



  // ==== Transfer Configs ====
  bool                      send_address;
  i2caddr_t                 addr;
  enum e_i2c_cmd            cmd;
  uint8_t                 * rxbuff;
  size_t                    rxlen;
  const uint8_t           * txbuff;
  size_t                    txlen;
  // Register stuff
  uint32_t                  reglen;
  uint32_t                  reg;


  // Interupt Vector
  uint32_t                  i2c_irq;
  uint32_t                  i2c_irq_prio;
  uint32_t                  irq_flags;

  // Timings
  const uint8_t           * i2c_timings;

  // lpi2c config
  lpi2c_master_config_t     mconfig;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Get errors from I2C driver.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define i2c_lld_get_errors(i2cp) ((i2cp)->errors)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (MIMXRT1062_I2C_USE_I2C0 == TRUE) && !defined(__DOXYGEN__)
extern I2CDriver I2CD1;
#endif

#if (MIMXRT1062_I2C_USE_I2C1 == TRUE) && !defined(__DOXYGEN__)
extern I2CDriver I2CD2;
#endif

#if (MIMXRT1062_I2C_USE_I2C2 == TRUE) && !defined(__DOXYGEN__)
extern I2CDriver I2CD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void i2c_lld_init(void);
  void i2c_lld_start(I2CDriver *i2cp);
  void i2c_lld_stop(I2CDriver *i2cp);
  msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                        const uint8_t *txbuf, size_t txbytes,
                                        uint8_t *rxbuf, size_t rxbytes,
                                        sysinterval_t timeout);
  msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                       uint8_t *rxbuf, size_t rxbytes,
                                       sysinterval_t timeout);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2C == TRUE */

#endif /* HAL_I2C_LLD_H */

/** @} */
