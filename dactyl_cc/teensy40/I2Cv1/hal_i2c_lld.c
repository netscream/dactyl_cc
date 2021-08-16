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
 * @file    hal_i2c_lld.c
 * @brief   PLATFORM I2C subsystem low level driver source.
 *
 * @addtogroup I2C
 * @{
 */
#include "osal.h"
#include "hal.h"

#include "MIMXRT1062.h"
#include "fsl_iomuxc.h"
#include "fsl_lpi2c.h"

#include "print.h"


#if (HAL_USE_I2C == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/


//static const uint8_t i2c_timings[3][7] = { {0x01, 0x02, 0x02, 0x11, 0x28, 0x1F, 0x08},  {0x01, 0x02, 0x02, 0x07, 0x0F, 0x0B, 0x01}, {0x01, 0x00, 0x00, 0x04, 0x04, 0x02, 0x01} };
/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.lpi2c"
#endif

/**
 * @brief   I2C1 driver identifier.
 */

#if (MIMXRT1062_I2C_USE_I2C1 == TRUE) || defined(__DOXYGEN__)
I2CDriver I2CD1;
#endif

#if (MIMXRT1062_I2C_USE_I2C2 == TRUE) || defined(__DOXYGEN__)
I2CDriver I2CD2;
#endif

#if (MIMXRT1062_I2C_USE_I2C3 == TRUE) || defined(__DOXYGEN__)
I2CDriver I2CD3;
#endif
/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
/**
 *   @brief Common I2C IRQ handler
 *
 *   @param[in] i2cp    Communication channel associated to the I2C
 */
static void i2c_lld_serve_interrupt(I2CDriver *i2cp)
{
    /*
    LPI2C_Type * i2c = i2cp->i2c;

    print("In I2C Interupt!\n");
    const uint32_t flags = LPI2C_MasterGetStatusFlags(i2c);
    // Stop interrupts
    //i2c->MIER = 0;

    //i2c->MSR = 0; //|= flags;

    i2c->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP(1); // Turn off autostop

    //osalSysLockFromISR();
    //osalSysUnlockFromISR();
    switch (flags & i2cp->irq_flags)
    {
        case LPI2C_MSR_NDF(1): // Nack
        break;
        case LPI2C_MSR_SDF(1): // Stop
        break;
        case LPI2C_MSR_TDF(1): // Transfer
        break;
        case LPI2C_MSR_RDF(1): // Receive
        break;
    }
    print("I2C Interrupt!\n");
    //i2c->MIER = i2cp->irq_flags; // Enable interrupts
    //osalThreadResumeS(&i2cp->thread, MSG_OK);
    */
   return;
}

/*
static inline uint32_t MasterGetStatusFlags(LPI2C_Type * i2c) { return i2c->MSR; }
static inline uint32_t MasterClearStatusFlags(LPI2C_Type * i2c, uint32_t X) { return i2c->MSR = X; }
static inline void MasterGetFifoCounts(LPI2C_Type * i2c, size_t * rxcount, size_t * txcount)
{
    if (NULL != rxcount)
    {
        *rxcount = ((i2c->MFSR & LPI2C_MFSR_RXCOUNT_MASK) >> LPI2C_MFSR_RXCOUNT_SHIFT);
    }
    if (NULL != txcount)
    {
        *txcount = ((i2c->MFSR & LPI2C_MFSR_TXCOUNT_MASK) >> LPI2C_MFSR_TXCOUNT_SHIFT);
    }
}

static msg_t MasterCheckAndClearError(LPI2C_Type * i2c, uint32_t status)
{
    msg_t m = MSG_OK;
    status &= (uint32_t) kLPI2C_MasterErrorFlags;
    if (0 != status)
    {
        if (0 != (status & (uint32_t)kLPI2C_MasterPinLowTimeoutFlag))
        {
            m = MSG_I2C_PinLowFlag;
        }
        else if (0U != (status & (uint32_t)kLPI2C_MasterArbitrationLostFlag))
        {
            m = MSG_I2C_ArbitrationLostFlag;
        }
        else if (0U != (status & (uint32_t)kLPI2C_MasterNackDetectFlag))
        {
            m = MSG_I2C_NackDetectFlag;
        }
        else if (0U != (status & (uint32_t)kLPI2C_MasterFifoErrFlag))
        {
            m = MSG_I2C_FifoErrorFlag;
        } else {  }


        MasterClearStatusFlags(i2c, status);
        i2c->MCR |= LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK;
    }

    return m;
}

static msg_t MasterCheckForBusyBus(LPI2C_Type * i2c)
{
    uint32_t stat = MasterGetStatusFlags(i2c);

    // Master Bus should be busy but not the Master itself
    if ((!(stat & kLPI2C_MasterBusBusyFlag) && (stat & kLPI2C_MasterBusyFlag)))
        return MSG_TIMEOUT;
    return MSG_OK;
}

static msg_t MasterWaitForTxReady(I2CDriver *i2cp)
{
    LPI2C_Type * i2c = i2cp->i2c;
    msg_t m = MSG_OK;
    uint32_t status;
    size_t txCount = 0;
    size_t txFifoSize = (int32_t)FSL_FEATURE_LPI2C_FIFO_SIZEn(i2c);

    int32_t retries = I2C_RETRY_TIMES;

    for (int ri = 0; ri < retries; ri++)
    {
        if (0) {
        // Get the number of words in the tx fifo and compute empty slots.
        MasterGetFifoCounts(i2c, NULL, &txCount);
        txCount = txFifoSize - txCount;

        // Check for error flags.
        status = MasterGetStatusFlags(i2c);
        if (MSG_OK != (m = MasterCheckAndClearError(i2c, status)))
            break;

        if (txCount > 0)
            break;
        }
        if (ri == (retries-1))
            m = MSG_I2C_TIMEOUT;
    }

    return m;
}

static msg_t MasterSend(I2CDriver *i2cp)
{
    msg_t m = MSG_OK;
    LPI2C_Type * i2c = i2cp->i2c;

    const uint8_t *buf = i2cp->txbuff;

    // Transfer the buffer
    for (int li = 0; li < i2cp->txlen; li++)
    {
        if (MSG_OK == (m = MasterWaitForTxReady(i2cp)))
        {
            i2c->MTDR = *buf++;
        } else { break; }
    }

    return m;
}

static msg_t MasterReceive(I2CDriver *i2cp)
{
    msg_t m = MSG_OK;
    if (i2cp->rxlen  > 0)
    {
        print("Master receive!\n");
        LPI2C_Type * i2c = i2cp->i2c;
        int32_t retries = 0;

        uint8_t * tmpbuf = i2cp->rxbuff;
        int32_t tmprxbytes = i2cp->rxlen;
        if (MSG_OK == (m = MasterWaitForTxReady(i2cp)))
        {
            i2c->MTDR = LPI2C_MTDR_CMD(i2c_recv) | (uint32_t)(tmprxbytes-1);

            while (0 < tmprxbytes)
            {

                uint32_t val = 0;

                if (MSG_OK == (m = LPI2C_MasterCheckAndClearError(i2c, LPI2C_MasterGetStatusFlags(i2c))))
                {
                    retries = I2C_RETRY_TIMES;
                    do
                    {
                        val = i2c->MRDR;
                        if (--retries < 0) { return MSG_TIMEOUT; }
                    } while ((val & LPI2C_MRDR_RXEMPTY_MASK));
                } else { break; }

                *tmpbuf++ = (uint8_t)(val & LPI2C_MRDR_DATA_MASK);
                tmprxbytes--;
            }
        }

    }
    return m;
}

static msg_t MasterStop(I2CDriver *i2cp)
{
    msg_t m = MSG_OK;
    LPI2C_Type * i2c = i2cp->i2c;
    LPI2C_MasterCheckAndClearError(i2c, LPI2C_MasterGetStatusFlags(i2c));
    if (MSG_OK == (m = MasterWaitForTxReady(i2cp)))
    {
        i2c->MTDR = LPI2C_MTDR_CMD(i2c_gen_stop);

        uint32_t flags = MasterGetStatusFlags(i2c);

        if ((flags & LPI2C_MSR_SDF_MASK) && (flags & LPI2C_MSR_TDF_MASK)) // STop and tx ready flags
        {
            MasterClearStatusFlags(i2c, LPI2C_MSR_SDF_MASK);
        } else { m = MSG_TIMEOUT; }
    }
    return m;
}

static msg_t MasterTransferBlocking(I2CDriver *i2cp, sysinterval_t timeout)
{
    msg_t msg = MSG_OK;
    systime_t start = 0, end = 0;
    LPI2C_Type * i2c = i2cp->i2c;
    uint32_t cmdbuff[7] = {0};
    uint32_t cmdind = 0;

    // Unlock during the wait
    osalSysUnlock();

    start = osalOsGetSystemTimeX();
    end = start + OSAL_MS2I(MIMXRT1062_I2C_TIMEOUT);

    // Wait for i2c not being busy
    while(true && MSG_OK == msg)
    {
        osalSysLock();
        // Check if bus is busy, if not then continue
        uint32_t flags = i2c->MSR;
        if (!(flags & LPI2C_MSR_BBF(1)))
            break;
        // Check if timed out, then return a timeout
        if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end))
        {
            msg = MSG_TIMEOUT;
        }
    }
    osalSysUnlock();

    // Check if bus is busy
    if (MSG_OK == msg && MSG_OK == (msg = MasterCheckForBusyBus(i2c)))
    {
        // Clear all status flags before starting the transfer
        MasterClearStatusFlags(i2c, kLPI2C_MasterClearFlags);

        // Turn off auto-stop
        i2c->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;

        // Transfer address if it should be transfered
        if (i2cp->send_address)
        {
            cmdbuff[cmdind++] = (0
                                | LPI2C_MTDR_CMD(i2c_gen_repeat_start_transm)
                                | (((i2cp->addr << 1) | i2c_transm) & 0xFF)
                                );
        }
        // Transfer register by its length
        if (i2cp->reglen > 0)
        {
            for (int si = 0; si < i2cp->reglen; si++)
            {
                cmdbuff[cmdind++] = (0
                                    | (((uint8_t*)i2cp->reg)[si] & 0xFF)
                                    );
            }
        }
        // Send another start if this is a read
        if (i2cp->send_address && i2cp->cmd == i2c_recv)
        {
            cmdbuff[cmdind++] = (0
                               | LPI2C_MTDR_CMD(i2c_gen_repeat_start_transm)
                               | ((i2cp->addr << 1) | i2c_recv)
                              );
        }

        if (i2c != (LPI2C_Type*)(0x403F8000))
            print("I2C Is not LPI2C3");
        msg = MasterWaitForTxReady(i2cp);

        // Send Command buffer
        for (int bi = 0; bi < cmdind; bi++)
        {
            if (MSG_OK == msg) //(msg = MasterWaitForTxReady(i2cp)))
            {
                i2c->MTDR = cmdbuff[bi];

            } else { break; }
        }

        if (MSG_OK == msg && 0)
        {
            // Send data if there is any
            if (i2cp->cmd == i2c_transm && i2cp->txlen > 0)
            {
                msg = MasterSend(i2cp);
            }

            // Receive data if there is any
            if (i2cp->cmd == i2c_recv && i2cp->rxlen > 0)
            {
                msg = MasterReceive(i2cp);
            }

            // Stop the transfer/receive
            if (MSG_OK == msg)
            {
                msg = MasterStop(i2cp);
            }
        }
    }
    return msg;
}
*/
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/
/**
 * @brief   I2C0 interrupt handler.
 *
 * @notapi
 */
#if (MIMXRT1062_I2C_USE_I2C1 == TRUE) || defined(__DOXYGEN__)
OSAL_IRQ_HANDLER(MIMXRT1062_I2C1_IRQ_VECTOR)
{
    OSAL_IRQ_PROLOGUE();

    i2c_lld_serve_interrupt(&I2CD1);

    OSAL_IRQ_EPILOGUE();
}
#endif
#if (MIMXRT1062_I2C_USE_I2C2 == TRUE) || defined(__DOXYGEN__)
OSAL_IRQ_HANDLER(MIMXRT1062_I2C2_IRQ_VECTOR)
{
    OSAL_IRQ_PROLOGUE();

    i2c_lld_serve_interrupt(&I2CD2);

    OSAL_IRQ_EPILOGUE();
}
#endif
#if (MIMXRT1062_I2C_USE_I2C3 == TRUE) || defined(__DOXYGEN__)
OSAL_IRQ_HANDLER(MIMXRT1062_I2C3_IRQ_VECTOR)
{
    OSAL_IRQ_PROLOGUE();

    i2c_lld_serve_interrupt(&I2CD3);

    OSAL_IRQ_EPILOGUE();
}
#endif
/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {
    print("I2C Initialized!\n");

CLOCK_SetMux(kCLOCK_Lpi2cMux, 0);
CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 5);
#if MIMXRT1062_I2C_USE_I2C1 == TRUE
  i2cObjectInit(&I2CD1);
  #if defined(LPI2C1)
  I2CD1.i2c = LPI2C1;
  #endif
  I2CD1.i2c_irq = LPI2C1_IRQn;
  I2CD1.i2c_irq_prio = MIMXRT1062_I2C_I2C1_PRIORITY;
  I2CD1.state = I2C_STOP;
  CLOCK_EnableClock(kCLOCK_Lpi2c1);
#endif
#if MIMXRT1062_I2C_USE_I2C2 == TRUE
  i2cObjectInit(&I2CD2);
  #if defined(LPI2C2)
  I2CD3.i2c = LPI2C2;
  #endif
  I2CD2.i2c_irq = LPI2C2_IRQn;
  I2CD2.i2c_irq_prio = MIMXRT1062_I2C_I2C2_PRIORITY;
  I2CD2.state = I2C_STOP;
  CLOCK_EnableClock(kCLOCK_Lpi2c2);
#endif
#if MIMXRT1062_I2C_USE_I2C3 == TRUE
  i2cObjectInit(&I2CD3);
  #if defined(LPI2C3)
  I2CD3.i2c = LPI2C3;
  #endif
  I2CD3.i2c_irq = LPI2C3_IRQn;
  I2CD3.i2c_irq_prio = MIMXRT1062_I2C_I2C3_PRIORITY;
  I2CD3.state = I2C_STOP;
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL, 1);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA, 1);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL, 0x888);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA, 0x888);
  CLOCK_EnableClock(kCLOCK_Lpi2c3);
