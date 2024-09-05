#ifndef EXTI_INTERFACE_H
#define EXTI_INTERFACE_H

enum EXTIID_t{EXTI_LINE0,EXTI_LINE1,EXTI_LINE2};
enum EXTITrigger_t{TRIGGER_LOW_LEVEL,TRIGGER_LOGICAL_CHANGE
					,TRIGGER_FALLING_EDGE,TRIGGER_RISING_EDGE};


void MEXTI_voidEnableInterrupt(enum EXTIID_t Interrupt);
void MEXTI_voidDisableInterrupt(enum EXTIID_t Interrupt);

void MEXTI_voidSetCallBack(enum EXTIID_t Interrupt, void(*CallBack)(void));

void MEXTI_voidSetTrigger(enum EXTIID_t Interrupt, enum EXTITrigger_t Trigger);


#endif

