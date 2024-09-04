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
#include "oem_bsp.h"
#include "soc_stm32xx.h"
#include "stm32f4xx_hal.h"

/* System Clock Configuration for STM32F4xx devices.
 *
 * The system Clock is configured as follows:
 *     System Clock source            = PLL (HSI)
 *     SYSCLK(Hz)                     = Configuration dependent.
 *     AHB Prescaler                  = 1
 *     APB1 Prescaler                 = 4
 *     APB2 Prescaler                 = 2
 *     HSI Frequency(Hz)              = 16000000
 *     PLL_M                          = OEM_CLOCK_RCC_PLLM
 *     PLL_N                          = OEM_CLOCK_RCC_PLLN
 *     PLL_R                          = OEM_CLOCK_RCC_PLLR
 *     PLL_P (optional)               = OEM_CLOCK_RCC_PLLP
 *     PLL_Q                          = OEM_CLOCK_RCC_PLLQ
 *     Flash Latency(WS)              = OEM_FLASH_LATENCY
 */
void SystemClock_Config(void)
{
    /* Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure. */
    RCC_OscInitTypeDef RCC_OscInitStruct =
    {
        .OscillatorType = RCC_OSCILLATORTYPE_HSI,
        .HSIState = RCC_HSI_ON,
        .HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
        .PLL =
        {
            .PLLState = RCC_PLL_ON,
            .PLLSource = RCC_PLLSOURCE_HSI,
            .PLLM = OEM_CLOCK_RCC_PLLM,
            .PLLN = OEM_CLOCK_RCC_PLLN,
            .PLLP = OEM_CLOCK_RCC_PLLP,
            .PLLQ = OEM_CLOCK_RCC_PLLQ,
#if defined(OEM_CLOCK_RCC_PLLR)
            .PLLR = OEM_CLOCK_RCC_PLLR
#endif
        }
    };

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitTypeDef RCC_ClkInitStruct =
    {
        .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                     | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV4,
        .APB2CLKDivider = RCC_HCLK_DIV2
    };

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, OEM_FLASH_LATENCY) != HAL_OK)
    {
        Error_Handler();
    }

#if defined(OEM_CLOCK_RNG_USES_CLK48) && OEM_CLOCK_RNG_USES_CLK48 != 0

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct =
    {
        .PeriphClockSelection = RCC_PERIPHCLK_CLK48,
        .Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ
    };

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
#endif

    __HAL_RCC_SYSCFG_CLK_ENABLE();
}
