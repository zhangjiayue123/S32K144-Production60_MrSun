/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#ifndef LPTMR_DRIVER_H
#define LPTMR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "device_registers.h"
#include "status.h"
#include "lptmr_hal.h"

/*! @file lptmr_driver.h*/

/*!
 * @addtogroup lptmr_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the LPTMR counter units available for configuring or reading the timer compare value.
 *
 * Implements : lptmr_counter_units_t_Class
 */
typedef enum
{
    LPTMR_COUNTER_UNITS_TICKS           = 0x00U,
    LPTMR_COUNTER_UNITS_MICROSECONDS    = 0x01U
} lptmr_counter_units_t;


/*!
 * @brief Defines the configuration structure for LPTMR.
 *
 * Implements : lptmr_config_t_Class
 */
typedef struct
{
    /* General parameters */
    bool dmaRequest;                    /*!< Enable/Disable DMA requests */
    bool interruptEnable;               /*!< Enable/Disable Interrupt */
    bool freeRun;                       /*!< Enable/Disable Free Running Mode */
    lptmr_workmode_t workMode;          /*!< Time/Pulse Counter Mode */
    /* Counter parameters */
    lptmr_clocksource_t clockSelect;    /*!< Clock selection for Timer/Glitch filter */
    lptmr_prescaler_t prescaler;        /*!< Prescaler Selection */
    bool bypassPrescaler;               /*!< Enable/Disable prescaler bypass */
    uint32_t compareValue;              /*!< Compare value */
    lptmr_counter_units_t counterUnits; /*!< Compare value units */
    /* Pulse Counter specific parameters */
    lptmr_pinselect_t pinSelect;        /*!< Pin selection for Pulse-Counter */
    lptmr_pinpolarity_t pinPolarity;    /*!< Pin Polarity for Pulse-Counter */
} lptmr_config_t;


/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
/*!
 * @name LPTMR Driver Functions
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initialize a configuration structure with default values.
 *
 * @param[out] config - pointer to the configuration structure to be initialized
 */
void LPTMR_DRV_InitConfigStruct(lptmr_config_t * const config);


/*!
 * @brief Initialize a LPTMR instance with values from an input configuration structure.
 *
 * When (counterUnits == LPTMR_COUNTER_UNITS_MICROSECONDS) the function will
 * automatically configure the timer for the input compareValue in microseconds.
 * The input params for 'prescaler' and 'bypassPrescaler' will be ignored - their values
 * will be adapted by the function, to best fit the input compareValue
 * (in microseconds) for the operating clock frequency.
 *
 * LPTMR_COUNTER_UNITS_MICROSECONDS may only be used for LPTMR_WORKMODE_TIMER mode.
 * Otherwise the function shall not convert 'compareValue' in ticks
 * and this is likely to cause erroneous behavior.
 *
 * When (counterUnits == LPTMR_COUNTER_UNITS_TICKS) the function will use the
 * 'prescaler' and 'bypassPrescaler' provided in the input config structure.
 *
 * @param[in] instance      - LPTMR instance number
 * @param[in] config        - pointer to the input configuration structure
 * @param[in] startCounter  - flag for starting the counter immediately after configuration
 */
void LPTMR_DRV_Init(const uint32_t instance,
                    const lptmr_config_t * const config,
                    const bool startCounter);


/*!
 * @brief Configure a LPTMR instance.
 *
 * When (counterUnits == LPTMR_COUNTER_UNITS_MICROSECONDS) the function will
 * automatically configure the timer for the input compareValue in microseconds.
 * The input params for 'prescaler' and 'bypassPrescaler' will be ignored - their values
 * will be adapted by the function, to best fit the input compareValue
 * (in microseconds) for the operating clock frequency.
 *
 * LPTMR_COUNTER_UNITS_MICROSECONDS may only be used for LPTMR_WORKMODE_TIMER mode.
 * Otherwise the function shall not convert 'compareValue' in ticks
 * and this is likely to cause erroneous behavior.
 *
 * When (counterUnits == LPTMR_COUNTER_UNITS_TICKS) the function will use the
 * 'prescaler' and 'bypassPrescaler' provided in the input config structure.
 *
 * @param[in] instance  - LPTMR instance number
 * @param[in] config    - pointer to the input configuration structure
 */
void LPTMR_DRV_SetConfig(const uint32_t instance,
                         const lptmr_config_t * const config);


/*!
 * @brief Get the current configuration of a LPTMR instance.
 *
 * @param[in] instance  - LPTMR instance number
 * @param[out] config   - pointer to the output configuration structure
 */
