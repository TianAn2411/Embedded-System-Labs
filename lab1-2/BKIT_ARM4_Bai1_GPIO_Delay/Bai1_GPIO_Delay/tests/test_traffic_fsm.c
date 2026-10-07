// Host check: gcc -ICore/Inc tests/test_traffic_fsm.c Core/Src/traffic_fsm.c && ./a.out
#include <assert.h>
#include <stdio.h>
#include "traffic_fsm.h"

int main(void)
{
  const struct { light_state_t s; int ms; } expect[] = {
    { LIGHT_RED, 5000 }, { LIGHT_GREEN, 3000 }, { LIGHT_YELLOW, 1000 }, { LIGHT_RED, 5000 },
  };
  for (int i = 0; i < 4; i++)
    for (int t = 0; t < expect[i].ms / TRAFFIC_TICK_MS; t++)
      assert(traffic_fsm_run() == expect[i].s);
  puts("traffic_fsm OK");
  return 0;
}
