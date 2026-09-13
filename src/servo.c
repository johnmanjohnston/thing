#include "servo.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <hardware/pwm.h>

void servo_init(servo* s, unsigned int gpio, unsigned int invert_push_direction,
				float fmul, float tmul) {
	s->gpio = gpio;
	s->invert_push_direction = invert_push_direction;
	s->force_multiplier = fmul;
	s->time_multiplier = tmul;
	gpio_set_function(s->gpio, GPIO_FUNC_PWM);

	s->slice = pwm_gpio_to_slice_num(s->gpio);

	pwm_config cfg = pwm_get_default_config();
	pwm_config_set_clkdiv(&cfg, 64.f);
	pwm_config_set_wrap(&cfg, 39062.f);

	pwm_init(s->slice, &cfg, 0);
	servo_set_active(s, 1);
}

void servo_set(servo* s, unsigned int value) {
	// either pushes a physical AC button on or off; no angular control
	if (value == 0) {
		pwm_set_gpio_level(s->gpio, SERVO_DEFAULT_GPIO_LEVEL);
	} else if (value == 1) {
		uint16_t level = SERVO_DEFAULT_GPIO_LEVEL;
		level = level + (s->invert_push_direction ? SERVO_PUSHED_GPIO_DELTA
												  : -SERVO_PUSHED_GPIO_DELTA);
		level *= s->force_multiplier;
		pwm_set_gpio_level(s->gpio, level);
	}
}

void servo_set_active(servo* s, unsigned int active) {
	pwm_set_enabled(s->slice, active);
}
