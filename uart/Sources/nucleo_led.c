#include "nucleo_led.h"

void nucleo_led_config(void)
{
	GPIO_InitTypeDef gpio_struct;
	TIM_TimeBaseInitTypeDef tim_timebase_struct;
	TIM_OCInitTypeDef tim_oc_struct;
//	NVIC_InitTypeDef nvic_struct;
	
	GPIO_RCC_CMD(LED_GPIO_RCC, ENABLE);
	LED_TIM_RCC_CMD(LED_TIM_RCC, ENABLE);
	
	gpio_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_struct.GPIO_OType = GPIO_OType_PP;
	gpio_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_struct.GPIO_Speed = GPIO_Speed_Level_1;
	gpio_struct.GPIO_Pin = LED_PIN;
	GPIO_Init(LED_GPIO, &gpio_struct);
	
	GPIO_PinAFConfig(LED_GPIO, LED_TIM_GPIO_PinSource, GPIO_AF_1);
	
//	nvic_struct.NVIC_IRQChannel = LED_TIM_IRQ;
//	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&nvic_struct);
	
	tim_timebase_struct.TIM_CounterMode = TIM_CounterMode_Up;
	tim_timebase_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_timebase_struct.TIM_Prescaler = LED_TIM_PSC;
	tim_timebase_struct.TIM_Period = LED_TIM_PERIOD;
	TIM_TimeBaseInit(LED_TIM, &tim_timebase_struct);
	
	tim_oc_struct.TIM_OCMode = TIM_OCMode_Toggle;
	tim_oc_struct.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc_struct.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc_struct.TIM_Pulse = LED_TIM_OC1_PULSE;
	TIM_OC1Init(LED_TIM, &tim_oc_struct);
	
	TIM_OC1PreloadConfig(LED_TIM, DISABLE);
	TIM_ARRPreloadConfig(LED_TIM, DISABLE);
	
	TIM_Cmd(LED_TIM, ENABLE);
	
//	TIM_ITConfig(LED_TIM, TIM_IT_Update, ENABLE);
}
