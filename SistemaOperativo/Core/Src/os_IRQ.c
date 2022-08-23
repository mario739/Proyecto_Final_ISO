/*
 * os_IRQ.c
 *
 *  Created on: Aug 22, 2022
 *      Author: ferna
 */

#include "os_IRQ.h"
#include "main.h"
#include "core_cm4.h"

static void* vector_funtions[90];

//Funcion para instalar una irq en el SO
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
 //Funcion para remover una irq de SO
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
	 //Guarda el estado actual de SO
	 state_os=get_status_os();
	 //Setea al SO en modo IRQ
	 set_status_os(MODE_IRQ);

	 function_user=vector_funtions[irq];
	 function_user();
	 //Setea el estado que se guardo anteriormente
	 set_status_os(state_os);
	 //limpia la bandera de la interrupcion que se atendio
	 NVIC_ClearPendingIRQ(irq);
	 //Se consulta si alguna tarea cambio su estado por lo tanto se necesita un rescheduling
	 if (get_status_scheduler_irq())
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




