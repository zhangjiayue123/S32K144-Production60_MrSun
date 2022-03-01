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
 * @file wdog_driver.h
 */

#ifndef WDOG_DRIVER_H
#define WDOG_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "wdog_hal.h"
#include "interrupt_manager.h"
#include "clock_manager.h"
#include "status.h"

/*!
 * @defgroup wdog_drv WDOG Driver
 * @ingroup wdog
 * @brief Watchdog Timer Peripheral Driver.
 * @addtogroup wdog_drv
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for WDOG instances. */
extern WDOG_Type * const g_wdogBase[WDOG_INSTANCE_COUNT];

/* Table to save WDOG IRQ enumeration numbers defined in the CMSIS header file. */
extern const IRQn_Type g_wdogIrqId[WDOG_INSTANCE_COUNT];

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/

 /*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name WDOG Driver API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initializes the WDOG driver.
 *
 * @param[in] instance  WDOG peripheral instance number
 * @param[in] userConfigPtr    pointer to the WDOG user configuration structure
 * @return operation status
 *        - STATUS_SUCCESS:    Operation was successful.
 *        - STATUS_ERROR:      Operation failed. Possible causes: previous
 *        clock source or the one specified in the configuration structure is
 *        disabled; WDOG configuration updates are not allowed.
 */
status_t WDOG_DRV_Init(uint32_t instance,
                            const wdog_user_config_t *userConfigPtr);
/*!
 * @brief De-initializes the WDOG driver
 *
 * @param[in] instance  WDOG peripheral instance number
 */
void WDOG_DRV_Deinit(uint32_t instance);

/*!
 * @brief Gets the current configuration of the WDOG.
 *
 * @param[in] instance  WDOG peripheral instance number
 * @param[out] config  the current configuration
 */
void WDOG_DRV_GetConfig(uint32_t instance, wdog_user_config_t *config);

/*!
 * @brief  Enables/Disables the WDOG timeout interrupt and sets a function to be
 * called when a timeout interrupt is received, before reset.
 *
 * @param[in] instance  WDOG peripheral instance number
 * @param[in] enable  enable/disable interrupt
 * @param[in] handler   timeout interrupt handler
 * @return operation status
 *        - STATUS_SUCCESS:    Operation was successful.
 *        - STATUS_ERROR:      Operation failed. Possible causes: failed to
 *        install handler; WDOG configuration updates not allowed.
 */
status_t WDOG_DRV_SetInt(uint32_t instance, bool enable, void (*handler)(void));

/*!
 * @brief Refreshes the WDOG counter.
 *
 * @param[in] instance  WDOG peripheral instance number
 */
void WDOG_DRV_Trigger(uint32_t instance);



/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* WDOG_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
