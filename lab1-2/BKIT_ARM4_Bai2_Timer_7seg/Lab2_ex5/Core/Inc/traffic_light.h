#ifndef __TRAFFIC_LIGHT_H__
#define __TRAFFIC_LIGHT_H__

#include "traffic_fsm.h"

/* Turn on the LED of state s, turn off the others */
void traffic_light_show(light_state_t s);
/* Show a 2-digit number on road_way 1 (digits 0,1) or road_way 2 (digits 2,3) */
void updateLEDBuffer(int road_way, int num);

#endif /* __TRAFFIC_LIGHT_H__ */
