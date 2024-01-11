//#define PRZYCISK_4 			GPIO_PIN_3
//#define PRZYCISK_3			GPIO_PIN_4
//#define PRZYCISK_2			GPIO_PIN_5
//#define PRZYCISK_1			GPIO_PIN_10
#define LCD_ROWS			2
#define LCD_COLS			16

extern void RTC_ReadDateTime(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);
extern void UART_Transmit(uint8_t* data, uint16_t len);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void displayMenu(void);
void displayCur(void);
void menu(void);
void stepInto(void);
void setParameters(void);
void menuInit(void);
void start(void);
void restart(void);
void lastDisplay(void);
