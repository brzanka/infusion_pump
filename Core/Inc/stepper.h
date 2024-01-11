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
#define area				80 //mm^2


int calc_distance (int doseinml);
int calc_speed (int milforsec);
void set_speed (int milforsec);
void stepping (int step);
void step_distance(int doseinml, int direction, float mlforsec);
//42 mm - max distance
//27 cm - length of rail
//1.5 cm - tube for a syringe diameter. length around 3 cm or more
//zmniejszyc nakladke na srube do 3 mm musi byc ciasno i dodac podpore na strzykawke

#endif /* INC_STEPPER_H_ */
