/*
 * software_timer.c
 *
 *  Created on: Oct 8, 2025
 *      Author: Truong Thien An
 */


#include "software_timer.h"
#include "tim.h"
#include "led_7seg.h"

int timer_flag[MAX_TIMER];
int timer_counter[MAX_TIMER];

void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}

void setTimer(int timer_id, int dur){
	if (timer_id < 0 || timer_id >= MAX_TIMER){
		return;
	}
	timer_counter[timer_id] = dur;
	timer_flag[timer_id] = 0;
}

int isExpired(int timer_id){
	if (timer_flag[timer_id] == 1){
		return 1;
	}
	return 0;
}

void timerRun(){
	for (int i = 0; i < MAX_TIMER; i++){
		if (timer_counter[i] > 0){
			timer_counter[i]--;
		}
		if (timer_counter[i] == 0){
			timer_flag[i] = 1;
		}
	}
}

// TIM2 interrupt every 1ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){
		timerRun();
		led7_Scan();
	}
}
