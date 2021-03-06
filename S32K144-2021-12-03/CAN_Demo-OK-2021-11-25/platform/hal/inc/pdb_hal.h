/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#ifndef PDB_HAL_H
#define PDB_HAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"

 /**
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type
* and a pointer to a different object type
* This is needed for retrieving 16 bits out of a 32 bit register
*
* @section [global]
* Violates MISRA 2012 Required Rule 1.3, Unusual pointer cast (incompatible indirect types)
* This is needed for retrieving 16 bits out of a 32 bit register
*
* @section [global]
* Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different
* essential type
* This is needed for retrieving 16 bits out of a 32 bit register
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 18.4, Pointer arithmetic other than array indexing used
* This is needed for moving with a 16 bit step through a 32 bit register
*
*/

/*! @file */

/*!
 * @addtogroup pdb_hal
 * @{
 */

/******************************************************************************
 * Definitions
 *****************************************************************************/

/*!
 * @brief Defines the type of value load mode for the PDB module.
 *
 * Some timing related registers, such as the MOD, IDLY, CHnDLYm, INTx and POyDLY,
 * buffer the setting values. Only the load operation is triggered.
 * The setting value is loaded from a buffer and takes effect. There are
 * four loading modes to fit different applications.
 * Implements : pdb_load_value_mode_t_Class
 */
typedef enum
{
    PDB_LOAD_VAL_IMMEDIATELY                        = 0U,
        /*!<  Loaded immediately after load operation. @internal gui name="Immediately" */
    PDB_LOAD_VAL_AT_MODULO_COUNTER                  = 1U,
        /*!< Loaded when counter hits the modulo after load operation. @internal gui name="Modulo counter" */
    PDB_LOAD_VAL_AT_NEXT_TRIGGER                    = 2U,
        /*!< Loaded when detecting an input trigger after load operation. @internal gui name="Next trigger" */
    PDB_LOAD_VAL_AT_MODULO_COUNTER_OR_NEXT_TRIGGER  = 3U
        /*!< Loaded when counter hits the modulo or detecting an input trigger after load operation. @internal gui name="Modulo counter/Next trigger" */
} pdb_load_value_mode_t;

/*!
 * @brief Defines the type of prescaler divider for the PDB counter clock.
  * Implements : pdb_clk_prescaler_div_t_Class
 */
typedef enum
{
    PDB_CLK_PREDIV_BY_1   = 0U, /*!< Counting divided by multiplication factor selected by MULT. @internal gui name="1" */
    PDB_CLK_PREDIV_BY_2   = 1U, /*!< Counting divided by multiplication factor selected by 2 times ofMULT. @internal gui name="2" */
    PDB_CLK_PREDIV_BY_4   = 2U, /*!< Counting divided by multiplication factor selected by 4 times ofMULT. @internal gui name="4" */
    PDB_CLK_PREDIV_BY_8   = 3U, /*!< Counting divided by multiplication factor selected by 8 times ofMULT. @internal gui name="8" */
    PDB_CLK_PREDIV_BY_16  = 4U, /*!< Counting divided by multiplication factor selected by 16 times ofMULT. @internal gui name="16" */
    PDB_CLK_PREDIV_BY_32  = 5U, /*!< Counting divided by multiplication factor selected by 32 times ofMULT. @internal gui name="32" */
    PDB_CLK_PREDIV_BY_64  = 6U, /*!< Counting divided by multiplication factor selected by 64 times ofMULT. @internal gui name="64" */
    PDB_CLK_PREDIV_BY_128 = 7U /*!< Counting divided by multiplication factor selected by 128 times ofMULT. @internal gui name="128" */
} pdb_clk_prescaler_div_t;

/*!
 * @brief Defines the type of trigger source mode for the PDB.
 *
 * Selects the trigger input source for the PDB. The trigger input source can
 * be internal or external (EXTRG pin), or the software trigger.
  * Implements : pdb_trigger_src_t_Class
 */
