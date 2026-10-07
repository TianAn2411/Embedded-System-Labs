#ifndef __TRAFFIC_FSM_H__
#define __TRAFFIC_FSM_H__

#define TIMER_TRAFFIC 0   // software timer id used by the FSM

typedef enum { LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW } light_state_t;

void traffic_fsm_init(void);
/* Non-blocking: returns the state to display, advances when its software timer expires */
light_state_t traffic_fsm_run(void);
/* Seconds left before the current state ends (rounded up) */
int traffic_fsm_seconds_left(void);

#endif /* __TRAFFIC_FSM_H__ */
