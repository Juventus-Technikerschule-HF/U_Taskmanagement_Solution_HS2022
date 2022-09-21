/*
 * U_Taskmanagement_Solution.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : chaos
 */ 

//#include <avr/io.h>
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"

#include "mem_check.h"

#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"

#include "ButtonHandler.h"


extern void vApplicationIdleHook( void );
void vButtonTask(void *pvParameters);
void vLedBlink(void *pvParameters);

TaskHandle_t ledTask;

void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
    resetReason_t reason = getResetReason();

	vInitClock();
	vInitDisplay();
	
	xTaskCreate( vLedBlink, (const char *) "ledBlink", configMINIMAL_STACK_SIZE+10, NULL, 1, &ledTask);
	xTaskCreate(vButtonTask, (const char *) "btTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"FreeRTOS 10.0.1");
	vDisplayWriteStringAtPos(1,0,"EDUBoard 1.0");
	vDisplayWriteStringAtPos(2,0,"Template");
	vDisplayWriteStringAtPos(3,0,"ResetReason: %d", reason);
	vTaskStartScheduler();
	return 0;
}

void vLedBlink(void *pvParameters) {
	(void) pvParameters;
	PORTF.DIRSET = PIN0_bm; /*LED1*/
	PORTF.OUT = 0x01;
	for(;;) {
// 		uint32_t stack = get_mem_unused();
// 		uint32_t heap = xPortGetFreeHeapSize();
// 		uint32_t taskStack = uxTaskGetStackHighWaterMark(ledTask);
// 		vDisplayClear();
// 		vDisplayWriteStringAtPos(0,0,"Stack: %d", stack);
// 		vDisplayWriteStringAtPos(1,0,"Heap: %d", heap);
// 		vDisplayWriteStringAtPos(2,0,"TaskStack: %d", taskStack);
// 		vDisplayWriteStringAtPos(3,0,"FreeSpace: %d", stack+heap);
		PORTF.OUTTGL = 0x01;				
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vButtonTask(void *pvParameters) {
	initButtons();
	vTaskDelay(3000);
	vDisplayClear();
	vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
	vDisplayWriteStringAtPos(1,0, "LastPress: None");
	vDisplayWriteStringAtPos(2,0, "Type: Not Pressed");
	for(;;) {
		updateButtons();
		
		if(getButtonPress(BUTTON1) == SHORT_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button1");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
		}
		if(getButtonPress(BUTTON2) == SHORT_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button2");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
		}
		if(getButtonPress(BUTTON3) == SHORT_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button3");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
		}
		if(getButtonPress(BUTTON4) == SHORT_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button4");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
		}
		if(getButtonPress(BUTTON1) == LONG_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button1");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		if(getButtonPress(BUTTON2) == LONG_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button2");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		if(getButtonPress(BUTTON3) == LONG_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button3");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		if(getButtonPress(BUTTON4) == LONG_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button4");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		vTaskDelay((1000/BUTTON_UPDATE_FREQUENCY_HZ)/portTICK_RATE_MS);
	}
}