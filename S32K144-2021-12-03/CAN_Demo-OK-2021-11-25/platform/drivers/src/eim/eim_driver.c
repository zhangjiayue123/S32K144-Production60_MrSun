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

/*!
 * @file eim_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned int define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned int.
 * The cast is required to initialize a pointer with an unsigned int define,
 * representing an address.
 */

#include <stddef.h>
#include "eim_driver.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Table of base addresses for EIM instances. */
EIM_Type * const g_eimBase[] = EIM_BASE_PTRS;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : EIM_DRV_Init
 * Description   : Initializes EIM module.
 * This function configures for EIM channels.
 * The EIM channel configuration structure array and number of configured channels
 * shall be passed as arguments and enables module.
 * This function should be called before calling any other EIM driver function
 * and make sure that EIM module has disabled before calling this function.
 *
 * Implements    : EIM_DRV_Init_Activity
 *END**************************************************************************/
void EIM_DRV_Init(uint32_t instance,
                  uint8_t channelCnt,
                  const eim_user_channel_config_t *channelConfigArr)
{
    EIM_Type * base;

    DEV_ASSERT(instance < EIM_INSTANCE_COUNT);

    base = g_eimBase[instance];
    uint8_t index;

    DEV_ASSERT(instance < EIM_INSTANCE_COUNT);
    DEV_ASSERT(channelCnt > 0U);
    DEV_ASSERT(channelCnt <= EIM_EICHDn_COUNT);
    DEV_ASSERT(channelConfigArr != NULL);

    for(index = 0U; index < channelCnt; index++)
    {
        /* Configures for EIM channel */
        EIM_DRV_ConfigChannel(instance, &channelConfigArr[index]);
    }

    /* Enables EIM module */
    EIM_HAL_Enable(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EIM_DRV_Deinit
 * Description   : De-initializes EIM module.
 * This function sets all registers to reset value and disables EIM module.
 * In order to use the EIM module again, EIM_DRV_Init must be called.
 *
 * Implements    : EIM_DRV_Deinit_Activity
 *END**************************************************************************/
void EIM_DRV_Deinit(uint32_t instance)
{
    EIM_Type * base;

    DEV_ASSERT(instance < EIM_INSTANCE_COUNT);

    base = g_eimBase[instance];

    /* Disables EIM module */
    EIM_HAL_Disable(base);
    /* Resets EIM descriptors and disables EIM channels */
    EIM_HAL_Init(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EIM_DRV_ConfigChannel
 * Description   : This function configures check bit mask, data mask and
 * operation status(enable/disable) for EIM channel.
 * The EIM channel configuration structure shall be
 * passed as arguments.
 * Make sure that EIM module has disabled before calling this function.
 *
 * Implements    : EIM_DRV_ConfigChannel_Activity
 *END**************************************************************************/
void EIM_DRV_ConfigChannel(uint32_t instance,
                           const eim_user_channel_config_t *userChannelConfig)
{
    EIM_Type * base;

    DEV_ASSERT(instance < EIM_INSTANCE_COUNT);
    DEV_ASSERT(userChannelConfig != NULL);
    DEV_ASSERT(userChannelConfig->channel < EIM_EICHDn_COUNT);

    base = g_eimBase[instance];

    /* Disables EIM channel to configure EIM channel */
    EIM_HAL_EnableChannelCmd(base, userChannelConfig->channel, false);
    /* Configures checkbit mask for EIM channel */
    EIM_HAL_SetCheckBitMask(base, userChannelConfig->channel, userChannelConfig->checkBitMask);
    /* Configures data mask for EIM channel */
    EIM_HAL_SetDataMask(base, userChannelConfig->channel, userChannelConfig->dataMask);
    /* Enables or disables EIM channel operation corresponding to channel configuration */
    EIM_HAL_EnableChannelCmd(base, userChannelConfig->channel, userChannelConfig->enable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EIM_DRV_GetChannelConfig
 * Description   : This function gets checkbit mask, data mask and operation
 * status of EIM channel.
 *
 * Implements    : EIM_DRV_GetChannelConfig_Activity
 *END**************************************************************************/
void EIM_DRV_GetChannelConfig(uint32_t instance,
                              uint8_t channel,
                              eim_user_channel_config_t *channelConfig)
{
    const EIM_Type * base;

    DEV_ASSERT(instance < EIM_INSTANCE_COUNT);
    DEV_ASSERT(channelConfig != NULL);
    DEV_ASSERT(channel < EIM_EICHDn_COUNT);

    base = g_eimBase[instance];

    /* Gets the channel number */
    channelConfig->channel = channel;
    /* Gets checkbit mask of EIM channel */
    channelConfig->checkBitMask = EIM_HAL_GetCheckBitMask(base, channel);
    /* Gets data mask of EIM channel */
    channelConfig->dataMask = EIM_HAL_GetDataMask(base, channel);
    /* Gets operation status */
    channelConfig->enable = EIM_HAL_IsChannelEnabled(base, channel);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
