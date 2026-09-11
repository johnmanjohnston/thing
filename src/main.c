#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "servo.h"
#include <hardware/pwm.h>
#include <stdio.h>

/*
#define SERVO_PIN 16

void set_ms(int servo_pin, float ms) {
	pwm_set_gpio_level(servo_pin, (ms / 9000.f) * 39062.f);
}

void set_servo(float cur_ms) {
	gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
	uint slice = pwm_gpio_to_slice_num(SERVO_PIN);

	pwm_config cfg = pwm_get_default_config();
	pwm_config_set_clkdiv(&cfg, 64.f);
	pwm_config_set_wrap(&cfg, 39062.f);

	pwm_init(slice, &cfg, true);
	set_ms(SERVO_PIN, cur_ms);
}*/

int main() {
	stdio_init_all();

	// Initialise the Wi-Fi chip
	if (cyw43_arch_init()) {
		printf("Wi-Fi init failed\n");
		return -1;
	}

	// Example to turn on the Pico W LED
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

	servo s;
	servo_init(&s, 16, 1);

	for (int i = 0; i <= 50; ++i) {
		servo_set(&s, i % 2);
		sleep_ms(128);

		if (i % 2 == 0)
			sleep_ms(2048);
	}

	servo_set_active(&s, 0);

	while (true) {
		/*
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
		sleep_ms(1000);
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
		sleep_ms(1000);*/

		sleep_ms(10);
	}
}
