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

#ifndef OEM_IAR_STM32F429II_ACA_H_
#define OEM_IAR_STM32F429II_ACA_H_

#if !defined(OEM_IAR_STM32F429II_ACA)
#error "Invalid include."
#endif

/* Clock chain */
#define OEM_CLOCK_RCC_PLLM      16U
#define OEM_CLOCK_RCC_PLLN      360U
#define OEM_CLOCK_RCC_PLLP      RCC_PLLP_DIV2
#define OEM_CLOCK_RCC_PLLQ      8U
#define OEM_CLOCK_AHB_CLK_DIV   RCC_SYSCLK_DIV1
#define OEM_CLOCK_APB1_CLK_DIV  RCC_HCLK_DIV2
#define OEM_CLOCK_APB2_CLK_DIV  RCC_HCLK_DIV1
#define OEM_FLASH_LATENCY       FLASH_LATENCY_5

/* Board voltage range (used in flash driver) */
#define OEM_FLASH_VOLTAGE_RANGE          FLASH_VOLTAGE_RANGE_3

/* LEDs */
#define OEM_LED_ACTIVE_HIGH              0

#define OEM_LED_STARTUP_GPIO_PIN         GPIO_PIN_11
#define OEM_LED_STARTUP_GPIO_PORT        GPIOD
#define OEM_LED_STARTUP_GPIO_CLK_ENABLE  __HAL_RCC_GPIOD_CLK_ENABLE

#define OEM_LED_ERROR_GPIO_PIN           GPIO_PIN_7
#define OEM_LED_ERROR_GPIO_PORT          GPIOD
#define OEM_LED_ERROR_GPIO_CLK_ENABLE    __HAL_RCC_GPIOD_CLK_ENABLE

/* Console */
#define OEM_CONSOLE_USART                USART1
#define OEM_CONSOLE_USART_CLK_ENABLE     __HAL_RCC_USART1_CLK_ENABLE
#define OEM_CONSOLE_USART_CLK_DISABLE    __HAL_RCC_USART1_CLK_DISABLE
#define OEM_CONSOLE_USART_FORCE_RESET    __HAL_RCC_USART1_FORCE_RESET
#define OEM_CONSOLE_USART_RELEASE_RESET  __HAL_RCC_USART1_RELEASE_RESET

#define OEM_CONSOLE_TX_GPIO_CLK_ENABLE   __HAL_RCC_GPIOA_CLK_ENABLE
#define OEM_CONSOLE_TX_GPIO_PORT         GPIOA
#define OEM_CONSOLE_TX_GPIO_PIN          GPIO_PIN_9
#define OEM_CONSOLE_TX_GPIO_AF           GPIO_AF7_USART1

#define OEM_CONSOLE_RX_GPIO_CLK_ENABLE   __HAL_RCC_GPIOA_CLK_ENABLE
#define OEM_CONSOLE_RX_GPIO_PORT         GPIOA
#define OEM_CONSOLE_RX_GPIO_PIN          GPIO_PIN_10
#define OEM_CONSOLE_RX_GPIO_AF           GPIO_AF7_USART1

#define OEM_TARGET_STRING "IAR STM32F429II Academy"

#endif /* OEM_IAR_STM32F429II_ACA_H_ */