void LPTMR_DRV_GetConfig(const uint32_t instance,
                         lptmr_config_t * const config);


/*!
 * @brief De-initialize a LPTMR instance
 *
 * @param[in] instance  - LPTMR instance number
 */
void LPTMR_DRV_Deinit(const uint32_t instance);


/*!
 * @brief Set the compare value in counter tick units, for a LPTMR instance.
 *
 * @param[in] instance            - LPTMR instance number
 * @param[in] compareValueByCount - the compare value in counter ticks, to be written
 * @return  One of the possible status codes:
 * - STATUS_SUCCESS: completed successfully
 * - STATUS_ERROR: cannot reconfigure compare value (TCF not set)
 * - STATUS_TIMEOUT: compare value greater then current counter value
 */
status_t LPTMR_DRV_SetCompareValueByCount(const uint32_t instance,
                                          const uint16_t compareValueByCount);


/*!
 * @brief Get the compare value in counter tick units, of a LPTMR instance.
 *
 * @param[in] instance              - LPTMR instance number
 * @param[out] compareValueByCount  - pointer to current compare value, in counter ticks
 */
void LPTMR_DRV_GetCompareValueByCount(const uint32_t instance,
                                      uint16_t * const compareValueByCount);


/*!
 * @brief Set the compare value for Timer Mode in microseconds, for a LPTMR instance.
 *
 * @param[in] instance       - LPTMR peripheral instance number
 * @param[in] compareValueUs - compare value in microseconds
 * @return  One of the possible status codes:
 * - STATUS_SUCCESS: completed successfully
 * - STATUS_ERROR: cannot reconfigure compare value
 * - STATUS_TIMEOUT: compare value greater then current counter value
 */
status_t LPTMR_DRV_SetCompareValueByUs(const uint32_t instance,
                                       const uint32_t compareValueUs);


/*!
 * @brief Get the compare value in microseconds, of a LPTMR instance.
 *
 * @param[in] instance         - LPTMR instance number
 * @param[out] compareValueUs  - pointer to current compare value, in microseconds
 */
void LPTMR_DRV_GetCompareValueByUs(const uint32_t instance,
                                   uint32_t * const compareValueUs);


/*!
 * @brief Get the current state of the Compare Flag of a LPTMR instance.
 *
 * @param[in] instance - LPTMR instance number
 * @return  the state of the Compare Flag
 */
bool LPTMR_DRV_GetCompareFlag(const uint32_t instance);


/*!
 * @brief Clear the Compare Flag of a LPTMR instance.
 *
 * @param[in] instance - LPTMR instance number
 */
void LPTMR_DRV_ClearCompareFlag(const uint32_t instance);


/*!
 * @brief Get the run state of a LPTMR instance.
 *
 * @param[in] instance - LPTMR instance number
 * @return the run state of the LPTMR instance:
 *  - true: Timer/Counter started
 *  - false: Timer/Counter stopped
 */
bool LPTMR_DRV_IsRunning(const uint32_t instance);


/*!
 * @brief Enable/disable the LPTMR interrupt.
 *
 * @param[in] instance          - LPTMR instance number
 * @param[in] enableInterrupt   - the new state of the LPTMR interrupt enable flag.
 */
void LPTMR_DRV_SetInterrupt(const uint32_t instance,
                            const bool enableInterrupt);


/*!
 * @brief Get the current counter value in counter tick units.
 *
 * @param[in] instance - LPTMR instance number
 * @return  the current counter value
 */
uint16_t LPTMR_DRV_GetCounterValueByCount(const uint32_t instance);


/*!
 * @brief Enable the LPTMR / Start the counter
 *
 * @param[in] instance - LPTMR instance number
 */
void LPTMR_DRV_StartCounter(const uint32_t instance);


/*!
 * @brief Disable the LPTMR / Stop the counter
 *
 * @param[in] instance - LPTMR instance number
 */
void LPTMR_DRV_StopCounter(const uint32_t instance);


/*!
 * @brief Set the Input Pin configuration for Pulse Counter mode
 *
 * @param[in] instance      - LPTMR instance number
 * @param[in] pinSelect     - LPTMR pin selection
 * @param[in] pinPolarity   - polarity on which to increment counter (rising/falling edge)
 */
void LPTMR_DRV_SetPinConfiguration(const uint32_t instance,
                                   const lptmr_pinselect_t pinSelect,
                                   const lptmr_pinpolarity_t pinPolarity);


#if defined(__cplusplus)
}
#endif

/*! @}*/

/*! @}*/

#endif /* LPTMR_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
