#include "nucleo_uart.h"

void nucleo_uart_config(void)
{
	// structure define
	USART_InitTypeDef usart_struct;
	GPIO_InitTypeDef gpio_struct;
	TIM_TimeBaseInitTypeDef tim_timebase_struct;
	NVIC_InitTypeDef nvic_struct;
	EXTI_InitTypeDef exti_struct;
	
	// clock enable
	GPIO_RCC_CMD(DEBUG_UART_RCC, ENABLE);
	GPIO_RCC_CMD(PUSHBUTTON_RCC, ENABLE);
	DEBUG_UART_RCC_CMD(DEBUG_UART_RCC, ENABLE);
	DEBUG_UART_TX_TIM_RCC_CMD(DEBUG_UART_TX_TIM_RCC, ENABLE);
	
	// enable SYSCFG clock
	SYSCFG_RCC_CMD(SYSCFG_RCC, ENABLE);
	
	// GPIO
	// USART GPIO
	gpio_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_struct.GPIO_OType = GPIO_OType_PP;
	gpio_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_struct.GPIO_Speed = GPIO_Speed_Level_1;
	gpio_struct.GPIO_Pin = DEBUG_UART_TX | DEBUG_UART_RX;
	GPIO_Init(DEBUG_UART_GPIO, &gpio_struct);
	
	GPIO_PinAFConfig(DEBUG_UART_GPIO, DEBUG_UART_TX_PINSOURCE, GPIO_AF_7);
	GPIO_PinAFConfig(DEBUG_UART_GPIO, DEBUG_UART_RX_PINSOURCE, GPIO_AF_7);
	
	// Push button GPIO
	gpio_struct.GPIO_Mode = GPIO_Mode_IN;
	gpio_struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio_struct.GPIO_Speed = GPIO_Speed_Level_1;
	gpio_struct.GPIO_Pin = PUSHBUTTON_PIN;
	GPIO_Init(PUSHBUTTON_GPIO, &gpio_struct);
	
	SYSCFG_EXTILineConfig(PUSHBUTTON_EXTI_PORTSOURCE, PUSHBUTTON_EXTI_PINSOURCE);
	
	// EXTI
	exti_struct.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_struct.EXTI_Line = PUSHBUTTON_EXTI_LINE;
	exti_struct.EXTI_LineCmd = ENABLE;
	exti_struct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&exti_struct);
	
	// NVIC
	// priority group 4: 16 primary priorities and 1 subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	// 1s TIM interrupt
	nvic_struct.NVIC_IRQChannel = DEBUG_UART_TX_TIM_IRQ;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 15;
	nvic_struct.NVIC_IRQChannelSubPriority = 0;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_struct);
	
	// push button interrupt
	nvic_struct.NVIC_IRQChannel = PUSHBUTTON_IRQ;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 15;
	nvic_struct.NVIC_IRQChannelSubPriority = 0;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_struct);
	
	// USART RX interrupt
	nvic_struct.NVIC_IRQChannel = DEBUG_UART_IRQ;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_struct.NVIC_IRQChannelSubPriority = 0;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_struct);
	
	// TIM
	tim_timebase_struct.TIM_CounterMode = TIM_CounterMode_Up;
	tim_timebase_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_timebase_struct.TIM_Prescaler = DEBUG_UART_TX_TIM_PSC;
	tim_timebase_struct.TIM_Period = DEBUG_UART_TX_TIM_PERIOD;
	TIM_TimeBaseInit(DEBUG_UART_TX_TIM, &tim_timebase_struct);
	
	TIM_Cmd(DEBUG_UART_TX_TIM, ENABLE);
	
	TIM_ITConfig(DEBUG_UART_TX_TIM, TIM_IT_Update, ENABLE);
	
	// USART
	usart_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_struct.USART_Parity = USART_Parity_No;
	usart_struct.USART_StopBits = USART_StopBits_1;
	usart_struct.USART_WordLength = USART_WordLength_8b;
	usart_struct.USART_BaudRate = DEBUG_UART_BAUDRATE;
	USART_Init(DEBUG_UART, &usart_struct);
	
	USART_Cmd(DEBUG_UART, ENABLE);
	
	USART_ITConfig(DEBUG_UART, USART_IT_RXNE, ENABLE);
}

int fputc(int ch, FILE* stream)
{
	while(USART_GetFlagStatus(DEBUG_UART, USART_FLAG_TXE) == RESET);
	
	USART_SendData(DEBUG_UART, ch);
	
	return ch;
}