#endif
}

static uint32_t I2CSrcFreq(void) {
    uint32_t freq = 0;
    if (CLOCK_GetMux(kCLOCK_Lpi2cMux) == 0)
    freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 8) / (CLOCK_GetDiv(kCLOCK_Lpi2cDiv) + 1);

    return freq;
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {
  LPI2C_Type * i2c = i2cp->i2c;


  if (i2cp->state == I2C_STOP) {
    LPI2C_MasterGetDefaultConfig(&i2cp->mconfig);
    LPI2C_MasterInit(i2c, &i2cp->mconfig, I2CSrcFreq());


    //LPI2C_MasterEnableInterrupts(i2c, i2cp->i2c_irq_prio);
    //nvicEnableVector(i2cp->i2c_irq, i2cp->i2c_irq_prio);
  }

}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp)
{
  LPI2C_Type * i2c = i2cp->i2c;
  if (i2cp->state != I2C_STOP)
  {
      nvicDisableVector(i2cp->i2c_irq);
      LPI2C_MasterDeinit(i2c);
  }
}

/**
 * @brief   Receives data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                     uint8_t *rxbuf, size_t rxbytes,
                                     sysinterval_t timeout)
{
    /*
    i2cp->cmd = i2c_recv;
    i2cp->addr = addr;
    i2cp->rxbuff = rxbuf;
    i2cp->rxlen = rxbytes;
    return MasterTransferBlocking(i2cp, timeout);*/
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = addr;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = i2cp->reg;
    xfer.subaddressSize = i2cp->reglen;
    xfer.data           = rxbuf;
    xfer.dataSize       = rxbytes;

    return kStatus_Success == LPI2C_MasterTransferBlocking(i2cp->i2c, &xfer) ? MSG_OK : MSG_TIMEOUT;
}

/**
 * @brief   Transmits data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes,
                                      sysinterval_t timeout)
{
    /*
    i2cp->cmd = i2c_transm;
    i2cp->addr = addr;
    i2cp->txbuff = txbuf;
    i2cp->txlen = txbytes;
    i2cp->rxbuff = rxbuf;
    i2cp->rxlen = rxbytes;
    return MasterTransferBlocking(i2cp, timeout);
    */
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = addr;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = i2cp->reg;
    xfer.subaddressSize = i2cp->reglen;
    xfer.data           = (uint8_t*)txbuf;
    xfer.dataSize       = txbytes;
    return kStatus_Success == LPI2C_MasterTransferBlocking(i2cp->i2c, &xfer) ? MSG_OK : MSG_TIMEOUT;
}

#endif /* HAL_USE_I2C == TRUE */

/** @} */
