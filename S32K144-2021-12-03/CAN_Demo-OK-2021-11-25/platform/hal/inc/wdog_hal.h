/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file wdog_hal.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro,
 * 'WDOG_UNLOCK', defined.
 * This macro is needed in order to unlock the WDOG prior to any update. The
 * reason for not using a function is the limited number of bus clocks in which
 * the WDOG can be reconfigured after an unlock sequence.
 *
 */

#ifndef WDOG_HAL_H
#define WDOG_HAL_H

#include "device_registers.h"
#include <stdbool.h>

/*!
 * @defgroup wdog_hal WDOG HAL
 * @ingroup wdog
 * @brief Watchdog Timer Hardware Abstraction Level.
 *
 * This HAL provides low-level access to all hardware features of the WDOG.
 * @addtogroup wdog_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*!
 * @brief Clock sources for the WDOG.
 * Implements : wdog_clk_source_t_Class
 */
typedef enum {
    WDOG_BUS_CLOCK                        = 0x00U, /*!< Bus clock */
    WDOG_LPO_CLOCK                        = 0x01U, /*!< LPO clock */
    WDOG_SOSC_CLOCK                       = 0x02U, /*!< SOSC clock */
    WDOG_SIRC_CLOCK                       = 0x03U  /*!< SIRC clock */
} wdog_clk_source_t;

/*!
 * @brief Test modes for the WDOG.
 * Implements : wdog_test_mode_t_Class
 */
typedef enum {
    WDOG_TST_DISABLED                     = 0x00U, /*!< Test mode disabled */
    WDOG_TST_USER                         = 0x01U, /*!< User mode enabled. (Test mode disabled.) */
    WDOG_TST_LOW                          = 0x02U, /*!< Test mode enabled, only the low byte is used. */
    WDOG_TST_HIGH                         = 0x03U  /*!< Test mode enabled, only the high byte is used. */
} wdog_test_mode_t;

/*!
 * @brief WDOG configuration structure
 * Implements : wdog_op_mode_t_Class
 */
typedef struct {
    bool                 wait;  /*!< Wait mode */
    bool                 stop;  /*!< Stop mode */
    bool                 debug; /*!< Debug mode */
} wdog_op_mode_t;

/*!
 * @brief WDOG configuration structure
 * Implements : wdog_user_config_t_Class
 */
typedef struct {
    wdog_clk_source_t    clkSource;       /*!< The clock source of the WDOG */
    wdog_op_mode_t       opMode;          /*!< The modes in which the WDOG is functional */
    bool                 updateEnable;    /*!< If true, further updates of the WDOG are enabled */
    bool                 intEnable;       /*!< If true, an interrupt request is generated before reset */
    bool                 winEnable;       /*!< If true, window mode is enabled */
    uint32_t             windowValue;     /*!< The window value */
    uint32_t             timeoutValue;    /*!< The timeout value */
    bool                 prescalerEnable; /*!< If true, a fixed 256 prescaling of the counter reference clock is enabled */
} wdog_user_config_t;


/* The dummy read is used in order to make sure that any write to the
 * WDOG registers will be started only after the write of the unlock value was
 * completed.
 */
#define WDOG_UNLOCK32(base)  \
    { \
        (base)->CNT = FEATURE_WDOG_UNLOCK_VALUE; \
        (void)(base)->CNT; \
    }
#define WDOG_UNLOCK16(base)  \
    { \
        (base)->CNT = FEATURE_WDOG_UNLOCK16_FIRST_VALUE; \
        (void)(base)->CNT; \
        (base)->CNT = FEATURE_WDOG_UNLOCK16_SECOND_VALUE; \
        (void)(base)->CNT; \
    }
