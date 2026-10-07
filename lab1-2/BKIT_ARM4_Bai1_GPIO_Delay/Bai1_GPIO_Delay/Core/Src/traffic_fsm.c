#include <stdint.h>
#include "traffic_fsm.h"

/* Transition table: state -> next state, how long it lasts */
static const struct { light_state_t next; uint32_t ms; } fsm[] = {
  [LIGHT_RED]    = { LIGHT_GREEN,  5000 },
  [LIGHT_GREEN]  = { LIGHT_YELLOW, 3000 },
  [LIGHT_YELLOW] = { LIGHT_RED,    1000 },
};

static light_state_t state = LIGHT_RED;
static uint32_t ticks = 0;

light_state_t traffic_fsm_run(void)
{
  light_state_t current = state;
  if (++ticks >= fsm[state].ms / TRAFFIC_TICK_MS) {
      ticks = 0; 
      state = fsm[state].next; 
    }
  return current;
}
