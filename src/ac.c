#include "ac.h"
#include <pico/time.h>

servo s1;
servo s2;
servo s3;

void init_servos() {
	servo_init(&s1, 16, 1, 1.f, 1.f);
	servo_init(&s2, 15, 1, 1.1f, 1.2f);
	servo_init(&s3, 17, 1, 1.f, 1.f);
}

void push_button(servo* s) {
	servo_set_active(s, 1);
	servo_set(s, 1);

	sleep_ms(170 * s->time_multiplier);

	servo_set(s, 0);
	sleep_ms(32);
	servo_set_active(s, 0);
}
