#include "i2c-lcd.h"
#include "menu2.h"
#include "stdlib.h"
#include <string.h>
#include "stepper.h"

GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
uint32_t previousMillis = 0;
uint32_t currentMillis = 0;
uint8_t keyPressed = 0;

typedef struct mainMenu //struct of the element in the list
{
	const char * text;
	struct mainMenu * next;
	struct mainMenu * previous;
	struct mainMenu * parent;
	struct mainMenu * child;
	uint8_t dose;
	float velocity;
	void (*menu_function)(void);
} mainMenu;

mainMenu select;
mainMenu newM;
mainMenu run;
mainMenu program1;
mainMenu program2;
mainMenu program3;
mainMenu program4;
mainMenu program5;
mainMenu dose1;
mainMenu dose2;
mainMenu dose3;
mainMenu dose4;
mainMenu dose5;
mainMenu velocity1;
mainMenu velocity2;
mainMenu velocity3;
mainMenu velocity4;
mainMenu velocity5;
mainMenu end;

//menu elements displayed on the screen
mainMenu select = {"SELECT", &newM, &newM, NULL, &program1, 0, 0, stepInto};
mainMenu newM = {"NEW", &select, &select, NULL, &dose1, 0, 0, stepInto};
mainMenu program1 = {"D:1ml 0.8ml/s", &program2, &program5, &select, &run, 1, 0.8, setParameters};
mainMenu program2 = {"D:2ml 0.6ml/s", &program3, &program1, &select, &run, 2, 0.6, setParameters};
mainMenu program3 = {"D:3ml 0.4ml/s", &program4, &program2, &select, &run, 3, 0.4, setParameters};
mainMenu program4 = {"D:4ml 0.2ml/s", &program5, &program3, &select, &run, 4, 0.2, setParameters};
mainMenu program5 = {"D:5ml 0.1ml/s", &program1, &program4, &select, &run, 5, 0.1, setParameters};
mainMenu dose1 = 	{"D:1ml", &dose2, &dose5, &newM, &velocity1, 1, 0, setParameters};
mainMenu dose2 =  	{"D:2ml", &dose3, &dose1, &newM, &velocity1, 2, 0, setParameters};
mainMenu dose3 = 	{"D:3ml", &dose4, &dose2, &newM, &velocity1, 3, 0, setParameters};
mainMenu dose4 =  	{"D:4ml", &dose5, &dose3, &newM, &velocity1, 4, 0, setParameters};
mainMenu dose5 = 	{"D:5ml", &dose1, &dose4, &newM, &velocity1, 5, 0, setParameters};
mainMenu velocity1 = { "V:0.1ml/s", &velocity2, &velocity5, &dose1, &run, 0, 0.1, setParameters};
mainMenu velocity2 = { "V:0.2ml/s", &velocity3, &velocity1, &dose1, &run, 0, 0.2, setParameters};
mainMenu velocity3 = { "V:0.4ml/s", &velocity4, &velocity2, &dose1, &run, 0, 0.4, setParameters};
mainMenu velocity4 = { "V:0.6ml/s", &velocity5, &velocity3, &dose1, &run, 0, 0.6, setParameters};
mainMenu velocity5 = { "V:0.8ml/s", &velocity1, &velocity4, &dose1, &run, 0, 0.8, setParameters};
mainMenu run = {"D: ml V:   ml/s", NULL, NULL, &select, &select, 0, 0, start};
mainMenu end = {"Done", &select, &select, &select, &select, 0, 0, restart};

mainMenu *currentMenu = &select;
uint8_t curIndex = 0;
uint8_t flag = 0;
uint8_t actionFlag = 0;
char number[1];

void menuInit(void) //display first menu
{
	lcd_clear();
	lcd_put_cur(0,0);
	lcd_send_data('>'); //shows which line will be chosen if key SELECT pressed
	lcd_put_cur(0,1);
	lcd_send_string(select.text);
	lcd_put_cur(1,1);
	lcd_send_string(newM.text);
}

void displayCur(void) //displays the current 2-lines menu
{
		lcd_clear();
		lcd_put_cur(0,0);
		lcd_send_data('>');
		lcd_put_cur(curIndex % 2,1);
		lcd_send_string(currentMenu->text);
		lcd_put_cur((curIndex+1) % 2,1);
		if(currentMenu->next!=NULL){
			lcd_send_string(currentMenu->next->text);
		}else{
			lcd_put_cur(0,3);
			itoa(run.dose,number,10);
			lcd_send_string(number);
			lcd_put_cur(0,9);
			sprintf(number, "%.1f", run.velocity);
			//itoa(run.velocity,number,10);
			lcd_send_string(number);
			lcd_put_cur(1,0);
			lcd_send_string("Press button");
		}
}
void stepInto(void) //enter into sub-menu
{
	if(currentMenu->child!=NULL)
	{
		curIndex = 0;
		currentMenu = currentMenu->child;
		displayCur();
	}
}

void menu(void) //check which key was pressed and move in menu
{
	if(actionFlag != 0)
	{
		if(flag == 1)
		{
			if(currentMenu->next!=NULL)
			{
			currentMenu = currentMenu->next;
			displayCur();
			}
		}
		else if(flag == 2)
		{
			if(currentMenu->previous!=NULL)
			{
			currentMenu = currentMenu->previous;
			displayCur();
			}
		}
		else if(flag == 3)
		{
			currentMenu ->menu_function();
		}
		else if(flag == 4)
		{
			if(currentMenu->parent!=NULL)
			{
			currentMenu = currentMenu->parent;
			displayCur();
			}
		}
	}
	actionFlag = 0;
}

void setParameters(void) //take the velocity and dose values from chosen menu
{
	if(currentMenu->velocity != 0)
	{
		run.velocity = currentMenu->velocity;
	}
	if(currentMenu->dose != 0)
	{
		run.dose = currentMenu->dose;
	}
	currentMenu = currentMenu->child;
	curIndex = 0;
	displayCur();
}

void start(void)
{
	//move the motor by the right angle with right speed
	lcd_clear();
	lcd_put_cur(0,0);
	step_distance(currentMenu->dose, 0, currentMenu->velocity);

    //come back to the previous position
    step_distance(currentMenu->dose, 1, currentMenu->velocity); //zmien predkosc na stala
	currentMenu = &end;
	lastDisplay();
}
void restart(void)
{
	currentMenu = &select;
	curIndex = 0;
	displayCur();
}
void lastDisplay(void)
{
	lcd_clear();
	lcd_put_cur(0,6);
	lcd_send_string(currentMenu->text);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //interrupts from keyboard
{
  currentMillis = HAL_GetTick();
  if (currentMillis - previousMillis > 10)
  {
	    /*Configure GPIO pins : PRZYCISK_1_Pin PRZYCISK_4_Pin PRZYCISK_3_Pin PRZYCISK_2_Pin */
	  GPIO_InitStructPrivate.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_1;
	  GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStructPrivate);

	actionFlag=1;

    if(GPIO_Pin == GPIO_PIN_1)
    {
    	flag = 1;
    }
    else if(GPIO_Pin == GPIO_PIN_15)
    {
    	flag = 2;
    }
    else if(GPIO_Pin == GPIO_PIN_14)
    {
    	flag = 3;
    }
    else if(GPIO_Pin == GPIO_PIN_13)
    {
    	flag = 4;
    }

    /*Configure GPIO pins : PB3 PB5 PB4 back to EXTI*/
    GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructPrivate);
    previousMillis = currentMillis;
  }
}
