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
void vBlinkTask1(void *pvParameters);
void vBlinkTask2(void *pvParameters);
void vBlinkTask3(void *pvParameters);
void vBlinkTask4(void *pvParameters);
void vLoadKiller(void *pvParameters);

TaskHandle_t blinkTask1Handle;
TaskHandle_t blinkTask2Handle;
TaskHandle_t blinkTask3Handle;
TaskHandle_t blinkTask4Handle;
TaskHandle_t loadkillTaskHandle;

void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
    resetReason_t reason = getResetReason();

	vInitClock();
	vInitDisplay();
	
	xTaskCreate( vBlinkTask1, (const char *) "blnktsk1", configMINIMAL_STACK_SIZE, NULL, 1, &blinkTask1Handle);
	xTaskCreate( vBlinkTask2, (const char *) "blnktsk2", configMINIMAL_STACK_SIZE, NULL, 1, &blinkTask2Handle);
	xTaskCreate( vBlinkTask3, (const char *) "blnktsk3", configMINIMAL_STACK_SIZE, NULL, 2, &blinkTask3Handle);
	xTaskCreate( vBlinkTask4, (const char *) "blnktsk4", configMINIMAL_STACK_SIZE, NULL, 3, &blinkTask4Handle);
	xTaskCreate(vButtonTask, (const char *) "btTask", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate( vLoadKiller, (const char*) "loadkill", configMINIMAL_STACK_SIZE, NULL, 2, &loadkillTaskHandle);

	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"FreeRTOS 10.0.1");
	vDisplayWriteStringAtPos(1,0,"EDUBoard 1.0");
	vDisplayWriteStringAtPos(2,0,"Template");
	vDisplayWriteStringAtPos(3,0,"ResetReason: %d", reason);
	vTaskStartScheduler();
	return 0;
}

void vLoadKiller(void *pvParameters) {
	uint32_t i = 0;
	vTaskSuspend(loadkillTaskHandle); //initial state of loadkill Task shall be off
	for(;;) {
		i++; //do something, but dont wait/delay
	}
}

void vBlinkTask1(void *pvParameters) {
	(void) pvParameters;
	PORTF.DIRSET = PIN0_bm | PIN1_bm;
	PORTF.OUTCLR = PIN0_bm | PIN1_bm;
	for(;;) {
		PORTF.OUTTGL = PIN0_bm | PIN1_bm;		
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vBlinkTask2(void *pvParameters) {
	(void) pvParameters;
	PORTF.DIRSET = PIN2_bm | PIN3_bm;
	PORTF.OUTCLR = PIN2_bm | PIN3_bm;
	for(;;) {
		PORTF.OUTTGL = PIN2_bm | PIN3_bm;		
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vBlinkTask3(void *pvParameters) {
	(void) pvParameters;
	PORTE.DIRSET = PIN0_bm | PIN1_bm;
	PORTE.OUTCLR = PIN0_bm | PIN1_bm;
	for(;;) {
		PORTE.OUTTGL = PIN0_bm | PIN1_bm;		
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vBlinkTask4(void *pvParameters) {
	(void) pvParameters;
	PORTE.DIRSET = PIN2_bm | PIN3_bm;
	PORTE.OUTCLR = PIN2_bm | PIN3_bm;
	for(;;) {
		PORTE.OUTTGL = PIN2_bm | PIN3_bm;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vButtonTask(void *pvParameters) {
	initButtons();
	for(;;) {
		updateButtons();		
		if(getButtonPress(BUTTON1) == SHORT_PRESSED) {
			eTaskState state = eTaskGetState(blinkTask1Handle);
			if(state == eSuspended) {
				vTaskResume(blinkTask1Handle);
			} else {
				vTaskSuspend(blinkTask1Handle);
			}
		}
		if(getButtonPress(BUTTON2) == SHORT_PRESSED) {
			eTaskState state = eTaskGetState(blinkTask2Handle);
			if(state == eSuspended) {
				vTaskResume(blinkTask2Handle);
			} else {
				vTaskSuspend(blinkTask2Handle);
			}
		}
		if(getButtonPress(BUTTON3) == SHORT_PRESSED) {
			eTaskState state = eTaskGetState(blinkTask3Handle);
			if(state == eSuspended) {
				vTaskResume(blinkTask3Handle);
			} else {
				vTaskSuspend(blinkTask3Handle);
			}
		}
		if(getButtonPress(BUTTON4) == SHORT_PRESSED) {
			eTaskState state = eTaskGetState(blinkTask4Handle);
			if(state == eSuspended) {
				vTaskResume(blinkTask4Handle);
			} else {
				vTaskSuspend(blinkTask4Handle);
			}
		}
		if(getButtonPress(BUTTON1) == LONG_PRESSED) {
			eTaskState state = eTaskGetState(loadkillTaskHandle);
			if(state == eSuspended) {
				vTaskResume(loadkillTaskHandle);
			} else {
				vTaskSuspend(loadkillTaskHandle);
			}
		}
		if(getButtonPress(BUTTON2) == LONG_PRESSED) {
			
		}
		if(getButtonPress(BUTTON3) == LONG_PRESSED) {
			
		}
		if(getButtonPress(BUTTON4) == LONG_PRESSED) {
			
		}
		vTaskDelay((1000/BUTTON_UPDATE_FREQUENCY_HZ)/portTICK_RATE_MS);
	}
}