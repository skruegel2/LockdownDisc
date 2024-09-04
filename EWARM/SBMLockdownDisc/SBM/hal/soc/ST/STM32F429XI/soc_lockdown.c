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

#include <setjmp.h>
#include "lockdown.h"
#include "stm32f4xx_hal.h"
#include "sbm_hal.h"
#include "soc_stm32f4xx_flash.h"
#include "soc_lockdown.h"
#include "memory_devices_and_slots.h"

#if defined(SBM_LOCKDOWN_LEVEL) && (SBM_LOCKDOWN_LEVEL != 0) || \
    defined(SBM_HAL_UNIT_TESTS)

/*
 * Define LOCKDOWN_SAFETY_NET to prevent the code from raising RDP, otherwise
 * you stand a very real chance of bricking your device.
 */
//#define LOCKDOWN_SAFETY_NET

/*
 * Convert RDP magic numbers into a useful level
 */
#define CURRENT_RDP_LEVEL(rdp)                                           \
    (                                                                    \
        (rdp) == OB_RDP_LEVEL_0 ? SBM_LOCKDOWN_LEVEL_UNLOCKED : (        \
            (rdp) == OB_RDP_LEVEL_2 ? SBM_LOCKDOWN_LEVEL_LOCKED_PERM :   \
                SBM_LOCKDOWN_LEVEL_LOCKED_TEMP                           \
        )                                                                \
    )

int soc_lockdown_level(void)
{
    FLASH_OBProgramInitTypeDef options;

    /* Fetch the current options from Bank 1 */
    options.Banks = FLASH_BANK_1;
    HAL_FLASHEx_OBGetConfig(&options);

    return CURRENT_RDP_LEVEL(options.RDPLevel);
}

int soc_lockdown_raise_level(int new_level)
{
    FLASH_OBProgramInitTypeDef options;
    int current_level, rv;

    /* Fetch the current options from Bank 1 */
    options.Banks = FLASH_BANK_1;
    HAL_FLASHEx_OBGetConfig(&options);

    /* Get current level and validate requested level. */
    current_level = CURRENT_RDP_LEVEL(options.RDPLevel);
    if (new_level < SBM_LOCKDOWN_LEVEL_UNLOCKED ||
        new_level > SBM_LOCKDOWN_LEVEL_LOCKED_PERM)
    {
        return -1;
    }

    /* We can only raise lockdown level. */
    if (new_level <= current_level)
        return current_level;

#ifndef LOCKDOWN_SAFETY_NET
    /* Set the new RDP level */
    options.OptionType = OPTIONBYTE_RDP;
    options.RDPLevel = (new_level == 2) ? OB_RDP_LEVEL_2 : OB_RDP_LEVEL_1;
#else
    /* The safety net will raise to RDP level 1 only */
    options.OptionType = OPTIONBYTE_RDP;
    options.RDPLevel = OB_RDP_LEVEL_1;
    (void) new_level;
#endif

    /* the WRPSector that comes from HAL_FLASHEx_OBGetConfig is the value of 'nWRPi'
       register, which marks locked sectors as '0'. However, when writing using
       HAL_FLASHEx_OBProgram locked sectors are marked as '1' (OB_WRP_SECTOR_x)
    */
    options.WRPSector = (~options.WRPSector) & 0xFFFu;

    /* Get the OB_WRP mask for the SBM sectors (OR mask with all the sectors) */
    unsigned int sbm_ob_wrp = 0;

    int start = soc_flash_get_address_sector(sbm_slot.start_address),
        end = soc_flash_get_address_sector(sbm_slot.start_address + sbm_slot.size - 1);

    if (start == -1 || end == -1)
    {
        return -1;
    }

    for (int sector = start; sector <= end; ++sector)
    {
        sbm_ob_wrp |= (1U << sector);
    }

    /* detect if the SBM sectors aren't WRP'ed yet */
    if ((options.WRPSector & sbm_ob_wrp) != sbm_ob_wrp)
    {
        /* Write-protect the SBM sectors */
        options.WRPSector |= sbm_ob_wrp;
        options.OptionType |= OPTIONBYTE_WRP;
        options.WRPState = OB_WRPSTATE_ENABLE;
    }

#ifdef LOCKDOWN_SAFETY_NET
    /*
     * Just return if nothing has changed
     */
    if (0 == options.OptionType)
    {
        return current_level;
    }
#endif

    /* Unlock access to Option byte "*_PRG" registers */
    (void) HAL_FLASH_OB_Unlock();

    /* Make the necessary changes to the option bytes */
    /* On success, commit the changes and make them active */
    rv = HAL_FLASHEx_OBProgram(&options);
    if (rv == HAL_OK)
    {
        HAL_FLASH_OB_Launch();
    }

    /* Lock option byte "*_PRG" registers */
    (void) HAL_FLASH_OB_Lock();

    return rv == HAL_OK ? soc_lockdown_level() : -1;
}

int soc_lockdown_firmware(void)
{
    /* STM32F407 is a single bank flash, we don't do anything here. */
    return soc_lockdown_level();
}

#endif /* defined(SBM_LOCKDOWN_LEVEL) && (SBM_LOCKDOWN_LEVEL != 0) */
