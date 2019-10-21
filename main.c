/**
 * @project TempSensor
 * @author Robert Gałat (robgal519@gmail.com)
 * @author Gabriel Górski (glaeqen@gmail.com)
 * @brief Measure temperature & pressure, then display on LCD screen
 * @version 0.1
 * @date 2019-10-14
 * 
 * @copyright Copyright (c) 2019
 */

#include "I2C/I2C.h"
#include "OLED/OLED.h"
#include "Drawer/Drawer.h"
#include "Fonts/consolas18.h"
#include "Sensor/BMP280.h"
#include <stm32f10x_tim.h>
#include <stdio.h>

static bool run_loop = false;

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		run_loop = true;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

void TIM4_setup(void){
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER_InitStructure.TIM_Prescaler = 8000;


	TIMER_InitStructure.TIM_Period = 15000;
	TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	/* NVIC Configuration */
	/* Enable the TIM4_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

int main()
{
	i2c_init();
	TIM4_setup();

	OLED *oled = oled_create();

	BMP280_initConnection();
	BMP280_Config cfg = {.osrs_t = x2, .osrs_p = x16, .mode = Forced};
         

	Framebuffer fb = oled_access_framebuffer(oled);
	char buffer[10] = {0};
	while(true){
		if(!run_loop) continue;
		oled_clear_framebuffer(oled);
		
		BMP280_updateConfiguration(cfg);  
		BMP280_readMeasurements();
		float temperature = BMP280_ExtractTemperature();
		float preassure = BMP280_ExtractPressure();

		sprintf(buffer, "%.2f C ", temperature);
	
		TextDrawable text_drawable = {
			.text = buffer,
			.font_info = consolas_18pt,
			.offset_x = 0,
			.offset_y = 5,
		};

		drawer_draw_text(fb, text_drawable);
		
		sprintf(buffer, "%.0f hPa", preassure/100);
		
		TextDrawable text_drawable2 = {
			.text = buffer,
			.font_info = consolas_18pt,
			.offset_x = 0,
			.offset_y = 5 + 24,
		};
		
		drawer_draw_text(fb, text_drawable2);
		
		oled_send_framebuffer(oled);
		run_loop = false;
	}
}
