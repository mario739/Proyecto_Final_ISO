/*
 * os_task.c
 *
 *  Created on: 3 ago. 2022
 *      Author: ferna
 */

#include "os_task.h"


void  weak tick_hook(void)
{
	__NOP();
}
void weak idle_hook(void)
{
	while(1)
	{
		__WFI();
	}
}

void  weak error_hook(void)
{
	while(1){
	}
}
