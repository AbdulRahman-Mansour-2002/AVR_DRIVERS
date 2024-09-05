 

#include "../../STD_LIB/BIT_MATH.h"
#include "../../STD_LIB/STD_TYPES.h"

#include "../CLOCK/CLOCK_interface.h"

#include "../EXTI_INTERRUPT/EXTI_interface.h"
#include "../EXTI_INTERRUPT/INTERRUPT.h"

#include "TIM_interface.h"
#include "TIM_private.h"




void MTIM0_voidInit(enum TIM0ClockSource ClockSource, enum TIM0Mode Mode, u8 ChannelMode, u8 Preload){
	
	TCCR0_REG = 0;
	TCNT0_REG = Preload;
	OCR0_REG = 0;
	TIMSK_REG &= ~3;
	
	TCCR0_REG = (GET_BIT(Mode,0) << 6) | (GET_BIT(Mode,1) << 3) | (ChannelMode << 4) | (ClockSource);
	
	PTIM0ClockSource = ClockSource;

}

u8 MTIM0_u8GetCounter(void){
	
	return TCNT0_REG;
}

void MTIM0_voidSetCounter(u8 CounterValue){
	TCNT0_REG = CounterValue;
}

void MTIM0_voidStart(void){
	TCCR0_REG |= PTIM0ClockSource;
}

void MTIM0_voidStop(void){
	TCCR0_REG &= ~7;
}

//DutyCycle will be a number between 0=>255 representing a duty cycle 0=>100%
void MTIM0_voidSetDutyCycle(u8 DutyCycle){
	OCR0_REG = DutyCycle;
}

//Sets CTC Frequency
//freq = fclk/(2*N*(1+OCR))	=>	OCR = (fclk/(2*N*freq)) -1
void MTIM0_voidSetFrequency(u32 Frequency){
	if(PTIM0ClockSource > 5 || PTIM0ClockSource == 0)
		return;
	u32 CPU_Frequency = MCLOCK_u32GetSystemClock();
	OCR0_REG = CPU_Frequency/(2*Frequency*PTIM0Prescalars[PTIM0ClockSource-1]) -1;
}

void MTIM0_voidDelay(u8 TimeTicks){
	TCNT0_REG = 0;
	while(TCNT0_REG < TimeTicks);
}


void MTIM0_voidEnableInterrupt(enum TIM0Interrupt Interrupt){
	SET_BIT(TIFR_REG,Interrupt);
	
	SET_BIT(TIMSK_REG,Interrupt);
	
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}

void MTIM0_voidDisableInterrupt(enum TIM0Interrupt Interrupt){
	CLR_BIT(TIMSK_REG,Interrupt);
	
}

void MTIM0_voidSetCallBack(enum TIM0Interrupt Interrupt, void(*CallBack)(void)){
	
	if (Interrupt > 1)
		return;
	PTim0CallBacks[Interrupt] = CallBack;
	
}

void TIM0_ISR_VECTOR_OVERFLOW(void){
	if(PTim0CallBacks[0] != 0){
		PTim0CallBacks[0]();
	}
	SET_BIT(TIFR_REG,0);
}	
void TIM0_ISR_VECTOR_COMPARE(void){
	if(PTim0CallBacks[1] != 0){
		PTim0CallBacks[1]();
	}
	SET_BIT(TIFR_REG,1);
}



//Timer 1


void	MTIM1_voidInit(enum TIM1ClockSource ClockSource, enum TIM1Mode Mode, u8 ChannelAMode,u8 ChannelBMode, u16 Preload){
	TCCR1A_REG = 0;
	TCCR1B_REG = 0;
	TCNT1_REG = 0;
	OCR1A_REG = 0;
	OCR1B_REG = 0;
	ICR1_REG = 0;
	
	TCCR1A_REG = (ChannelAMode<<6) | (ChannelBMode<<4) | (Mode&3);
	TCCR1B_REG = ((Mode&0xC)<<1) | (ClockSource);
	PTIM1ClockSource = ClockSource;
	
}
u16		MTIM1_u16GetCounter(void){
	return TCNT1_REG;
}
void	MTIM1_voidSetCounter(u16 CounterValue){
	TCNT1_REG = CounterValue;
}
void	MTIM1_voidStart(void){
	TCCR1B_REG |= PTIM1ClockSource;
}
void	MTIM1_voidStop(void){
	TCCR1B_REG &= ~7;
}
//DutyCycle will be a number between 0=>65535 representing a duty cycle 0=>100%
void	MTIM1_voidSetDutyCycle(enum TIM1ChannelID Channel,u16 DutyCycle){
	if(Channel == TIM1_CHANNEL_A)
		OCR1A_REG = DutyCycle;
	else if(Channel == TIM1_CHANNEL_B)
		OCR1B_REG = DutyCycle;
	
}
//Sets CTC Frequency
void	MTIM1_voidSetFrequency(enum TIM1ChannelID Channel, u32 Frequency){
	u16 Reg = MCLOCK_u32GetSystemClock()/(2*Frequency*PTIM1Prescalars[PTIM0ClockSource-1])-1;
	if(Channel == TIM1_CHANNEL_A)
		OCR1A_REG = Reg;
	else if(Channel == TIM1_CHANNEL_B)
		OCR1B_REG = Reg;
		
}