typedef enum
{
    PDB_TRIGGER_0           = 0U,     /*!< Select trigger-In 0. @internal gui name="External trigger" */
    PDB_TRIGGER_1           = 1U,     /*!< Select trigger-In 1. @internal gui name="Trigger 1" */
    PDB_TRIGGER_2           = 2U,     /*!< Select trigger-In 2. @internal gui name="Trigger 2" */
    PDB_TRIGGER_3           = 3U,     /*!< Select trigger-In 3. @internal gui name="Trigger 3" */
    PDB_TRIGGER_4           = 4U,     /*!< Select trigger-In 4. @internal gui name="Trigger 4" */
    PDB_TRIGGER_5           = 5U,     /*!< Select trigger-In 5. @internal gui name="Trigger 5" */
    PDB_TRIGGER_6           = 6U,     /*!< Select trigger-In 6. @internal gui name="Trigger 6" */
    PDB_TRIGGER_7           = 7U,     /*!< Select trigger-In 7. @internal gui name="Trigger 7" */
    PDB_TRIGGER_8           = 8U,     /*!< Select trigger-In 8. @internal gui name="Trigger 8" */
    PDB_TRIGGER_9           = 9U,     /*!< Select trigger-In 8. @internal gui name="Trigger 9" */
    PDB_TRIGGER_10          = 10U,    /*!< Select trigger-In 10. @internal gui name="Trigger 10" */
    PDB_TRIGGER_11          = 11U,    /*!< Select trigger-In 11. @internal gui name="Trigger 11" */
    PDB_TRIGGER_12          = 12U,    /*!< Select trigger-In 12. @internal gui name="Trigger 12" */
    PDB_TRIGGER_13          = 13U,    /*!< Select trigger-In 13. @internal gui name="Trigger 13" */
    PDB_TRIGGER_14          = 14U,    /*!< Select trigger-In 14. @internal gui name="Trigger 14" */
    PDB_SOFTWARE_TRIGGER    = 15U    /*!< Select software trigger. @internal gui name="Software trigger" */
} pdb_trigger_src_t;

/*!
 * @brief Defines the type of the multiplication source mode for PDB.
 *
 * Selects the multiplication factor of the prescaler divider for the PDB counter clock.
 * Implements : pdb_clk_prescaler_mult_factor_t_Class
 */
typedef enum
{
    PDB_CLK_PREMULT_FACT_AS_1  = 0U, /*!< Multiplication factor is 1. @internal gui name="1" */
    PDB_CLK_PREMULT_FACT_AS_10 = 1U, /*!< Multiplication factor is 10. @internal gui name="10" */
    PDB_CLK_PREMULT_FACT_AS_20 = 2U, /*!< Multiplication factor is 20. @internal gui name="20" */
    PDB_CLK_PREMULT_FACT_AS_40 = 3U  /*!< Multiplication factor is 40. @internal gui name="40" */
} pdb_clk_prescaler_mult_factor_t;

/*!
 * @brief Defines the type of structure for basic timer in PDB.
 *
 * @internal gui name="Basic configuration" id="pdbCfg"
 * Implements : pdb_timer_config_t_Class
 */
typedef struct
{
    pdb_load_value_mode_t           loadValueMode;          /*!< Select the load mode. @internal gui name="Load mode" id="LoadMode" */
    bool                            seqErrIntEnable;        /*!< Enable PDB Sequence Error Interrupt. @internal gui name="Sequence error interrupt" id="SequenceErrorInterrupt" */
    pdb_clk_prescaler_div_t         clkPreDiv;              /*!< Select the prescaler divider. @internal gui name="Divider" id="Divider" */
    pdb_clk_prescaler_mult_factor_t clkPreMultFactor;       /*!< Select multiplication factor for prescaler. @internal gui name="Multiplier" id="Multiplier" */
    pdb_trigger_src_t               triggerInput;           /*!< Select the trigger input source. @internal gui name="Trigger" id="Trigger" */
    bool                            continuousModeEnable;   /*!< Enable the continuous mode. @internal gui name="Continuous mode" id="ContinuousMode" */
    bool                            dmaEnable;              /*!< Enable the dma for timer. @internal gui name="DMA" id="DMA" */
    bool                            intEnable;              /*!< Enable the interrupt for timer. @internal gui name="Interrupt" id="Interrupt" */
} pdb_timer_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Resets the PDB registers to a known state.
 *
 * This function resets the PDB registers to a known state. This state is
 * defined in a reference manual and is power on reset value.
 *
 * @param base Register base address for the module.
 */
