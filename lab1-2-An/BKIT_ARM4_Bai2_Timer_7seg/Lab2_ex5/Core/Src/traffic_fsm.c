#include "traffic_fsm.h"
#include "software_timer.h"

/* Transition table: state -> next state, how long it lasts (ms) */
static const struct { light_state_t next; int ms; } fsm[] = {
  [LIGHT_RED]    = { LIGHT_GREEN,  5000 },
  [LIGHT_GREEN]  = { LIGHT_YELLOW, 3000 },
  [LIGHT_YELLOW] = { LIGHT_RED,    1000 },
};

static light_state_t state = LIGHT_RED;

void traffic_fsm_init(void)
{
  state = LIGHT_RED;
  setTimer(TIMER_TRAFFIC, fsm[state].ms);
}

light_state_t traffic_fsm_run(void)
{
  if (isExpired(TIMER_TRAFFIC)) { 
    state = fsm[state].next; 
    setTimer(TIMER_TRAFFIC, fsm[state].ms); 
  }
  return state;
}

int traffic_fsm_seconds_left(void)
{
  return (timer_counter[TIMER_TRAFFIC] + 999) / 1000;
}
