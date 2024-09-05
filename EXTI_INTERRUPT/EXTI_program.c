

#include "../../STD_LIB/BIT_MATH.h"
#include "../../STD_LIB/STD_TYPES.h"

#include "INTERRUPT.h"

#include "EXTI_interface.h"
#include "EXTI_private.h"



void MEXTI_voidEnableInterrupt(enum EXTIID_t Interrupt){
	switch(Interrupt){
		case EXTI_LINE0:
			SET_BIT(EXTI_REG->CR,6); // cr is the control register 
			break;	
		case EXTI_LINE1:
			SET_BIT(EXTI_REG->CR,7);
			break;
		case EXTI_LINE2:
			SET_BIT(EXTI_REG->CR,5);
			break;
		default:
			return;
	}	
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}


void MEXTI_voidDisableInterrupt(enum EXTIID_t Interrupt){
	switch(Interrupt){
		case EXTI_LINE0:
			CLR_BIT(EXTI_REG->CR,6);
			break;
		case EXTI_LINE1:
			CLR_BIT(EXTI_REG->CR,7);
			break;
		case EXTI_LINE2:
			CLR_BIT(EXTI_REG->CR,5);
			break;
		default:
			break;
	}
}

void MEXTI_voidSetCallBack(enum EXTIID_t Interrupt, void(*CallBack)(void)){
	
	if(Interrupt > EXTI_LINE2)
		return;
	PEXTI_voidCallBacks[Interrupt] = CallBack;
	
}

void MEXTI_voidSetTrigger(enum EXTIID_t Interrupt, enum EXTITrigger_t Trigger){
	
	if(Interrupt == EXTI_LINE2 && 
	(Trigger == TRIGGER_LOGICAL_CHANGE || Trigger == TRIGGER_LOW_LEVEL))
		return;
	
	if(Trigger > TRIGGER_RISING_EDGE)
		return;
	switch(Interrupt){
		case EXTI_LINE0:
			EXTI_REG->MCUCR &= ~3;
			EXTI_REG->MCUCR |= Trigger;
			break;
		case EXTI_LINE1:
			EXTI_REG->MCUCR &= ~12;
			EXTI_REG->MCUCR |= Trigger<<2;
			break;
		case EXTI_LINE2:
			CLR_BIT(EXTI_REG->CR,5);
			SET_BIT(EXTI_REG->FR,5);
			CLR_BIT(EXTI_REG->MCUCSR,6);
			EXTI_REG->MCUCSR |= GET_BIT(Trigger,0)<<6;                                                                 
			break;
		default:
			break;
	}
}

void MEXTI_voidClearFlag(enum EXTIID_t Interrupt){
	switch(Interrupt){
		
		case EXTI_LINE0:
			SET_BIT(EXTI_REG->FR,6);
			break;
		case EXTI_LINE1:
			SET_BIT(EXTI_REG->FR,7);
			break;
		case EXTI_LINE2:
			SET_BIT(EXTI_REG->FR,5);
			break;
		default:
			break;
	}
}

void EXTI_VECTOR_ISR_EXTI0(void){
	
	if(PEXTI_voidCallBacks[0] != 0){
			PEXTI_voidCallBacks[0]();
		}
}

void EXTI_VECTOR_ISR_EXTI1(void){
	
	if(PEXTI_voidCallBacks[1] != 0){
		PEXTI_voidCallBacks[1]();
	}
}

void EXTI_VECTOR_ISR_EXTI2(void){
	
	if(PEXTI_voidCallBacks[2] != 0){
		PEXTI_voidCallBacks[2]();
	}
}