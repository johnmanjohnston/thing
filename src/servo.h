#pragma once

struct {
	unsigned int gpio;
	unsigned int slice;
	unsigned int invert_push_direction;
	float force_multiplier;
	float time_multiplier;
} typedef servo;

void servo_init(servo* s, unsigned int gpio, unsigned int invert_push_direction,
				float fmul, float tmul);
void servo_set(servo* s, unsigned int value);
void servo_set_active(servo* s, unsigned int active);

#define SERVO_DEFAULT_GPIO_LEVEL 1470
// #define SERVO_PUSHED_GPIO_DELTA 350
#define SERVO_PUSHED_GPIO_DELTA 650
