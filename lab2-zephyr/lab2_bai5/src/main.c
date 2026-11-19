/* BKIT ARM4 - Lab 2 - Bai 5: traffic light with pedestrian request */
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define RED_MS        7000
#define GREEN_MS      5000
#define GREEN_MIN_MS  2000
#define YELLOW_MS     2000

static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec y0   = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);  /* red */
static const struct gpio_dt_spec y1   = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);  /* green */

enum phase_id { PH_RED, PH_GREEN, PH_YELLOW };
enum evt_type { EVT_PHASE, EVT_REQUEST, EVT_SHORTEN };

struct evt {
	uint8_t  type;
	uint8_t  phase;
	uint32_t t_ms;
};

K_MSGQ_DEFINE(log_q, sizeof(struct evt), 16, 4);
K_SEM_DEFINE(ped_sem, 0, 1);

static volatile uint8_t cur_phase = PH_RED;

static void log_evt(uint8_t type, uint8_t phase)
{
	struct evt e = { .type = type, .phase = phase, .t_ms = k_uptime_get_32() };

	k_msgq_put(&log_q, &e, K_NO_WAIT);
}

static void ped_isr(struct k_timer *timer)
{
	static const uint8_t gap_s[] = { 14, 8, 17 };
	static uint8_t next;

	k_sem_give(&ped_sem);
	log_evt(EVT_REQUEST, cur_phase);
	k_timer_start(timer, K_SECONDS(gap_s[next]), K_NO_WAIT);
	next = (next + 1) % 3;
}
K_TIMER_DEFINE(ped_timer, ped_isr, NULL);

static void set_phase(uint8_t phase)
{
	cur_phase = phase;
	gpio_pin_set_dt(&y0, phase != PH_GREEN);
	gpio_pin_set_dt(&y1, phase != PH_RED);
	log_evt(EVT_PHASE, phase);
}

void thread_traffic_light(void *p1, void *p2, void *p3)
{
	if (gpio_pin_configure_dt(&y0, GPIO_OUTPUT_INACTIVE) < 0 ||
	    gpio_pin_configure_dt(&y1, GPIO_OUTPUT_INACTIVE) < 0) {
		return;
	}

	while (1) {
		set_phase(PH_RED);
		k_msleep(RED_MS);

		set_phase(PH_GREEN);
		int64_t green_start = k_uptime_get();
		k_msleep(GREEN_MIN_MS);

		/* presses made before now do not count, otherwise green is cut for nothing */
		k_sem_reset(&ped_sem);

		int64_t left = GREEN_MS - (k_uptime_get() - green_start);

		if (k_sem_take(&ped_sem, K_MSEC(left)) == 0) {
			log_evt(EVT_SHORTEN, PH_GREEN);
		}

		set_phase(PH_YELLOW);
		k_msleep(YELLOW_MS);
	}
}

void thread_led_blinky(void *p1, void *p2, void *p3)
{
	if (gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE) < 0) {
		return;
	}

	while (1) {
		gpio_pin_set_dt(&led3, 1);
		k_msleep(1000);
		gpio_pin_set_dt(&led3, 0);
		k_msleep(1000);
	}
}

void thread_logger(void *p1, void *p2, void *p3)
{
	static const char *const phase_name[] = { "DO", "XANH", "VANG" };
	struct evt e;

	while (1) {
		k_msgq_get(&log_q, &e, K_FOREVER);

		switch (e.type) {
		case EVT_PHASE:
			printf("[%6u ms] DOI PHA -> %s\n", e.t_ms, phase_name[e.phase]);
			break;
		case EVT_REQUEST:
			printf("[%6u ms] YEU CAU qua duong (dang pha %s)\n",
			       e.t_ms, phase_name[e.phase]);
			break;
		case EVT_SHORTEN:
			printf("[%6u ms] RUT NGAN pha XANH -> sang VANG\n", e.t_ms);
			break;
		}
	}
}

K_THREAD_DEFINE(traffic_tid, 1024, thread_traffic_light, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(blinky_tid,  1024, thread_led_blinky,    NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(logger_tid,  2048, thread_logger,        NULL, NULL, NULL, 10, 0, 0);

int main(void)
{
	k_timer_start(&ped_timer, K_SECONDS(10), K_NO_WAIT);
	return 0;
}