void PDB_HAL_Init(PDB_Type * const base);

/*!
 * @brief Configure the PDB timer.
 *
 * This function configure the PDB's basic timer.
 *
 * @param base Register base address for the module.
 * @param configPtr Pointer to configuration structure, see to "pdb_timer_config_t".
 */
void PDB_HAL_ConfigTimer(PDB_Type * const base, const pdb_timer_config_t *const configPtr);

/*!
 * @brief Triggers the PDB by software if enabled.
 *
 * If enabled, this function triggers the PDB by using software.
 *
 * @param base Register base address for the module.
 * Implements : PDB_HAL_SetSoftTriggerCmd_Activity
 */
static inline void PDB_HAL_SetSoftTriggerCmd(PDB_Type * const base)
{
    DEV_ASSERT(base != NULL);
    REG_BIT_SET32(&(base->SC), PDB_SC_SWTRIG_MASK);
}

/*!
 * @brief Switches on to enable the PDB module.
 *
 * This function switches on to enable the PDB module.
 *
 * @param base Register base address for the module.
 * Implements : PDB_HAL_Enable_Activity
 */
static inline void PDB_HAL_Enable(PDB_Type * const base)
{
    DEV_ASSERT(base != NULL);
    REG_BIT_SET32(&(base->SC), PDB_SC_PDBEN_MASK);
}
/*!
 * @brief Switches to disable the PDB module.
 *
 * This function switches to disable the PDB module.
 *
 * @param base Register base address for the module.
 * Implements : PDB_HAL_Disable_Activity
 */
static inline void PDB_HAL_Disable(PDB_Type * const base)
{
    DEV_ASSERT(base != NULL);
    REG_BIT_CLEAR32(&(base->SC), PDB_SC_PDBEN_MASK);
}

/*!
 * @brief Gets the PDB delay interrupt flag.
 *
 * This function gets the PDB delay interrupt flag.
 *
 * @param base Register base address for the module.
 * @return Flat status, true if the flag is set.
 * Implements : PDB_HAL_GetTimerIntFlag_Activity
 */
static inline bool PDB_HAL_GetTimerIntFlag(PDB_Type const * const base)
{
    DEV_ASSERT(base != NULL);
    return (1U == ((base->SC & PDB_SC_PDBIF_MASK) >> PDB_SC_PDBIF_SHIFT));
}

/*!
 * @brief Clears the PDB delay interrupt flag.
 *
 * This function clears PDB delay interrupt flag.
 *
 * @param base Register base address for the module.
 * Implements : PDB_HAL_ClearTimerIntFlag_Activity
 */
static inline void PDB_HAL_ClearTimerIntFlag(PDB_Type * const base)
{
    DEV_ASSERT(base != NULL);
    volatile uint32_t dummy_read;
    REG_BIT_CLEAR32(&(base->SC), PDB_SC_PDBIF_MASK);

    /* This read-after-write guarantees that the write to clear operation is completed,
     * for the case when memory write buffering is enabled. */
    dummy_read = REG_READ32(&(base->SC));
    (void) dummy_read;
}

/*!
 * @brief Loads the delay registers value for the PDB module.
 *
 * This function sets the LDOK bit and loads the delay registers value.
 * Writing one  to this bit updates the internal registers MOD, IDLY, CHnDLYm and
 * POyDLY with the values written to their buffers. The MOD, IDLY,
 * CHnDLYm and POyDLY take effect according to the load mode settings.
 *
 * After one is written to the LDOK bit, the values in the buffers of above mentioned registers
 * are not effective and cannot be written until the values in the
 * buffers are loaded into their internal registers.
 * The LDOK can be written only when the the PDB is enabled or as alone with it. It is
 * automatically cleared either when the values in the buffers are loaded into the
 * internal registers or when the PDB is disabled.
 *
 * @param base Register base address for the module.
 * Implements : PDB_HAL_SetLoadValuesCmd_Activity
 */
static inline void PDB_HAL_SetLoadValuesCmd(PDB_Type * const base)
{
    DEV_ASSERT(base != NULL);
    REG_BIT_SET32(&(base->SC), PDB_SC_LDOK_MASK);
}

