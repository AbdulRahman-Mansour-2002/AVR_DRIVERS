


#ifndef TIM_PRIVATE_H_
#define TIM_PRIVATE_H_

//Timer 0 Stuff
#define TIMSK_REG			(*(volatile u8*)0x59)
#define TIFR_REG			(*(volatile u8*)0x58)

#define TCCR0_REG			(*(volatile u8*)0x53)
#define TCNT0_REG			(*(volatile u8*)0x52)
#define OCR0_REG			(*(volatile u8*)0x5C)


#define TIM0_ISR_VECTOR_OVERFLOW		__vector_11
#define TIM0_ISR_VECTOR_COMPARE			__vector_10


void TIM0_ISR_VECTOR_OVERFLOW(void)	__attribute__((signal,used));
void TIM0_ISR_VECTOR_COMPARE(void)	__attribute__((signal,used));

void (*PTim0CallBacks[2])(void) = {0,0};

u8 PTIM0ClockSource = 0;
u16 PTIM0Prescalars[5] = {1,8,64,256,1024};



//Timer 1 Stuff
#define TCCR1A_REG				(*(volatile u8*	)0x4F)
#define TCCR1B_REG				(*(volatile u8*	)0x4E)
#define TCNT1_REG				(*(volatile u16*)0x4C)
#define ICR1_REG				(*(volatile u16*)0x46)
#define OCR1B_REG				(*(volatile u16*)0x48)
#define OCR1A_REG				(*(volatile u16*)0x4A)

#define TIM1_ISR_VECTOR_OVERFLOW		__vector_9
#define TIM1_ISR_VECTOR_COMPAREA		__vector_7
#define TIM1_ISR_VECTOR_COMPAREB		__vector_8
#define TIM1_ISR_VECTOR_CAPTURE			__vector_6

void TIM1_ISR_VECTOR_CAPTURE(void)	__attribute__((signal,used));
void TIM1_ISR_VECTOR_COMPAREA(void)	__attribute__((signal,used));
void TIM1_ISR_VECTOR_COMPAREB(void)	__attribute__((signal,used));
void TIM1_ISR_VECTOR_OVERFLOW(void)	__attribute__((signal,used));

void (*PTIM1CallBacks[3])(void) = {0,0,0};

void (*PTIM1CaptureCallBack)(u16 CaptureValue) = 0;

u8 PTIM1ClockSource = 0;
u16 PTIM1Prescalars[5] = {1,8,64,256,1024};


volatile u32 PTIM1_u32OverFlowCount = 0;
u16 PTIM1_u16FirstValue = 0;


#define TCCR2_REG			(*(volatile u8*)0x45)
#define TCNT2_REG			(*(volatile u8*)0x44)
#define OCR2_REG			(*(volatile u8*)0x43)
#define ASSR_REG			(*(volatile u8*)0x42)

#define TIM2_ISR_VECTOR_OVERFLOW		__vector_5
#define TIM2_ISR_VECTOR_COMPARE			__vector_4


void TIM2_ISR_VECTOR_OVERFLOW(void)	__attribute__((signal,used));
void TIM2_ISR_VECTOR_COMPARE(void)	__attribute__((signal,used));

void (*PTim2CallBacks[2])(void) = {0,0};

u8 PTIM2ClockSource = 0;
u16 PTIM2Prescalars[5] = {1,8,64,256,1024};



#endif /* TIM_PRIVATE_H_ */