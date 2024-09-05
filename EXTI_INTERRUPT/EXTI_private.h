#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

typedef struct{
	volatile u8 MCUCSR;
	volatile u8 MCUCR;
	volatile u8 RESERVED1;
	volatile u8 RESERVED2;
	volatile u8 RESERVED3;
	volatile u8 RESERVED4;
	volatile u8 FR;
	volatile u8 CR;
}EXTI_t;


#define EXTI_REG					((volatile EXTI_t*)0x54)

#define EXTI_REG_MCUCR				(*(volatile u8*)0x55)
#define EXTI_REG_MCUCSR				(*(volatile u8*)0x54)
#define EXTI_REG_GICR				(*(volatile u8*)0x5B)
#define EXTI_REG_GIFR				(*(volatile u8*)0x5A)



#define EXTI_VECTOR_ISR_EXTI0		__vector_1
#define EXTI_VECTOR_ISR_EXTI1		__vector_2
#define EXTI_VECTOR_ISR_EXTI2		__vector_3


void EXTI_VECTOR_ISR_EXTI0() __attribute__((signal,used));
void EXTI_VECTOR_ISR_EXTI1() __attribute__((signal,used));
void EXTI_VECTOR_ISR_EXTI2() __attribute__((signal,used));




void (*PEXTI_voidCallBacks[3])(void) = {0,0,0};

#endif

