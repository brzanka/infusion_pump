/*
 * stepper.c
 *
 *  Created on: Nov 11, 2023
 *      Author: 31415
 */
#include "stepper.h"
#include "stdlib.h"
#include <string.h>

int calc_steps (int doseinml) {
	int steps = (((doseinml * 1000) / area) / linear_step);
	return steps;
}

void set_speed (float mlforsec) {
	float mmforsec = mlforsec * 1000 / area;
	int delayms = (1000 * linear_step) / mmforsec;
	HAL_Delay(delayms);
}

void stepping (int step)
{
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
	int numberofsteps = calc_steps(doseinml);
	int numberofsequences = numberofsteps / 4;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
	if (direction == 0)  // for clockwise
	{
		for (int seq = 0; seq < numberofsequences; seq ++)
		{
			for (int step = 3; step >= 0; step --)
			{
				stepping(step);
				set_speed(mlforsec);
			}
			//displaying the progress value on the screen from 0 to 100%
			lcd_put_cur(1,0);
			int progress = (float)seq/(float)numberofsequences * 100;
			if(progress % 5 == 0) {
			char buffer[3];
			itoa(progress, buffer,10);
			lcd_send_string(buffer);
			lcd_send_string("%");
			}
		}
	}

	else if (direction == 1)  // for anti-clockwise
	{
		for (int seq = 0; seq < numberofsequences; seq++)
		{
			for (int step = 0; step <= 3; step++)
			{
				stepping(step);
				set_speed(mlforsec);
			}
			lcd_put_cur(1,0);
			lcd_send_string("Return...");

		}
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
}