#define WDOG_UNLOCK(base)    \
    { \
        if (((base)->CS & WDOG_CS_CMD32EN_MASK) != 0U) \
        { \
            WDOG_UNLOCK32(base); \
        } \
        else \
        { \
            WDOG_UNLOCK16(base); \
        } \
    }

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name WDOG Common Configurations
 * @{
 */

/*!
 * @brief Initializes the WDOG.
 *
 * This function initializes the WDOG to known state.
 *
 *
 * @param[in] base WDOG base pointer.
 */
void WDOG_HAL_Init(WDOG_Type *base);

/*!
 * @brief Verifies if the WDOG is enabled.
 *
 * This function verifies the state of the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the state of the WDOG:
 *         - false: WDOG is disabled
 *         - true: WDOG is enabled
 * Implements : WDOG_HAL_IsEnabled_Activity
 */
static inline bool WDOG_HAL_IsEnabled(const WDOG_Type *base)
{
    return (((base->CS & WDOG_CS_EN_MASK) >> WDOG_CS_EN_SHIFT) != 0U);
}

/*!
 * @brief Enables the WDOG.
 *
 * This function enables the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * Implements : WDOG_HAL_Enable_Activity
 */
static inline void WDOG_HAL_Enable(WDOG_Type *base)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_EN_MASK) | WDOG_CS_EN(1U);
}

/*!
 * @brief Disables the WDOG.
 *
 * This function diables the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * Implements : WDOG_HAL_Disable_Activity
 */
static inline void WDOG_HAL_Disable(WDOG_Type *base)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_EN_MASK) | WDOG_CS_EN(0U);
}

/*!
 * @brief Refreshes the WDOG counter
 *
 * @param[in] base WDOG base pointer.
 * Implements : WDOG_HAL_Trigger_Activity
 */
static inline void WDOG_HAL_Trigger(WDOG_Type *base)
{
    if (((base)->CS & WDOG_CS_CMD32EN_MASK) != 0U)
    {
        base->CNT = FEATURE_WDOG_TRIGGER_VALUE;
    }
    else
    {
        base->CNT = FEATURE_WDOG_TRIGGER16_FIRST_VALUE;
        (void)base->CNT;
        base->CNT = FEATURE_WDOG_TRIGGER16_SECOND_VALUE;
    }
}

/*!
 * @brief Configures all WDOG registers.
 *
 * This function configures all WDOG registers.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] config the new configuration to be used.
 */
void WDOG_HAL_Config(WDOG_Type *base, const wdog_user_config_t *config);

/*!
 * @brief Gets the current WDOG configuration.
 *
 * This function gets the current WDOG configuration
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the current WDOG configuration
 */
wdog_user_config_t WDOG_HAL_GetConfig(const WDOG_Type *base);

/*!
 * @brief Enables/Disables window mode.
 *
 * This function enables/disables window mode for the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable window mode
 * Implements : WDOG_HAL_SetWindow_Activity
 */
static inline void WDOG_HAL_SetWindow(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_WIN_MASK) | WDOG_CS_WIN(enable? 1UL : 0UL);
}

/*!
 * @brief Gets Interrupt Flag (FLG) status.
 *
 * This function verifies if an interrupt occurred.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the state of the Interrupt Flag:
 *         - false: no interrupt occurred
 *         - true: an interrupt occurred
 * Implements : WDOG_HAL_GetInt_Activity
 */
static inline bool WDOG_HAL_GetInt(const WDOG_Type *base)
{
    return (((base->CS & WDOG_CS_FLG_MASK) >> WDOG_CS_FLG_SHIFT) != 0U);
}

/*!
 * @brief Clears the Interrupt Flag.
 *
 * This function clears the Interrupt Flag (FLG).
 *
 *
 * @param[in] base WDOG base pointer.
 * Implements : WDOG_HAL_ClearInt_Activity
 */
static inline void WDOG_HAL_ClearInt(WDOG_Type *base)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_FLG_MASK) | WDOG_CS_FLG(1U);
}

/*!
 * @brief Enables/Disables prescaler.
 *
 * This function enables/disables the prescaler for the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable prescaler.
 * Implements : WDOG_HAL_SetPrescaler_Activity
 */
static inline void WDOG_HAL_SetPrescaler(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_PRES_MASK) | WDOG_CS_PRES(enable? 1UL : 0UL);
}

/*!
 * @brief Selects the clock source used by the WDOG.
 *
 * This function selects the clock source used by the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] clkSource the clock source to be used by the WDOG:
 *         - 0x00U: Bus clock
 *         - 0x01U: LPO clock
 *         - 0x02U: System oscillator clock (SOSC, from SCG)
 *         - 0x03U: Slow internal reference clock (SIRC, from SCG)
 * Implements : WDOG_HAL_SetClockSource_Activity
 */
