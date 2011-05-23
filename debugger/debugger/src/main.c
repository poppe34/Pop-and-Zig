/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * AVR Software Framework (ASF).
 */
#define F_CPU   16000000UL
#include <asf.h>
#include "led.h"
#include "gpio.h"
#include "util/delay.h"
#include "scheduler.h"
#include "alarms_task.h"
#include "zigbee.h"

int main(void)
{
	board_init();
    uint16_t count;
	// Insert application code here, after the board has been initialized.
	
	sysclk_init();
	
	pmic_init();

    zigbee_init();

    PORTC.INT0MASK |= PIN3_bm;
	PORTC.INTCTRL |= PORT_INT0LVL_LO_gc;
	PORTC.PIN3CTRL |= (PORT_ISC1_bm);
	//PORTC.PIN3CTRL |= (PORT_ISC1_bm | PORT_ISC2_bm);
	
	sei();
	
	alarm_task_init();
	scheduler();

    return 0;
}

