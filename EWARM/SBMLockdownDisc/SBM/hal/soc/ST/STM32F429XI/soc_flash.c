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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "soc_flash.h"
#include "soc_stm32f4xx_flash.h"
#include "oem_bsp.h"

#define FLASH_ERASE_VALUE 0xFFu

#define FLASH_WR_PAGE_SIZE (sizeof(flash_page_t)) /* 32 bits (FLASH_TYPEPROGRAM_WORD) */

typedef uint32_t flash_page_t;

static void flash_lock(void);
static inline void flash_unlock(void);

void soc_flash_init(void)
{
    /* Purge any left-over errors */
    flash_unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR);
    flash_lock();
}

size_t soc_flash_page_size(void)
{
    return sizeof(uint32_t);
}

hal_mem_result_t soc_flash_read(hal_mem_address_t address, void *dst, size_t size)
{
    /* Precondition checks */
    assert(dst != NULL);

    /* Sanity check address range */
    assert(address >= SOC_FLASH_START_ADDRESS);
    assert(address <  (SOC_FLASH_START_ADDRESS + SOC_FLASH_SIZE));
    assert(size    <= (SOC_FLASH_START_ADDRESS + SOC_FLASH_SIZE) - address);

    memcpy(dst, (const void*)address, size);
    return HAL_MEM_SUCCESS;
}

hal_mem_result_t soc_flash_write(hal_mem_address_t address, const void *src, size_t size)
{
    /* Precondition checks */
    assert(src != NULL);
    assert(((uintptr_t)src & 0x3) == 0);
    assert((address % FLASH_WR_PAGE_SIZE) == 0);
    assert((size % FLASH_WR_PAGE_SIZE) == 0);

    const uint32_t *src_words = (const uint32_t*)src;

    /* Unlock the Flash and program each page (or "word" in ST HAL speak) */
    flash_unlock();

    for (size_t i = 0; i < size; i += FLASH_WR_PAGE_SIZE, ++src_words)
    {
        /*
        * The Program function is located in RAM, and we must ensure Flash is not
        * accessed while the program operation is in progress, thus interrupts
        * must be disabled during programming.
        */
        const uint32_t x = cpu_critical_enter();
        const HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                                                           address + i,
                                                           *src_words);
        cpu_critical_exit(x);

        if (status != HAL_OK)
        {
            flash_lock();
            return HAL_MEM_PROGRAM_ERROR;
        }
    }

    flash_lock();
    return HAL_MEM_SUCCESS;
}

hal_mem_result_t soc_flash_erase(hal_mem_address_t address, size_t size)
{
    FLASH_EraseInitTypeDef flash_erase;
    uint32_t result;
    HAL_StatusTypeDef status;

    const int start_sector = soc_flash_get_address_sector(address);
    const int end_sector   = soc_flash_get_address_sector((address + size) - 1);

    if ((start_sector < 0) || (end_sector < 0) || (end_sector < start_sector))
    {
        /* The address range should have been verified above, so this shouldn't fail
         * unless there's a bug */
        return HAL_MEM_INTERNAL_ERROR;
    }

    flash_erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    flash_erase.Sector       = (uint32_t)start_sector;
    flash_erase.NbSectors    = ((uint32_t)end_sector - (uint32_t)start_sector) + 1;
    flash_erase.VoltageRange = OEM_FLASH_VOLTAGE_RANGE;

    /*
     * The Erase function is located in RAM, and we must ensure Flash is not
     * accessed while the erase operation is in progress, thus interrupts
     * must be disabled.
     */
    const uint32_t x = cpu_critical_enter();

    /* Unlock Flash and perform the erase */
    flash_unlock();
    status = HAL_FLASHEx_Erase(&flash_erase, &result);
    flash_lock();

    /* Restore interrupts */
    cpu_critical_exit(x);

    return (HAL_OK == status) ? HAL_MEM_SUCCESS : HAL_MEM_ERASE_ERROR;
}

hal_mem_result_t soc_flash_verify_erased(hal_mem_address_t address, size_t size)
{
    /* Check that all bytes in the specified range are at their erased value */
    const uint8_t *source = (const uint8_t*)address;
    for (size_t i = 0U; i < size; i++)
    {
        if (source[i] != FLASH_ERASE_VALUE)
        {
            return HAL_MEM_NOT_ERASED;
        }
    }

    return HAL_MEM_SUCCESS;
}

static void flash_lock(void)
{
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_DATA_CACHE_ENABLE();
    HAL_FLASH_Lock();
}

static inline void flash_unlock(void)
{
    HAL_FLASH_Unlock();
}
