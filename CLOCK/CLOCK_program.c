#include "../../STD_LIB/BIT_MATH.h"
#include "../../STD_LIB/STD_TYPES.h"


#include "CLOCK_interface.h"
#include "CLOCK_private.h"
#include "CLOCK_config.h"



void MCLOCK_voidDelayMS(u32 ms){
	register u32 Ticks = (CLOCK_SYSTEM_FREQUENCY/7000)*ms - 10;
	while(Ticks--)
	__asm__ volatile("NOP"); /* That means the cpu wastes the clock cycle or
	 in other words decode last operation and get the second */
}

u32 MCLOCK_u32GetSystemClock(void){
	return CLOCK_SYSTEM_FREQUENCY;
}