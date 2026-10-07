#ifndef __TRAFFIC_FSM_H__
#define __TRAFFIC_FSM_H__

#define TRAFFIC_TICK_MS 100   // period between traffic_fsm_run() calls

typedef enum { LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW } light_state_t;

/* Call once per tick: returns the state to display, advances when its time is up */
light_state_t traffic_fsm_run(void);

#endif /* __TRAFFIC_FSM_H__ */
