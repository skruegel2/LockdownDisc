/********************************************************************************
* Copyright 2017-2022 Secure Thingz Ltd.
* All rights reserved.
*
* This source file and its use is subject to a Secure Thingz Embedded Trust
* License agreement. This source file may contain licensed source code from
* other third-parties and is subject to those license agreements as well.
*
* Permission to use, copy, modify, compile and distribute compiled binary of the
* source code for use as specified in the Embedded Trust license agreement is
* hereby granted provided that the this copyright notice and other third-party
* copyright notices appear in all copies of the source code.
*
* Distribution of Embedded Trust source code in any form is governed by the
* Embedded Trust license agreement. Use of the Secure Thingz name or trademark
* in any form is prohibited.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/** @file
 * \brief Header file for flash lock functions.
 */

#ifndef SOC_ST_STM32F4xx_FLASH_H
#define SOC_ST_STM32F4xx_FLASH_H

#include "stm32f4xx_hal.h"
#include "memoryMap.h"
#include "sbm_hal.h"
#include "oem_board.h"
#include "stm32f4xx_flash_map.h"

/**
  * @brief  Protect the FLASH control registers for the specified bank.
  *
  * This is done by writing an invalid key sequence to the KEY
  * register. We'd normally write FLASH_KEY1 following by FLASH_KEY2
  * but by writing FLASH_KEY1 twice (or any value, really), the
  * controller will lock itself down until the next system reset.
  *
  * A system reset is required to remove this protection.
  * Note that the caller must be prepared to handle an immediate bus-
  * error as a result of invoking this function.
  *
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Lock_Protect();

/**
  * @brief  Protect the FLASH Option Control Registers.
  * A system reset is required to remove this protection.
  * Note that the caller must be prepared to handle an immediate bus-
  * error as a result of invoking this function.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_OB_Lock_Protect(void);

#endif /* SOC_ST_STM32F4xx_FLASH_H */
