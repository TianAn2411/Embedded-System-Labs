#include "main.h"
#include "traffic_light.h"

/* RED = LED3 (PE4), GREEN = Y0 (PE5), YELLOW = Y1 (PE6) */
void traffic_light_show(light_state_t s)
{
  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, s == LIGHT_RED);
  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, s == LIGHT_GREEN);
  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, s == LIGHT_YELLOW);
}
