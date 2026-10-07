/*
 * software_timer.h
 *
 *  Created on: Oct 8, 2025
 *      Author: Truong Thien An
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define MAX_TIMER 20

extern int timer_flag[MAX_TIMER];
extern int timer_counter[MAX_TIMER];

void timer_init();
void setTimer(int timer_id, int dur);
int isExpired(int timer_id);
void timerRun();


#endif /* INC_SOFTWARE_TIMER_H_ */
