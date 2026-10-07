#ifndef __TRAFFIC_LIGHT_H__
#define __TRAFFIC_LIGHT_H__

#include "traffic_fsm.h"

/* Turn on the LED of state s, turn off the others */
void traffic_light_show(light_state_t s);

#endif /* __TRAFFIC_LIGHT_H__ */
