/*
 * os_IRQ.c
 *
 *  Created on: Aug 22, 2022
 *      Author: ferna
 */

#include "os_IRQ.h"
#include "main.h"
#include "core_cm4.h"
//extern TIM_HandleTypeDef htim6;

static void* vector_funtions[90];

 bool os_install_irq(IRQn_Type irq,void* isr)
 {
	 if (vector_funtions[irq]==NULL)
	 {
		 vector_funtions[irq]=isr;
		 NVIC_EnableIRQ(irq);
		 return true;
	}
	 return false;
 }
 bool os_remove_irq(IRQn_Type irq)
 {
	 if (vector_funtions[irq]!=NULL) {
		 NVIC_DisableIRQ(irq);
		 return true;
	}
	 return false;
 }

 static void os_irq_handler(IRQn_Type irq)
{
	 void (*function_user)(void);
	 e_state_os state_os;
	 state_os=get_status_os();

	 set_status_os(MODE_IRQ);

	 function_user=vector_funtions[irq];
	 function_user();

	 set_status_os(state_os);

	 NVIC_ClearPendingIRQ(irq);

	 if (get_status_scheculer_irq())
	 {
		 set_status_scheduler_irq(false);
		 os_yield();
	 }
}

 void TIM6_DAC_IRQHandler(void)
 {
	 os_irq_handler(TIM6_DAC_IRQn);
 }
 void USART3_IRQHandler(void)
 {
	 os_irq_handler(USART3_IRQn);
 }
 void EXTI15_10_IRQHandler(void)
 {
	 os_irq_handler(EXTI15_10_IRQn );
 }
 void ADC_IRQHandler(void)
 {
	 os_irq_handler(ADC_IRQn );
 }