void	MTIM1_voidDelay(u16 TimeTicks){
	TCNT1_REG = 0;
	while(TCNT1_REG < TimeTicks);
}
void	MTIM1_voidEnableInterrupt(enum TIM1Interrupt Interrupt){
	SET_BIT(TIFR_REG,Interrupt);
	SET_BIT(TIMSK_REG,Interrupt);
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}
void	MTIM1_voidDisableInterrupt(enum TIM1Interrupt Interrupt){
	CLR_BIT(TIMSK_REG,Interrupt);
}
void	MTIM1_voidSetCallBack(enum TIM1Interrupt Interrupt, void *CallBack){

	if(Interrupt < TIM1_INTERRUPT_CAPTURE)
		PTIM1CallBacks[Interrupt-2] = (void(*)(void))CallBack;
	else if (Interrupt == TIM1_INTERRUPT_CAPTURE)
		PTIM1CaptureCallBack = (void(*)(u16))CallBack;
}


f32 MTIM1_f32GetPulseDuration(enum TIM1InputPulseType Pulse){
	float FirstCapture = 0, SecondCapture = 0;
	u16 OverFlows = 0;
	//Enable Noise Canceler
	SET_BIT(TCCR1B_REG,7);
	//Clear Capture Interrupt flag
	SET_BIT(TIFR_REG,5);
	if(Pulse == TIM1_PULSE_HIGH)
		//set capture on rising edge
		SET_BIT(TCCR1B_REG,6);
	else 
		//set capture on Falling edge
		CLR_BIT(TCCR1B_REG,6);
	//pull on capture flag
	while(!(TIFR_REG & (1<<TIM1_INTERRUPT_CAPTURE)));
	FirstCapture = ICR1_REG;
	//Clear capture flag
	SET_BIT(TIFR_REG,5);
	//Clear Counter
	//TCNT1_REG = 0;
	
	if(Pulse == TIM1_PULSE_HIGH)
		//set capture on Falling edge
		CLR_BIT(TCCR1B_REG,6);
	else
		//set capture on Rising edge
		SET_BIT(TCCR1B_REG,6);
	//Pull on capture flag
	while(!(TIFR_REG & (1<<TIM1_INTERRUPT_CAPTURE))){
		if(TIFR_REG & (1<<TIM1_INTERRUPT_OVERFLOW)){
			OverFlows++;
			SET_BIT(TIFR_REG,2);
		}
	}

	//Read Captured Value
	SecondCapture = ICR1_REG + OverFlows*65536;
	
	float Duration = 1000* (SecondCapture - FirstCapture) * PTIM1Prescalars[PTIM1ClockSource-1] / MCLOCK_u32GetSystemClock();
	return Duration;
}

f32 MTIM1_f32GetSignalFrequency(void){
	f32 FirstCapture = 0, SecondCapture = 0;
	u16 OverFlows = 0;
	//Enable Noise Canceler
	SET_BIT(TCCR1B_REG,7);
	//Clear Capture Interrupt flag
	SET_BIT(TIFR_REG,5);
	//set capture on rising edge
	SET_BIT(TCCR1B_REG,6);
	//pull on capture flag
	while(!(TIFR_REG & (1<<5)));
	FirstCapture = ICR1_REG;
	//Clear capture flag
	SET_BIT(TIFR_REG,5);
	//Clear Counter
	//TCNT1_REG = 0;
	//Pull on capture flag
	while(!(TIFR_REG & (1<<5))){
		if(TIFR_REG & (1<<2)){
			OverFlows++;
			SET_BIT(TIFR_REG,2);
		}
	}

	//Read Captured Value
	SecondCapture = ICR1_REG + OverFlows*65536;
	return MCLOCK_u32GetSystemClock()/( (SecondCapture- FirstCapture) * PTIM1Prescalars[PTIM1ClockSource-1]);
}



void MTIM1_voidStartTickMeasure(void){
	PTIM1_u32OverFlowCount = 0;
	MTIM1_voidEnableInterrupt(TIM1_INTERRUPT_OVERFLOW);
	PTIM1_u16FirstValue = TCNT1_REG ;
}

