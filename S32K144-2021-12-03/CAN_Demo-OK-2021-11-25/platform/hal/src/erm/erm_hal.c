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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

#include "erm_hal.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_HAL_Init
 * Description   : This function initializes the module to default configuration,
 * the configuration register is initialized with interrupt notification disabled
 * for all channels and the status register events are cleared.
 *
 * Implements    : ERM_HAL_Init_Activity
 *END**************************************************************************/
void ERM_HAL_Init(ERM_Type * const base)
{
    base->CR0 = 0UL;
    /* Write 1 to clear flags */
    base->SR0 = 0xFFFFFFFFUL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_HAL_GetErrorDetail
 * Description   : This function gets the address of the last ECC event
 * in Memory n and ECC event.
 *
 * Implements    : ERM_HAL_GetErrorDetail_Activity
 *END**************************************************************************/
erm_ecc_event_t ERM_HAL_GetErrorDetail(const ERM_Type * const base,
                                       uint8_t channel,
                                       uint32_t * addressPtr)
{
    DEV_ASSERT(addressPtr != NULL);
    erm_ecc_event_t eccEvent = ERM_EVENT_NONE;

    /* Single-bit correction ECC events */
    if (ERM_HAL_IsEventDetected(base, channel, ERM_EVENT_SINGLE_BIT) != false)
    {
        /* Assign eccEvent is Single-bit correction ECC events */
        eccEvent = ERM_EVENT_SINGLE_BIT;
        /* Address */
        *addressPtr = ERM_HAL_GetLastErrorAddress(base, channel);
    }
    /* Non-correctable ECC events */
    else
    {
        if (ERM_HAL_IsEventDetected(base, channel, ERM_EVENT_NON_CORRECTABLE) != false)
        {
            /* Assign eccEvent is Non-correctable ECC events */
            eccEvent = ERM_EVENT_NON_CORRECTABLE;
            /* Address */
            *addressPtr = ERM_HAL_GetLastErrorAddress(base, channel);
        }
    }

    return eccEvent;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
