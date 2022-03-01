/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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

/*!
 * @file ewm_hal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially Boolean'
 * to 'essentially unsigned'. This is required by the conversion of a bool into a bit.
 */

#include "ewm_hal.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : EWM_HAL_Init
 * Description   : Init EWM. This method configures the EWM instance Control
 * Register fields such as interrupt enable, input pin, instance enable.
 * The user must make sure that the prescaler, compare high and compare low
 * registers are configured prior to this function call
 *
 * Implements : EWM_HAL_Init_Activity
 *END**************************************************************************/
void EWM_HAL_Init(EWM_Type * const base,
                  bool interruptEnable,
                  ewm_in_assert_logic_t assertLogic,
                  bool enable)
{

    /* Set the values that are not affected by the input pin */
    uint8_t tempValue = (uint8_t)(EWM_CTRL_EWMEN(enable) | EWM_CTRL_INTEN(interruptEnable));

    /* Depending how the input pin is configured set the values into the
     * temporary variable
     */
    switch(assertLogic)
    {
    case EWM_IN_ASSERT_ON_LOGIC_ZERO:
        tempValue |= (uint8_t)(EWM_CTRL_INEN(1U) |    /* Input pin enabled         */
                               EWM_CTRL_ASSIN(0U));   /* Input asserted on logic 0 */
        break;
    case EWM_IN_ASSERT_ON_LOGIC_ONE:
        tempValue |= (uint8_t)(EWM_CTRL_INEN(1U) |    /* Input pin enabled         */
                               EWM_CTRL_ASSIN(1U));   /* Input asserted on logic 1 */
        break;
    default:
        /* Input pin disabled */
        break;
    }

    /* Write the configuration into the Control register */
    base->CTRL = tempValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EWM_HAL_GetInputPinAssertLogic
 * Description   : Get the Input pin assert logic
 *
 * Implements : EWM_HAL_GetInputPinAssertLogic_Activity
 *END**************************************************************************/
ewm_in_assert_logic_t EWM_HAL_GetInputPinAssertLogic(const EWM_Type * const base)
{
    /* Variable where to save the retrieved configuration */
    ewm_in_assert_logic_t returnValue;
    /* Temporary variable to use for storing the configuration */
    uint8_t tempValue;

    /* Check if input pin is enabled */
    if((base->CTRL & EWM_CTRL_INEN_MASK) != 0U)
    {
        /* If true get the assert logic into the temp variable */
        tempValue = (uint8_t)((base->CTRL & EWM_CTRL_ASSIN_MASK) >> EWM_CTRL_ASSIN_SHIFT);

        /* Convert the assert logic to the corresponding ewm_in_assert_logic_t
         * value.
         */
        switch(tempValue)
        {
            case 0U:
                returnValue = EWM_IN_ASSERT_ON_LOGIC_ZERO;
                break;
            case 1U:
                returnValue = EWM_IN_ASSERT_ON_LOGIC_ONE;
                break;
            default:
                returnValue = EWM_IN_ASSERT_DISABLED;
                break;
        }
    }
    else
    {
        /* Pin is disabled, return the corresponding value */
        returnValue = EWM_IN_ASSERT_DISABLED;
    }

    return returnValue;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
