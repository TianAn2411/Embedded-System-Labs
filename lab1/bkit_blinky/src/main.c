/*
 * BKIT ARM4 - Lab 1: Blinky voi Zephyr RTOS
 * Nhay LED3 (LED DEBUG, chan PE4): sang 1 giay, tat 1 giay.
 */
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* Thoi gian giua hai lan dao trang thai LED (ms) */
#define SLEEP_TIME_MS 1000

/* Lay node LED tu bi danh "led0" trong devicetree (app.overlay) */
#define LED0_NODE DT_ALIAS(led0)

/* gpio_dt_spec gom du thong tin: port (GPIOE), chan (4), co (ACTIVE_HIGH) */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int ret;
	bool led_state = true;

	/* Kiem tra driver GPIO cua port da san sang chua */
	if (!gpio_is_ready_dt(&led)) {
		printf("Loi: GPIO cua LED chua san sang\n");
		return 0;
	}

	/* Cau hinh chan la output, trang thai ban dau: LED sang */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printf("Loi: khong cau hinh duoc chan LED (%d)\n", ret);
		return 0;
	}

	printf("BKIT ARM4 - Zephyr Blinky bat dau!\n");

	while (1) {
		printf("LED3: %s\n", led_state ? "ON" : "OFF");

		k_msleep(SLEEP_TIME_MS);        /* Nhuong CPU trong 1000 ms */

		ret = gpio_pin_toggle_dt(&led); /* Dao trang thai LED */
		if (ret < 0) {
			return 0;
		}
		led_state = !led_state;
	}
	return 0;
}
