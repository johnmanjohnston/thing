#include "ac.h"
#include <pico/time.h>

servo s1;
servo s2;
servo s3;

void init_servos() {
	servo_init(&s1, 16, 1, 1.f);
	servo_init(&s2, 15, 1, 1.2f);
	servo_init(&s3, 17, 1, 1.2f);
}

void push_button(servo* s) {
	servo_set_active(s, 1);
	servo_set(s, 1);

	sleep_ms(150);

	servo_set(s, 0);
	sleep_ms(64);
	servo_set_active(s, 0);
}