/*!
 * @brief Sets the modulus value for the PDB module.
 *
 * This function sets the modulus value for the PDB module.
 * When the counter reaches the setting value, it is automatically reset to zero.
 * When in continuous mode, the counter begins to increase
 * again.
 *
 * @param base Register base address for the module.
 * @param value The setting value of upper limit for PDB counter.
 * Implements : PDB_HAL_SetTimerModulusValue_Activity
 */
static inline void PDB_HAL_SetTimerModulusValue(PDB_Type * const base, uint32_t value)
{
    DEV_ASSERT(base != NULL);
    REG_RMW32(&(base->MOD), PDB_MOD_MOD_MASK, PDB_MOD_MOD(value));
}

/*!
 * @brief Gets the PDB counter value of PDB timer.
 *
 * This function gets the PDB counter value of PDB timer.
 *
 * @param base Register base address for the module.
 * @return The current counter value.
 * Implements : PDB_HAL_GetTimerValue_Activity
 */
static inline uint32_t PDB_HAL_GetTimerValue(PDB_Type const * const base)
{
    DEV_ASSERT(base != NULL);
    return ((base->CNT & PDB_CNT_CNT_MASK) >> PDB_CNT_CNT_SHIFT);
}

/*!
 * @brief Sets the interrupt delay milestone of the PDB counter.
 *
 * This function sets the interrupt delay milestone of the PDB counter.
 * If enabled, a PDB interrupt is generated when the counter is equal to the
 * setting value.
 *
 * @param base Register base address for the module.
 * @param value The setting value for interrupt delay milestone of PDB counter.
 * Implements : PDB_HAL_SetValueForTimerInterrupt_Activity
 */
static inline void PDB_HAL_SetValueForTimerInterrupt(PDB_Type * const base, uint32_t value)
{
    DEV_ASSERT(base != NULL);
    REG_RMW32(&(base->IDLY), PDB_IDLY_IDLY_MASK, PDB_IDLY_IDLY(value));
}

/*!
 * @brief Switches to enable the pre-trigger back-to-back mode.
 *
 * This function switches to enable the pre-trigger back-to-back mode.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChnMask ADC channel group index mask for trigger.
 * @param enable Switcher to assert the feature.
 */
void PDB_HAL_SetAdcPreTriggerBackToBackEnable(PDB_Type * const base, uint32_t chn, uint32_t preChnMask, bool enable);

/*!
 * @brief Switches to enable the pre-trigger output.
 *
 * This function switches to enable pre-trigger output.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChnMask ADC channel group index mask for trigger.
 * @param enable Switcher to assert the feature.
 */
void PDB_HAL_SetAdcPreTriggerOutputEnable(PDB_Type * const base, uint32_t chn, uint32_t preChnMask, bool enable);

/*!
 * @brief Switches to enable the pre-trigger.
 *
 * This function switches to enable the pre-trigger.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChnMask ADC channel group index mask for trigger.
 * @param enable Switcher to assert the feature.
 */
void PDB_HAL_SetAdcPreTriggerEnable(PDB_Type * const base, uint32_t chn, uint32_t preChnMask, bool enable);

/*!
 * @brief Gets the flag which indicates whether the PDB counter has reached the pre-trigger delay value.
 *
 * This function gets the flag which indicates the PDB counter has reached the
 * pre-trigger delay value.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChnMask ADC channel group index mask for trigger.
 * @return Flag mask. Indicated bit would be 1 if the event is asserted.
 * Implements : PDB_HAL_GetAdcPreTriggerFlags_Activity
 */
static inline uint32_t PDB_HAL_GetAdcPreTriggerFlags(PDB_Type const * const base, uint32_t chn, uint32_t preChnMask)
{
    DEV_ASSERT(base != NULL);
    DEV_ASSERT(chn < FEATURE_PDB_ADC_CHANNEL_COUNT);
    return (preChnMask & ((base->CH[chn].S & PDB_S_CF_MASK) >> PDB_S_CF_SHIFT));
}

