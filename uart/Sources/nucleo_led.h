#ifndef __NUCLEO_LED_H
#define __NUCLEO_LED_H

#include "main.h"

#define LED_GPIO				GPIOA
#define LED_PIN					GPIO_Pin_5
#define LED_GPIO_RCC			RCC_AHBPeriph_GPIOA
#define LED_TIM					TIM2
#define LED_TIM_RCC				RCC_APB1Periph_TIM2
#define LED_TIM_RCC_CMD			RCC_APB1PeriphClockCmd
#define LED_TIM_PSC				36000-1
#define LED_TIM_PERIOD			1000-1
#define LED_TIM_OC1_PULSE		0
#define LED_TIM_GPIO_PinSource	GPIO_PinSource5
#define LED_TIM_IRQ				TIM2_IRQn
#define LED_TIM_IRQ_HANDLER		TIM2_IRQHandler

#define GPIO_ToggleBits(GPIOx, GPIO_Pin)	GPIOx->ODR ^= GPIO_Pin

#define LED_ON					GPIO_SetBits(LED_GPIO, LED_PIN)
#define LED_OFF					GPIO_ResetBits(LED_GPIO, LED_PIN)
#define LED_TOGGLE				GPIO_ToggleBits(LED_GPIO, LED_PIN)

void nucleo_led_config(void);

#endif
