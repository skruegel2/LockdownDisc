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

#include "soc_arm_cmsis.h"
#include "soc_stm32xx.h"

void soc_stm32xx_init(void)
{
    /* Nothing to do here. */
}

void soc_stm32xx_quiesce(void)
{
    /* Disable all GPIO clocks. */
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
#ifdef GPIOD
    __HAL_RCC_GPIOD_CLK_DISABLE();
#endif
#ifdef GPIOE
    __HAL_RCC_GPIOE_CLK_DISABLE();
#endif
#ifdef GPIOF
    __HAL_RCC_GPIOF_CLK_DISABLE();
#endif
#ifdef GPIOG
    __HAL_RCC_GPIOG_CLK_DISABLE();
#endif
    __HAL_RCC_GPIOH_CLK_DISABLE();

    /* Reset the RCC clock configuration to the default reset state */
    HAL_RCC_DeInit();

    /* Disable the SysTick timer again */
    NVIC_DisableIRQ(SysTick_IRQn);
    SysTick->CTRL = 0u;
    NVIC_ClearPendingIRQ(SysTick_IRQn);
}
