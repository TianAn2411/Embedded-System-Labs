/*
 * Lab 1 - Bai 1: nhay LED3, sang 1 s, tat 2 s
 * Thread tinh (K_THREAD_DEFINE), main() khong dieu khien LED.
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED_ON_MS         1000
#define LED_OFF_MS        2000
#define THREAD_STACK_SIZE 1024
#define THREAD_PRIO       7

/* led0 -> debug_led -> GPIOE pin 4 (LED3), xem app.overlay */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

void thread_led_blinky(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	if (!gpio_is_ready_dt(&led)) {
		return;
	}

	/* Bat dau o trang thai tat, vong lap dau se bat ro rang */
	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
		return;
	}

	while (1) {
		gpio_pin_set_dt(&led, 1);   /* sang */
		k_msleep(LED_ON_MS);
		gpio_pin_set_dt(&led, 0);   /* tat */
		k_msleep(LED_OFF_MS);
	}
}

/* Tao thread luc bien dich, delay 0 = chay ngay khi he thong khoi dong */
K_THREAD_DEFINE(led_tid, THREAD_STACK_SIZE, thread_led_blinky,
		NULL, NULL, NULL, THREAD_PRIO, 0, 0);

int main(void)
{
	return 0;
}
