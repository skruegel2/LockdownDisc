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
#include "stm32f4xx_flash_map.h"

#if defined(STM32XX_MEMORY_VARIANT_E)
#define STM32F4xx_FLASH_SECTORS_COUNT (8)

#elif defined(STM32XX_MEMORY_VARIANT_G)
#define STM32F4xx_FLASH_SECTORS_COUNT (12)

#elif defined(STM32XX_MEMORY_VARIANT_I)
#define STM32F4xx_FLASH_SECTORS_COUNT (24)

#else
#error "Undefined STM32F4 memory variant."
#endif

/* This array contains sector base addresses.
 * The last position works as a tail to get the end address of the last sector. */
static const hal_mem_address_t sector_base_addresses[STM32F4xx_FLASH_SECTORS_COUNT+1] = {
    0x08000000U,
    0x08004000U,
    0x08008000U,
    0x0800C000U,
    0x08010000U,
    0x08020000U,
    0x08040000U,
    0x08060000U,
#if STM32F4xx_FLASH_SECTORS_COUNT > 8
    0x08080000U,
    0x080A0000U,
    0x080C0000U,
    0x080E0000U,
#if STM32F4xx_FLASH_SECTORS_COUNT > 12
    0x08100000U,
    0x08104000U,
    0x08108000U,
    0x0810C000U,
    0x08110000U,
    0x08120000U,
    0x08140000U,
    0x08160000U,
    0x08180000U,
    0x081A0000U,
    0x081C0000U,
    0x081E0000U,
#endif
#endif
    SOC_FLASH_END_ADDRESS + 1
};

int soc_flash_get_address_sector(hal_mem_address_t address)
{
    for(int sector=1; sector <= STM32F4xx_FLASH_SECTORS_COUNT; ++sector)
    {
        if(address < sector_base_addresses[sector])
        {
            return sector-1;
        }
    }

    return -1;
}
