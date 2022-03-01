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

/*!
 * @file mpu_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or
 * different essential type.
 * This is required by the conversion of a bit-field of a register into a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially unsigned' type to 'essentially enum<i>'.
 * This is required by the conversion of a bit-field of a register into a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite
 * expression (different essential type categories).
 * This is required by the conversion of a bit-field of a register into enum type.
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
#include "mpu_driver.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Table of base addresses for MPU instances. */
MPU_Type * const g_mpuBase[] = MPU_BASE_PTRS;

/*******************************************************************************
 * Code
 *******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_Init
 * Description   : Sets the MPU regions according to user input, and then enables the MPU.
 * Please note that access rights for region 0 will always be configured and
 * regionCnt takes values between 1 and the maximum region count supported by the hardware.
 * E.g. In S32K144 the number of supported regions is 8.
 * The user must make sure that the clock is enabled.
 *
 * Implements    : MPU_DRV_Init_Activity
 *END**************************************************************************/
status_t MPU_DRV_Init(uint32_t instance,
                      uint8_t regionCnt,
                      const mpu_user_config_t *userConfigArr)
{
    MPU_Type * base;
    status_t retStatus = STATUS_SUCCESS;
    uint8_t i;

    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(userConfigArr != NULL);
    DEV_ASSERT((regionCnt > 0U) && (regionCnt <= MPU_RGD_COUNT));

    base = g_mpuBase[instance];

    /* Initializes the MPU module by its default */
    MPU_HAL_Init(base);

    /*
     * In region 0:
     * Only bus master CORE, DMA can be changed access right (DEBUG is ignored).
     * The start address, end address, process identifier and process identifier mask are ignored.
     * Sets Master Access Right for region 0.
     */
    for (i = 0U; i < FEATURE_MPU_MASTER_COUNT; i++)
    {
        retStatus = MPU_DRV_SetMasterAccessRights(instance, 0U, &(userConfigArr[0U].masterAccRight[i]));
        if (retStatus != STATUS_SUCCESS)
        {
            break;
        }
    }

    /*
     * From region 1 to the next region.
     * Initializes the regions, array index 1 is for region 1 and so on.
     */
    for (i = 1U; i < regionCnt; i++)
    {
        if (retStatus != STATUS_SUCCESS)
        {
            break;
        }
        else
        {
            retStatus = MPU_DRV_SetRegionConfig(instance, i, &userConfigArr[i]);
        }
    }

    if (STATUS_SUCCESS == retStatus)
    {
        /* Enables the MPU module operation */
        MPU_HAL_Enable(base);
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_Deinit
 * Description   : De-initializes the MPU region by resetting and disabling MPU module.
 *
 * Implements    : MPU_DRV_Deinit_Activity
 *END**************************************************************************/
void MPU_DRV_Deinit(uint32_t instance)
{
    MPU_Type * base;

    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);

    base = g_mpuBase[instance];

    /* Initialize the MPU module by its default */
    MPU_HAL_Init(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetRegionAddr
 * Description   : Sets the region start address and end address.
 *
 * Implements    : MPU_DRV_SetRegionAddr_Activity
 *END**************************************************************************/
void MPU_DRV_SetRegionAddr(uint32_t instance,
                           uint8_t regionNum,
                           uint32_t startAddr,
                           uint32_t endAddr)
{
    MPU_Type * base;

    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(regionNum < MPU_RGD_COUNT);
    DEV_ASSERT(startAddr <= endAddr);

    base = g_mpuBase[instance];

    /* Sets a region's start and end addresses */
    MPU_HAL_SetRegionAddr(base, regionNum, startAddr, endAddr);

    /* Re-enables the region descriptor valid bit */
    MPU_HAL_SetRegionValidCmd(base, regionNum, true);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetRegionConfig
 * Description   : Sets the region configuration.
 *
 * Implements    : MPU_DRV_SetRegionConfig_Activity
 *END**************************************************************************/
status_t MPU_DRV_SetRegionConfig(uint32_t instance,
                                 uint8_t regionNum,
                                 const mpu_user_config_t *userConfigPtr)
{
    MPU_Type * base;
    status_t returnCode = STATUS_SUCCESS;
    mpu_high_masters_access_rights_t highAccRight;
    mpu_low_masters_access_rights_t lowAccRight;
    uint8_t accRight;
    uint8_t masterNum;
    uint8_t i;

    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(userConfigPtr != NULL);
    DEV_ASSERT((regionNum > 0U) && (regionNum < MPU_RGD_COUNT));

    base = g_mpuBase[instance];

    /* Configures region start and end address */
    MPU_HAL_SetRegionAddr(base, regionNum, userConfigPtr->startAddr, userConfigPtr->endAddr);

    /* Configures region access rights */
    for (i = 0U; i < FEATURE_MPU_MASTER_COUNT; i++)
    {
        accRight = (uint8_t)userConfigPtr->masterAccRight[i].accessRight;
        masterNum = userConfigPtr->masterAccRight[i].masterNum;
        /* Configures access rights for bus masters */
        if (masterNum > FEATURE_MPU_MAX_LOW_MASTER_NUMBER)
        {
            if (masterNum > FEATURE_MPU_MAX_HIGH_MASTER_NUMBER)
            {
                returnCode = STATUS_ERROR;
                break;
            }
            else
            {
                /* For masters which have only read and write permissions */
                highAccRight.writeEnable = (((accRight & MPU_W_MASK) >> MPU_W_SHIFT) != 0U);
                highAccRight.readEnable = (((accRight & MPU_R_MASK) >> MPU_R_SHIFT) != 0U);
                MPU_HAL_SetHighMasterAccessRights(base, regionNum, masterNum, &highAccRight);
            }
        }
        else
        {
            /* For masters which have separated privilege rights for user and supervisor mode accesses (e.g. master0~2 in S32K144) */
            lowAccRight.userAccessRights = (mpu_user_access_rights_t)((accRight & MPU_USER_MASK) >> MPU_USER_SHIFT);
            lowAccRight.superAccessRights = (mpu_supervisor_access_rights_t)((accRight & MPU_SUPERVISOR_MASK) >> MPU_SUPERVISOR_SHIFT);
        #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
            lowAccRight.processIdentifierEnable = userConfigPtr->masterAccRight[i].processIdentifierEnable;
        #endif
            MPU_HAL_SetLowMasterAccessRights(base, regionNum, masterNum, &lowAccRight);
        }
    }

    if (STATUS_SUCCESS == returnCode)
    {
    #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
        /* Sets Process Identifier */
        MPU_HAL_SetProcessIdentifier(base, regionNum, userConfigPtr->processIdentifier);

        /* Sets Process Identifier mask */
        MPU_HAL_SetProcessIdentifierMask(base, regionNum, userConfigPtr->processIdMask);
    #endif

        /* Validate this region */
        MPU_HAL_SetRegionValidCmd(base, regionNum, true);
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetMasterAccessRights
 * Description   : Configures access permission.
 *
 * Implements    : MPU_DRV_SetMasterAccessRights_Activity
 *END**************************************************************************/
status_t MPU_DRV_SetMasterAccessRights(uint32_t instance,
                                       uint8_t regionNum,
                                       const mpu_master_access_right_t *accessRightsPtr)
{
    MPU_Type * base;
    status_t returnCode = STATUS_SUCCESS;
    mpu_high_masters_access_rights_t highAccRight;
    mpu_low_masters_access_rights_t lowAccRight;
    uint8_t accRight;

    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(regionNum < MPU_RGD_COUNT);
    DEV_ASSERT(accessRightsPtr != NULL);

    base = g_mpuBase[instance];

    accRight = (uint8_t)accessRightsPtr->accessRight;
    if (accessRightsPtr->masterNum > FEATURE_MPU_MAX_LOW_MASTER_NUMBER)
    {
        if (accessRightsPtr->masterNum > FEATURE_MPU_MAX_HIGH_MASTER_NUMBER)
        {
            returnCode = STATUS_ERROR;
        }
        else
        {
            /* For masters which have only read and write permissions */
            highAccRight.writeEnable = (((accRight & MPU_W_MASK) >> MPU_W_SHIFT) != 0U);
            highAccRight.readEnable = (((accRight & MPU_R_MASK) >> MPU_R_SHIFT) != 0U);
            MPU_HAL_SetAlternateHighMasterAccessRights(base, regionNum, accessRightsPtr->masterNum, &highAccRight);
        }
    }
    else
    {
        /* For masters which have separated privilege rights for user and supervisor mode accesses (e.g. master0~2 in S32K144) */
        lowAccRight.userAccessRights = (mpu_user_access_rights_t)((accRight & MPU_USER_MASK) >> MPU_USER_SHIFT);
        lowAccRight.superAccessRights = (mpu_supervisor_access_rights_t)((accRight & MPU_SUPERVISOR_MASK) >> MPU_SUPERVISOR_SHIFT);
    #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
        lowAccRight.processIdentifierEnable = accessRightsPtr->processIdentifierEnable;
    #endif
        MPU_HAL_SetAlternateLowMasterAccessRights(base, regionNum, accessRightsPtr->masterNum, &lowAccRight);
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_GetDetailErrorAccessInfo
 * Description   : Gets the MPU access error detail information for a slave port.
 *
 * Implements    : MPU_DRV_GetDetailErrorAccessInfo_Activity
 *END**************************************************************************/
void MPU_DRV_GetDetailErrorAccessInfo(uint32_t instance,
                                      uint8_t slavePortNum,
                                      mpu_access_err_info_t *errInfoPtr)
{
    MPU_Type * base;

    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(slavePortNum < FEATURE_MPU_SLAVE_COUNT);
    DEV_ASSERT(errInfoPtr != NULL);

    base = g_mpuBase[instance];

    /* Check if there is access violation in the slave port */
    if (MPU_HAL_GetSlavePortErrorStatus(base, slavePortNum))
    {
        /* Get the slave port detail error */
        MPU_HAL_GetDetailErrorAccessInfo(base, slavePortNum, errInfoPtr);
    }
}

/*******************************************************************************
 * EOF
 *******************************************************************************/
