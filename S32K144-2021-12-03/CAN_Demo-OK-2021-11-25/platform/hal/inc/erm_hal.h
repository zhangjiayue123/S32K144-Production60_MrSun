/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

#ifndef ERM_HAL_H
#define ERM_HAL_H

/*! @file erm_hal.h */

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"

/*!
 * @defgroup erm_hal ERM HAL
 * @ingroup erm
 * @details This section describes the programming interface of the ERM HAL.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The distance between channels */
#define ERM_CHANNELS_OFFSET_SIZE (4U)
/*! @brief Start bit of non correctable error */
#define ERM_NCE_START            (30U)
/*! @brief Start bit of single bit correction */
#define ERM_SBC_START            (31U)

/*!
 * @brief ERM types of ECC events
 * Implements : erm_ecc_event_t_Class
 */
typedef enum
{
    ERM_EVENT_NONE              = 0U,   /*!< None events */
    ERM_EVENT_SINGLE_BIT        = 1U,   /*!< Single-bit correction ECC events */
    ERM_EVENT_NON_CORRECTABLE   = 2U    /*!< Non-correctable ECC events */
} erm_ecc_event_t;


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name ERM HAL API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the ERM module
 *
 * This function initializes the module to default configuration,
 * the configuration register is initialized with interrupt notification disabled
 * for all channels and the status register events are cleared
 *
 * @param[in] base The ERM peripheral base address
 */
void ERM_HAL_Init(ERM_Type * const base);

/*!
 * @brief Enables Memory n interrupt event
 *
 * This function enables Memory n interrupt event
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The configured memory channel
 * @param[in] eccEvent The configured event
 * @param[in] enable Enable interrupt event
 *            - true: Interrupt event is enabled
 *            - false: Interrupt event is disabled
 * Implements : ERM_HAL_EnableEventInterrupt_Activity
 */
static inline void ERM_HAL_EnableEventInterrupt(ERM_Type * const base,
                                                uint8_t channel,
                                                erm_ecc_event_t eccEvent,
                                                bool enable)
{
    DEV_ASSERT(channel < ERM_EARn_COUNT);
    uint32_t tempCtrl = base->CR0;

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        if (enable)
        {
            tempCtrl |= 1UL << (ERM_SBC_START - (channel * ERM_CHANNELS_OFFSET_SIZE));
        }
        else
        {
            tempCtrl &= ~(1UL << (ERM_SBC_START - (channel * ERM_CHANNELS_OFFSET_SIZE)));
        }
    }
    else
    {
        /* Non-correctable */
        if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
        {
            if (enable)
            {
                tempCtrl |= 1UL << (ERM_NCE_START - (channel * ERM_CHANNELS_OFFSET_SIZE));
            }
            else
            {
                tempCtrl &= ~(1UL << (ERM_NCE_START - (channel * ERM_CHANNELS_OFFSET_SIZE)));
            }
        }
    }

    /* Write to register */
    base->CR0 = tempCtrl;
}

/*!
 * @brief Checks if the Memory n interrupt event is enabled
 *
 * This function checks if the Memory n interrupt event is enabled
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The examined memory channel
 * @param[in] eccEvent The examined event
 * @return Interrupt event
 *         - true: Interrupt event is enabled
 *         - false: Interrupt event is disabled
 * Implements : ERM_HAL_IsEventInterruptEnabled_Activity
 */
static inline bool ERM_HAL_IsEventInterruptEnabled(const ERM_Type * const base,
                                                   uint8_t channel,
                                                   erm_ecc_event_t eccEvent)
{
    DEV_ASSERT(channel < ERM_EARn_COUNT);
    bool retVal = false;

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        retVal = (((base->CR0 >> (ERM_SBC_START - (channel * ERM_CHANNELS_OFFSET_SIZE))) & 1U) != 0U);
    }
    else
    {
        /* Non-correctable */
        if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
        {
            retVal = (((base->CR0 >> (ERM_NCE_START - (channel * ERM_CHANNELS_OFFSET_SIZE))) & 1U) != 0U);
        }
    }

    return retVal;
}

/*!
 * @brief Checks if the Memory n error event is detected
 *
 * This function checks if the Memory n error event is detected
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The examined memory channel
 * @param[in] eccEvent The examined event
 * @return The status of Memory n error event
 *         - true: Error event on Memory n detected
 *         - false: No error event on Memory n detected
 * Implements : ERM_HAL_IsEventDetected_Activity
 */
static inline bool ERM_HAL_IsEventDetected(const ERM_Type * const base,
                                           uint8_t channel,
                                           erm_ecc_event_t eccEvent)
{
    DEV_ASSERT(channel < ERM_EARn_COUNT);
    uint32_t retVal = 0U;

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        retVal = base->SR0 >> (ERM_SBC_START - (channel * ERM_CHANNELS_OFFSET_SIZE));
    }
    else
    {
        /* Non-correctable */
        if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
        {
            retVal = base->SR0 >> (ERM_NCE_START - (channel * ERM_CHANNELS_OFFSET_SIZE));
        }
    }

    return (retVal & 1U) != 0U;
}

/*!
 * @brief Clears error event and the corresponding interrupt notification
 *
 * This function clears error event and the corresponding interrupt notification
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The configured memory channel
 * @param[in] eccEvent The examined event
 * Implements : ERM_HAL_ClearEvent_Activity
 */
static inline void ERM_HAL_ClearEvent(ERM_Type * const base,
                                      uint8_t channel,
                                      erm_ecc_event_t eccEvent)
{
    DEV_ASSERT(channel < ERM_EARn_COUNT);

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        base->SR0 = 1UL << (ERM_SBC_START - (channel * ERM_CHANNELS_OFFSET_SIZE));
    }
    else
    {
        /* Non-correctable */
        if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
        {
            base->SR0 = 1UL << (ERM_NCE_START - (channel * ERM_CHANNELS_OFFSET_SIZE));
        }
    }

#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    (void)base->SR0;
#endif
}

/*!
 * @brief Gets the address of the last ECC event in Memory n
 *
 * This function gets the address of the last ECC event in Memory n
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The examined memory channel
 * @return Address of the last ECC event
 * Implements : ERM_HAL_GetLastErrorAddress_Activity
 */
static inline uint32_t ERM_HAL_GetLastErrorAddress(const ERM_Type * const base,
                                                   uint8_t channel)
{
    DEV_ASSERT(channel < ERM_EARn_COUNT);

    return base->EARn[channel].EAR;
}

/*!
 * @brief Gets the address of the last ECC event in Memory n and ECC event
 *
 * This function gets the address of the last ECC event in Memory n and ECC event
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The examined memory channel
 * @param[out] addressPtr The pointer to address of the last ECC event in Memory n with ECC event
 * @return The last occurred ECC event
 */
erm_ecc_event_t ERM_HAL_GetErrorDetail(const ERM_Type * const base,
                                       uint8_t channel,
                                       uint32_t * addressPtr);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* ERM_HAL_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
