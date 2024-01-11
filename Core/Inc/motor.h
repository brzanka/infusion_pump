#include "stm32l0xx_hal.h"

extern void delayms(uint32_t ms);
void stepper_set_rpm (int rpm);
void stepper_half_drive (int step);
void stepper_step_angle (float angle, int direction, int rpm);
