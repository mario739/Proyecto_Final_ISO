#ifndef OS_IRQ_H
#define OS_IRQ_H

#include <stddef.h>
#include <stdbool.h>
#include "stm32f429xx.h"

 bool os_install_irq(IRQn_Type irq,void* isr);
 bool os_remove_irq(IRQn_Type irq);

#endif