/*!
 * @brief Clears the flag which indicates that the PDB counter has reached the pre-trigger delay value.
 *
 * This function clears the flag which indicates that the PDB counter has reached  the
 * pre-trigger delay value.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChnMask ADC channel group index mask for trigger.
 */
void PDB_HAL_ClearAdcPreTriggerFlags(PDB_Type * const base, uint32_t chn, uint32_t preChnMask);

/*!
 * @brief Gets the flag which indicates whether a sequence error is detected.
 *
 * This function gets the flag which indicates whether a sequence error is detected.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChnMask ADC channel group index mask for trigger.
 * @return Flag mask. Indicated bit would be 1 if the event is asserted.
 * Implements : PDB_HAL_GetAdcPreTriggerSeqErrFlags_Activity
 */
static inline uint32_t PDB_HAL_GetAdcPreTriggerSeqErrFlags(PDB_Type const * const base, uint32_t chn, uint32_t preChnMask)
{
    DEV_ASSERT(base != NULL);
    DEV_ASSERT(chn < FEATURE_PDB_ADC_CHANNEL_COUNT);
    return (preChnMask & ((base->CH[chn].S & PDB_S_ERR_MASK) >> PDB_S_ERR_SHIFT));
}

/*!
 * @brief Clears the flag which indicates that a sequence error has been detected.
 *
 * This function clears the flag which indicates that the sequence error has been detected.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChnMask ADC channel group index mask for trigger.
 */
void PDB_HAL_ClearAdcPreTriggerSeqErrFlags(PDB_Type * const base, uint32_t chn, uint32_t preChnMask);

/*!
 * @brief Sets the pre-trigger delay value.
 *
 * This function sets the pre-trigger delay value.
 *
 * @param base Register base address for the module.
 * @param chn ADC instance index for trigger.
 * @param preChn ADC channel group index for trigger.
 * @param value Setting value for pre-trigger's delay value.
 */
void PDB_HAL_SetAdcPreTriggerDelayValue(PDB_Type * const base, uint32_t chn, uint32_t preChn, uint32_t value);

/*!
 * @brief Switches to enable the pulse-out trigger.
 *
 * This function switches to enable the pulse-out trigger.
 *
 * @param base Register base address for the module.
 * @param pulseChnMask Pulse-out channle index mask for trigger.
 * @param enable Switcher to assert the feature.
 */
void PDB_HAL_SetCmpPulseOutEnable(PDB_Type * const base, uint32_t pulseChnMask, bool enable);

/*!
 * @brief Sets the counter delay value for the pulse-out goes high.
 *
 * This function sets the counter delay value for the pulse-out goes high.
 *
 * @param base Register base address for the module.
 * @param pulseChn Pulse-out channel index for trigger.
 * @param value Setting value for PDB delay .
 * Implements : PDB_HAL_SetCmpPulseOutDelayForHigh_Activity
 */
static inline void PDB_HAL_SetCmpPulseOutDelayForHigh(PDB_Type * const base, uint32_t pulseChn, uint32_t value)
{
    DEV_ASSERT(base != NULL);
    DEV_ASSERT(pulseChn < FEATURE_PDB_PODLY_COUNT);
    base->POnDLY[pulseChn].ACCESS16BIT.DLY1 = (uint16_t)PDB_POnDLY_ACCESS16BIT_DLY1_DLY1(value);
}

/*!
 * @brief Sets the counter delay value for the pulse-out goes low.
 *
 * This function sets the counter delay value for the pulse-out goes low.
 *
 * @param base Register base address for the module.
 * @param pulseChn Pulse-out channel index for trigger.
 * @param value Setting value for PDB delay .
 * Implements : PDB_HAL_SetCmpPulseOutDelayForLow_Activity
 */
static inline void PDB_HAL_SetCmpPulseOutDelayForLow(PDB_Type * const base, uint32_t pulseChn, uint32_t value)
{
    DEV_ASSERT(base != NULL);
    DEV_ASSERT(pulseChn < FEATURE_PDB_PODLY_COUNT);
    base->POnDLY[pulseChn].ACCESS16BIT.DLY2 = (uint16_t)PDB_POnDLY_ACCESS16BIT_DLY2_DLY2(value);
}

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* PDB_HAL_H */

/******************************************************************************
 * EOF
 *****************************************************************************/
