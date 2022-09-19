#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "GPIO_interface.h"
#include "GPIO_private.h"

volatile PORT_t* PGPIO_PORT_tPtrGetRegister(enum GPIOPortID_t PortID){
	
	switch(PortID){
		case GPIO_PORTA:
		return PORTA_REG;
		case GPIO_PORTB:
		return PORTB_REG;
		case GPIO_PORTC:
		return PORTC_REG;
		case GPIO_PORTD:
		return PORTD_REG;
		default:
		return 0;
	}
}


void MGPIO_voidSetPinDirection(enum GPIOPortID_t PortID, u8 PinNumber,enum GPIODirection_t Direction){
	
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return;
	
	switch(Direction){
		case GPIO_INPUT:
		CLR_BIT(P->DDR,PinNumber);
		CLR_BIT(P->PORT,PinNumber);
		break;
		case GPIO_OUTPUT:
		SET_BIT(P->DDR,PinNumber);
		CLR_BIT(P->PORT,PinNumber);
		break;
		case GPIO_INPUT_PULLUP:
		CLR_BIT(P->DDR,PinNumber);
		SET_BIT(P->PORT,PinNumber);
		break;
	}
}

void MGPIO_voidSetPinValue(enum GPIOPortID_t PortID, unsigned char PinNumber, enum GPIOState_t Value){
	
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return;
	
	switch(Value){
		case GPIO_HIGH:
		SET_BIT(P->PORT,PinNumber);
		break;
		case GPIO_LOW:
		CLR_BIT(P->PORT,PinNumber);
		break;
	}
	
}


void MGPIO_voidSetPortDirection(enum GPIOPortID_t PortID,enum GPIODirection_t Direction){
	
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return;
	
	switch(Direction){
		case GPIO_INPUT:
		CLR_REG(P->DDR);
		CLR_REG(P->PORT);
		break;
		case GPIO_OUTPUT:
		SET_REG(P->DDR);
		CLR_REG(P->PORT);
		break;
		case GPIO_INPUT_PULLUP:
		CLR_REG(P->DDR);
		SET_REG(P->PORT);
		break;
	}
}
void MGPIO_voidSetPortValue(enum GPIOPortID_t PortID, u8 Value){
	
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return;
	P->PORT = Value;
	
}

void MGPIO_voidTogglePin(enum GPIOPortID_t PortID, u8 PinNumber){
	
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return;
	TOG_BIT(P->PORT,PinNumber);
	
}

void MGPIO_voidTogglePort(enum GPIOPortID_t PortID){
	
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return;
	TOG_REG(P->PORT);
}

u8 MGPIO_u8GetPinValue(enum GPIOPortID_t PortID, u8 PinNumber){
	
	u8 Local_u8Val = 0;
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return 0;
	Local_u8Val = GET_BIT(P->PIN,PinNumber);

	if(Local_u8Val)
	return GPIO_HIGH;
	else
	return GPIO_LOW;
}

u8 MGPIO_u8GetPortValue(enum GPIOPortID_t PortID)
{
	volatile PORT_t* P = 0;
	P = PGPIO_PORT_tPtrGetRegister(PortID);
	if (P == 0)
	return 0;
	return P->PIN;
}