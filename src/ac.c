#include "ac.h"
#include <pico/time.h>

servo s1;
servo s2;
servo s3;

void init_servos() {
	servo_init(&s1, 16, 1);
	servo_init(&s2, 15, 1);
	// servo_init(&s3, 16, 1);
}

void push_button(servo* s) {
	servo_set_active(s, 1);
	servo_set(s, 1);

	sleep_ms(128);

	servo_set(s, 0);
	sleep_ms(64);
	servo_set_active(s, 0);
}