static inline void WDOG_HAL_SetClockSource(WDOG_Type *base, wdog_clk_source_t clkSource)
{
    uint32_t regValue;
    regValue = base->CS;
    regValue &= ~(WDOG_CS_CLK_MASK);
    regValue |= WDOG_CS_CLK(clkSource);

    WDOG_UNLOCK(base);

    base->CS = regValue;
}

/*!
 * @brief Enables/Disables WDOG interrupt.
 *
 * This function enables/disables the interrupts from the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable interrupts.
 * Implements : WDOG_HAL_SetInt_Activity
 */
static inline void WDOG_HAL_SetInt(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_INT_MASK) | WDOG_CS_INT(enable? 1UL : 0UL);
}

/*!
 * @brief Verifies if the WDOG updates are allowed.
 *
 * This function verifies if software is allowed to reconfigure the WDOG without
 * a reset.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the state of the WDOG updates:
 *         - false: updates are not allowed
 *         - true: updates are allowed
 * Implements : WDOG_HAL_IsUpdateEnabled_Activity
 */
static inline bool WDOG_HAL_IsUpdateEnabled(const WDOG_Type *base)
{
    return (((base->CS & WDOG_CS_UPDATE_MASK) >> WDOG_CS_UPDATE_SHIFT) != 0U);
}

/*!
 * @brief Enables/Disables WDOG updates.
 *
 * This function enables/disables the possibility to update the WDOG config.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable updates.
 * Implements : WDOG_HAL_SetUpdate_Activity
 */
static inline void WDOG_HAL_SetUpdate(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_UPDATE_MASK) | WDOG_CS_UPDATE(enable? 1UL : 0UL);
}

/*!
 * @brief Gets the WDOG test mode.
 *
 * This function verifies the test mode of the WDOG.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the test mode:
 *         - 0x00U: Test mode disabled
 *         - 0x01U: User mode enabled (test mode disabled)
 *         - 0x02U: Test mode enabled (for the low byte)
 *         - 0x03U: Test mode enabled (for the high byte)
 * Implements : WDOG_HAL_GetTestMode_Activity
 */
static inline wdog_test_mode_t WDOG_HAL_GetTestMode(const WDOG_Type *base)
{
    uint32_t tmp = ((base->CS & WDOG_CS_TST_MASK) >> WDOG_CS_TST_SHIFT);
    wdog_test_mode_t testMode;

    switch (tmp)
    {
    case 0U:
        testMode = WDOG_TST_DISABLED;
        break;
    case 1U:
        testMode = WDOG_TST_USER;
        break;
    case 2U:
        testMode = WDOG_TST_LOW;
        break;
    case 3U:
        testMode = WDOG_TST_HIGH;
        break;
    default:
        testMode = WDOG_TST_DISABLED;
        break;
    }

    return testMode;
}

/*!
 * @brief Changes the WDOG test mode.
 *
 * This function changes the test mode of the WDOG. If the WDOG is tested in
 * mode, software should set this field to 0x01U in order to indicate that the
 * WDOG is functioning normally.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param testMode the test mode:
 *         - 0x00U: Test mode disabled
 *         - 0x01U: User mode enabled (test mode disabled)
 *         - 0x02U: Test mode enabled (for the low byte)
 *         - 0x03U: Test mode enabled (for the high byte)
 * Implements : WDOG_HAL_SetTestMode_Activity
 */
static inline void WDOG_HAL_SetTestMode(WDOG_Type *base, wdog_test_mode_t testMode)
{
    uint32_t regValue;
    regValue = base->CS;
    regValue &= ~(WDOG_CS_TST_MASK);
    regValue |= WDOG_CS_TST(testMode);

    WDOG_UNLOCK(base);

    base->CS = regValue;
}

/*!
 * @brief Enables/Disables WDOG in debug mode.
 *
 * This function enables/disables the WDOG in debug mode.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable WDOG in debug mode
 * Implements : WDOG_HAL_SetDebug_Activity
 */
static inline void WDOG_HAL_SetDebug(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_DBG_MASK) | WDOG_CS_DBG(enable? 1UL : 0UL);
}

