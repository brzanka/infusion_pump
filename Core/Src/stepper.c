/*
 * stepper.c
 *
 *  Created on: Nov 11, 2023
 *      Author: 31415
 */
#include "stepper.h"

int calc_distance (int doseinml) {
	int distance = doseinml * 1000 / area; //way to go in mm
	return distance;
}

int calc_speed (int milforsec) {
	int mmforsec = milforsec / 1000;
	return mmforsec;
}

void set_speed (int milforsec) { //speed in mm/s
	int speed = calc_speed (milforsec);
	int delay = 1000/linear_step/speed; //delay in millis
	HAL_Delay(delay);
}

void stepping (int step)
{
// IN4 - pin pA0
// IN3 - pin pA1
// IN2 - pin pA4
// IN1 - pin PB0
		switch (step)
		{
		case 0:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			break;

				case 1:
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
					break;

				case 2:
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
					break;

				case 3:
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
					break;
				}
}

void step_distance(int doseinml, int direction, float mlforsec) {
	int numberofsteps = calc_distance(doseinml) / linear_step;
	int numberofsequences = numberofsteps / 3;

	if (direction == 0)  // for clockwise
	{
		for (int seq = 0; seq < numberofsequences; seq ++)
		{
			for (int step = 2; step >= 0; step --)
			{
				stepping(step);
				set_speed(mlforsec);
			}
			//displaying the progress value on the screen from 0 to 100%
			lcd_put_cur(0,0);
			int progress = (float)seq/(float)numberofsequences * 100;
			if(progress % 5 == 0) {
			char buffer[3];
			itoa(progress, buffer,10); //sending progress value to char buffer
			lcd_send_string(buffer);
			lcd_send_string("%");
			}
		}
	}

	else if (direction == 1)  // for anti-clockwise
	{
		for (int seq=0; seq<numberofsequences; seq++)
		{
			for (int step=0; step<3; step++)
			{
				stepping(step);
				set_speed(milforsec);
			}
			lcd_put_cur(0,0);
			lcd_send_string("Return...");

		}
	}
}
