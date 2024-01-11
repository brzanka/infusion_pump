/*
 * stepper.h
 *
 *  Created on: Nov 11, 2023
 *      Author: 31415
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_
#include "stm32l0xx_hal.h"
#include "stdlib.h"
#include "i2c-lcd.h"

#define stepsperrev			48
#define linear_step			0.0254
#define distance_per_rev	1.22
#define area				80.0 //mm^2

int calc_steps (int doseinml);
void set_speed (int milforsec);
void stepping (int step);
void step_distance(int doseinml, int direction, float mlforsec);
//42 mm - max distance

#endif /* INC_STEPPER_H_ */