/*!
 * @brief Enables/Disables WDOG in wait mode.
 *
 * This function enables/disables the WDOG in wait mode.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable WDOG in wait mode.
 * Implements : WDOG_HAL_SetWait_Activity
 */
static inline void WDOG_HAL_SetWait(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_WAIT_MASK) | WDOG_CS_WAIT(enable? 1UL : 0UL);
}

/*!
 * @brief Enables/Disables WDOG in stop mode.
 *
 * This function enables/disables the WDOG in stop mode.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable WDOG in stop mode.
 * Implements : WDOG_HAL_SetStop_Activity
 */
static inline void WDOG_HAL_SetStop(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_STOP_MASK) | WDOG_CS_STOP(enable? 1UL : 0UL);
}

/*!
 * @brief Gets the value of the WDOG counter.
 *
 * This function gets the value of the WDOG counter.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the value of the WDOG counter.
 * Implements : WDOG_HAL_GetCounter_Activity
 */
static inline uint16_t WDOG_HAL_GetCounter(const WDOG_Type *base)
{
    return (uint16_t) base->CNT;
}

/*!
 * @brief Sets the value of the WDOG counter.
 *
 * This sets the value of the WDOG counter.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] counter the value of the WDOG counter.
 * Implements : WDOG_HAL_SetCounter_Activity
 */
static inline void WDOG_HAL_SetCounter(WDOG_Type *base, uint32_t counter)
{
    base->CNT = counter;
}

/*!
 * @brief Gets the value of the WDOG timeout.
 *
 * This function gets the value of the WDOG timeout.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the value of the WDOG timeout.
 * Implements : WDOG_HAL_GetTimeout_Activity
 */
static inline uint16_t WDOG_HAL_GetTimeout(const WDOG_Type *base)
{
    return (uint16_t) base->TOVAL;
}

/*!
 * @brief Sets the value of the WDOG timeout.
 *
 * This sets the value of the WDOG timeout.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] timeout the value of the WDOG timeout.
 * Implements : WDOG_HAL_SetTimeout_Activity
 */
static inline void WDOG_HAL_SetTimeout(WDOG_Type *base, uint16_t timeout)
{
    WDOG_UNLOCK(base);

    base->TOVAL = (uint32_t) timeout;
}

/*!
 * @brief Sets the value of the WDOG window.
 *
 * This sets the value of the WDOG window.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] window the value of the WDOG window.
 * Implements : WDOG_HAL_SetWindowValue_Activity
 */
static inline void WDOG_HAL_SetWindowValue(WDOG_Type *base, uint16_t window)
{
    WDOG_UNLOCK(base);

    base->WIN = (uint32_t) window;
}

/*!
 * @brief Checks if the WDOG is unlocked.
 *
 * This function checks if the WDOG is unlocked. If the module is unlocked,
 * reconfiguration of the registers is allowed.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return true if the module is unlocked, false if the module is locked.
 * Implements : WDOG_HAL_IsUnlocked_Activity
 */
static inline bool WDOG_HAL_IsUnlocked(const WDOG_Type *base)
{
    return (((base->CS & WDOG_CS_ULK_MASK) >> WDOG_CS_ULK_SHIFT) != 0U);
}

/*!
 * @brief Checks if the last configuration of the WDOG was successful.
 *
 * This function checks if the last configuration of the WDOG was successful.
 *
 *
 * @param[in] base WDOG base pointer.
 * @return true if the configuration was successful, false if not.
 * Implements : WDOG_HAL_IsReconfigurationComplete_Activity
 */
static inline bool WDOG_HAL_IsReconfigurationComplete(const WDOG_Type *base)
{
    return (((base->CS & WDOG_CS_RCS_MASK) >> WDOG_CS_RCS_SHIFT) != 0U);
}

/*!
 * @brief Enables/Disables support for 32-bit refresh/unlock command write words.
 *
 * This function enables/disables support for 32-bit refresh/unlock command write words.
 *
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable 32-bit refresh/unlock command write words.
 * Implements : WDOG_HAL_SetCmd32_Activity
 */
static inline void WDOG_HAL_SetCmd32(WDOG_Type *base, bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_CMD32EN_MASK) | WDOG_CS_CMD32EN(enable? 1UL : 0UL);

    WDOG_UNLOCK(base);
}
/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* WDOG_HAL_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/

