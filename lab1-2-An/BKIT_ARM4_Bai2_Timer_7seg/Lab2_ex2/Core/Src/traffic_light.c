#include "main.h"
#include "traffic_light.h"
#include "led_7seg.h"

/* RED = LED3 (PE4), GREEN = Y0 (PE5), YELLOW = Y1 (PE6) */
void traffic_light_show(light_state_t s)
{
  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, s == LIGHT_RED);
  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, s == LIGHT_GREEN);
  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, s == LIGHT_YELLOW);
}

void updateLEDBuffer(int road_way, int num)
{
  switch (road_way){
    case 1:
      led7_SetDigit(num / 10, 0, 0);
      led7_SetDigit(num % 10, 1, 0);
      break;
    case 2:
      led7_SetDigit(num / 10, 2, 0);
      led7_SetDigit(num % 10, 3, 0);
      break;
    default:
      break;
  }
}