u32 MTIM1_u32GetElapsedTicks(void){

	u32 Ticks = TCNT1_REG + PTIM1_u32OverFlowCount * 65536 - PTIM1_u16FirstValue;
	return Ticks;
}

u16 MTIM1_u16GetDivision(void){
	return PTIM1Prescalars[PTIM1ClockSource-1];
}


void TIM1_ISR_VECTOR_CAPTURE(void){
	if(PTIM1CaptureCallBack != 0)
		PTIM1CaptureCallBack(ICR1_REG);
}
void TIM1_ISR_VECTOR_COMPAREA(void){
	if(PTIM1CallBacks[2] != 0)
		PTIM1CallBacks[2]();
}
void TIM1_ISR_VECTOR_COMPAREB(void){
	if(PTIM1CallBacks[1] != 0)
		PTIM1CallBacks[1]();
}
void TIM1_ISR_VECTOR_OVERFLOW(void){
	PTIM1_u32OverFlowCount++;
	if(PTIM1CallBacks[0] != 0)
		PTIM1CallBacks[0]();
}


//Timer 2
void MTIM2_voidInit(enum TIM2ClockSource ClockSource, enum TIM2Mode Mode, u8 ChannelMode, u8 Preload){
	TCCR2_REG = 0;
	TCNT2_REG = Preload;
	OCR2_REG = 0;
	TIMSK_REG &= ~(3 << 6);
	
	TCCR2_REG = (GET_BIT(Mode,0) << 6) | (GET_BIT(Mode,1) << 3) | (ChannelMode << 4) | (ClockSource);
	
	PTIM2ClockSource = ClockSource;
}
void MTIM2_voidEnableAsynchMode(void){
	u8 Local_u8TCCR = TCCR2_REG;
	//Disable Interrupts
	TIMSK_REG &= ~(3 << 6);
	//Set AS2
	SET_BIT(ASSR_REG,3);
	TCCR2_REG = Local_u8TCCR;
	OCR2_REG = 0;
	while(!(ASSR_REG & (1<<2)));
	while(!(ASSR_REG & (1<<1)));
	while(!(ASSR_REG & (1)));
	TIFR_REG |= (3<<6);
}
void MTIM2_voidDisableAsynchMode(void){
	u8 Local_u8TCCR = TCCR2_REG;
	//Disable Interrupts
	TIMSK_REG &= ~(3 << 6);
	//Set AS2
	CLR_BIT(ASSR_REG,3);
	TCCR2_REG = Local_u8TCCR;
	OCR2_REG = 0;
	TIFR_REG |= (3<<6);
	
}
u8	 MTIM2_u8GetCounter(void){
	
	return TCNT2_REG;
}
void MTIM2_voidSetCounter(u8 CounterValue){
	
	
}
void MTIM2_voidStart(void){
	TCCR2_REG |= PTIM2ClockSource;
	
}
void MTIM2_voidStop(void){
	TCCR2_REG &= ~7;
	
}
//DutyCycle will be a number between 0=>255 representing a duty cycle 0=>100%
void MTIM2_voidSetDutyCycle(u8 DutyCycle){
	OCR2_REG = DutyCycle;
	
}
//Sets CTC Frequency
void MTIM2_voidSetFrequency(u32 Frequency){
	if(PTIM2ClockSource > 5 || PTIM2ClockSource == 0)
		return;
	u32 CPU_Frequency = MCLOCK_u32GetSystemClock();
	OCR2_REG = CPU_Frequency/(2*Frequency*PTIM2Prescalars[PTIM2ClockSource-1]) -1;
	
}
void MTIM2_voidDelay(u8 TimeTicks){
	
	TCNT2_REG = 0;
	while(TCNT2_REG < TimeTicks);
}
void MTIM2_voidEnableInterrupt(enum TIM2Interrupt Interrupt){
	SET_BIT(TIFR_REG,Interrupt);
	
	SET_BIT(TIMSK_REG,Interrupt);
	
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;	
	
}
void MTIM2_voidDisableInterrupt(enum TIM2Interrupt Interrupt){
	
		CLR_BIT(TIMSK_REG,Interrupt);
}
void MTIM2_voidSetCallBack(enum TIM2Interrupt Interrupt, void(*CallBack)(void)){
	if (Interrupt > 7 || Interrupt < 6)
		return;
	PTim2CallBacks[Interrupt-6] = CallBack;
}


void TIM2_ISR_VECTOR_OVERFLOW(void){
	if(PTim2CallBacks[0] != 0){
		PTim2CallBacks[0]();
	}
	SET_BIT(TIFR_REG,6);
}
void TIM2_ISR_VECTOR_COMPARE(void){
	if(PTim2CallBacks[1] != 0){
		PTim2CallBacks[1]();
	}
	SET_BIT(TIFR_REG,7);
}