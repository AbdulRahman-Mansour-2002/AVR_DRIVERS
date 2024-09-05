#include "../../STD_LIB/STD_TYPES.h"
#include "../../STD_LIB/BIT_MATH.h"

#include "../GPIO/GPIO_interface.h"

#include "../EXTI_INTERRUPT/INTERRUPT.h"
#include "../EXTI_INTERRUPT/INTERRUPT.h"

#include "UART_interface.h"
#include "UART_private.h"
#include "UART_config.h"

void UART_voidEnablemode(enum UART_mode Local_Enable_mode)
{
	switch(Local_Enable_mode)
	{
		case UART_Disable: // UCSRA Temp bit of Tx and Rx enable = 0
			break;
		case UART_Tx:
			SET_BIT(UCSRB_REG, 3); // UCSRB Temp bit of Tx enable = 1
			break;
		case UART_Rx:
			SET_BIT(UCSRB_REG, 4); // UCSRB Temp bit of Rx enable = 1
			break;
		case UART_Tx_or_Rx:
			SET_BIT(UCSRB_REG, 3); // UCSRB Temp bit of Tx enable = 1
			SET_BIT(UCSRB_REG, 4); // UCSRB Temp bit of Rx enable = 1
			break;
		default:
			break;
	}
}

float UART_floatU2Xmode(enum UART_U2X_Options Local_U2X_mode, u32 Local_Baud_Rate, enum UART_ClockPolarity Local_Clock_polarity)
{
	float UBRR_Temp = 0.0;
	switch(Local_U2X_mode)
	{
		case UART_Async_NormalSpeed:
			CLR_BIT(UCSRC_REG, 6); // UCSRC Temp bit of UMSEL = 0 means Asynchronous
			CLR_BIT(UCSRA_REG, 1); // UCSRA Temp bit of U2X = 0
			UBRR_Temp = ((float)F_CPU / (16.0 * Local_Baud_Rate)) - 1 + 0.5; // +0.5 for rounding
			break;
		case UART_Async_DoubleSpeed:
			CLR_BIT(UCSRC_REG, 6); // UCSRC Temp bit of UMSEL = 0 means Asynchronous
			SET_BIT(UCSRA_REG, 1); // UCSRA Temp bit of U2X = 1
			UBRR_Temp = ((float)F_CPU / (8.0 * Local_Baud_Rate)) - 1 + 0.5;
			break;
		case UART_sync:
			SET_BIT(UCSRC_REG, 6); // UCSRC Temp bit of UMSEL = 1 means Synchronous
			UBRR_Temp = ((float)F_CPU / (2.0 * Local_Baud_Rate)) - 1 + 0.5;
			switch(Local_Clock_polarity)
			{
				case UART_FallingEdge:
					CLR_BIT(UCSRC_REG, 0); // UCPOL = 0
					break;
				case UART_RisingEdge:
					SET_BIT(UCSRC_REG, 0); // UCPOL = 1
					break;
			}
			break;
		default:
			break;
	}
	return UBRR_Temp;
}

void UART_voidMPCMmode(enum UART_MPCM_Options Local_MPCM_mode)
{
	switch(Local_MPCM_mode)
	{
		case UART_Single:
			CLR_BIT(UCSRA_REG, 0); // MPCM = 0 means Single processor
			break;
		case UART_Multi:
			SET_BIT(UCSRA_REG, 0); // MPCM = 1 means Multi processor
			break;
		default:
			break;
	}
}

void UART_voidInterruptmode(enum UART_InterruptMode Local_Interrupt_mode)
{
	switch(Local_Interrupt_mode)
	{
		case UART_InterruptDisable:
			break;
		case UART_RxInterruptEnable:
			SET_BIT(UCSRB_REG, 7); // Enable Rx interrupt
			break;
		case UART_TxInterruptEnable:
			SET_BIT(UCSRB_REG, 6); // Enable Tx interrupt
			break;
		case UART_UDRInterruptEnable:
			SET_BIT(UCSRB_REG, 5); // Enable UDR interrupt
			break;
		case UART_Tx_Rx_InterruptEnable:
			SET_BIT(UCSRB_REG, 7); // Enable Rx interrupt
			SET_BIT(UCSRB_REG, 6); // Enable Tx interrupt
			break;
		case UART_Tx_UDR_InterruptEnable:
			SET_BIT(UCSRB_REG, 6); // Enable Tx interrupt
			SET_BIT(UCSRB_REG, 5); // Enable UDR interrupt
			break;
		case UART_Rx_UDR_InterruptEnable:
			SET_BIT(UCSRB_REG, 7); // Enable Rx interrupt
			SET_BIT(UCSRB_REG, 5); // Enable UDR interrupt
			break;
		case UART_Tx_Rx_UDR_InterruptEnable:
			SET_BIT(UCSRB_REG, 7); // Enable Rx interrupt
			SET_BIT(UCSRB_REG, 6); // Enable Tx interrupt
			SET_BIT(UCSRB_REG, 5); // Enable UDR interrupt
			break;
		default:
			break;
	}
}

void UART_voidDataFrame(enum UART_DataFrame Local_Data_Frame)
{
	switch(Local_Data_Frame)
	{
		case UART_5bitData:
			CLR_BIT(UCSRB_REG, 2); // UCSZ2 = 0
			CLR_BIT(UCSRC_REG, 2); // UCSZ1 = 0
			CLR_BIT(UCSRC_REG, 1); // UCSZ0 = 0
			break;
		case UART_6bitData:
			SET_BIT(UCSRC_REG, 1); // UCSZ0 = 1
			break;
		case UART_7bitData:
			SET_BIT(UCSRC_REG, 2); // UCSZ1 = 1
			break;
		case UART_8bitData:
			SET_BIT(UCSRC_REG, 1); // UCSZ0 = 1
			SET_BIT(UCSRC_REG, 2); // UCSZ1 = 1
			break;
		case UART_9bitData:
			SET_BIT(UCSRC_REG, 1); // UCSZ0 = 1
			SET_BIT(UCSRC_REG, 2); // UCSZ1 = 1
			SET_BIT(UCSRB_REG, 2); // UCSZ2 = 1
			break;
		default:
			break;
	}
}

void UART_voidParity_Stopmode(enum UART_Parity_and_Stop_Mode Local_Control_Frame)
{
	switch(Local_Control_Frame)
	{
		case UART_PArityDisable_1Stop:
			CLR_BIT(UCSRC_REG, 5); // UPM1 = 0
			CLR_BIT(UCSRC_REG, 4); // UPM0 = 0
			CLR_BIT(UCSRC_REG, 3); // 1 Stop
			break;
		case UART_PArityDisable_2Stop:
			CLR_BIT(UCSRC_REG, 5); // UPM1 = 0
			CLR_BIT(UCSRC_REG, 4); // UPM0 = 0
			SET_BIT(UCSRC_REG, 3); // 2 Stop
			break;
		case UART_PArityEven_1Stop:
			SET_BIT(UCSRC_REG, 5); // UPM1 = 1
			CLR_BIT(UCSRC_REG, 4); // UPM0 = 0
			CLR_BIT(UCSRC_REG, 3); // 1 Stop
			break;
		case UART_PArityEven_2Stop:
			SET_BIT(UCSRC_REG, 5); // UPM1 = 1
			CLR_BIT(UCSRC_REG, 4); // UPM0 = 0
			SET_BIT(UCSRC_REG, 3); // 2 Stop
			break;
		case UART_PArityOdd_1Stop:
			SET_BIT(UCSRC_REG, 5); // UPM1 = 1
			SET_BIT(UCSRC_REG, 4); // UPM0 = 1
			CLR_BIT(UCSRC_REG, 3); // 1 Stop
			break;
		case UART_PArityOdd_2Stop:
			SET_BIT(UCSRC_REG, 5); // UPM1 = 1
			SET_BIT(UCSRC_REG, 4); // UPM0 = 1
			SET_BIT(UCSRC_REG, 3); // 2 Stop
			break;
		default:
			break;
	}
}

UART_t UART_UART_tInit(enum UART_mode Local_Enable_mode, enum UART_U2X_Options Local_U2X_mode,
enum UART_MPCM_Options Local_MPCM_mode, enum UART_InterruptMode Local_Interrupt_mode,
enum UART_DataFrame Local_Data_Frame, enum UART_Parity_and_Stop_Mode Local_Control_Frame,
enum UART_ClockPolarity Local_Clock_polarity, u32 Local_Baud_Rate)
{
	UCSRA_REG = 0x00;
	UCSRB_REG = 0x00;
	UCSRC_REG = 0x00;
	float UBRR_Temp = 0.0;
	
	UART_t UART0;
	UART0.Enable_mode = Local_Enable_mode;
	UART0.U2X_mode = Local_U2X_mode;
	UART0.MPCM_mode = Local_MPCM_mode;
	UART0.Interrupt_mode = Local_Interrupt_mode;
	UART0.Data_Frame = Local_Data_Frame;
	UART0.Control_Frame = Local_Control_Frame;
	UART0.Clock_polarity = Local_Clock_polarity;
	UART0.Baud_Rate = Local_Baud_Rate;
	
	UBRR_Temp = UART_floatU2Xmode(Local_U2X_mode, Local_Baud_Rate, Local_Clock_polarity);
	UART_voidEnablemode(Local_Enable_mode);
	UART_voidMPCMmode(Local_MPCM_mode);
	UART_voidInterruptmode(Local_Interrupt_mode);
	UART_voidDataFrame(Local_Data_Frame);
	UART_voidParity_Stopmode(Local_Control_Frame);
	
	UBRRH_REG = ((unsigned short)UBRR_Temp) >> 8; // to take most significant bits
	UBRRL_REG = (((u16)UBRR_Temp) & 0xFF); // to take least significant bits
	return UART0;
}


void UART_voidTransmitByte(u8 Byte, UART_t UART0)
{
	// This while holds the operation until TX reg UDR is empty
	while(GET_BIT(UCSRA_REG, 5) == 0) {} // 5 means UDRE
	
	if(UART0.Data_Frame == UART_9bitData)
	{
		UCSRB_REG |= (GET_BIT(Byte, 8)) >> 8; //shifted by 8 to be in 9 digit
	}
	UDR_REG = Byte;
}

void UART_voidTransmitString(u8* Str, UART_t UART0)
{
	u8 i = 0;
	while(Str[i] != '\0')
	{
		UART_voidTransmitByte(Str[i], UART0);
		i++;
	}
	UART_voidTransmitByte('\0', UART0);
}

u8 UART_u8ReciveByte(UART_t UART0)
{
	u8 Local_Byte = 0;
	// This while holds the operation until Rx reg is empty
	while(GET_BIT(UCSRA_REG, 7) == 0) {} // 7 means RXC
	
	if(UCSRA_REG & ((1 << 4) | (1 << 3) | (1 << 2))) // if any error flag is raised
		return -1;
	
	if(UART0.Data_Frame == UART_9bitData)
	{
		Local_Byte = (GET_BIT(UCSRB_REG, 1) << 7); // 1 means RXB8 and shifted by 7 to be in 9 digit
	}
	
	Local_Byte |= UDR_REG;
	return Local_Byte;
}

void UART_voidReciveString(UART_t UART0, u8* Str)
{
	u8 i = 0;
	while(Str[i] != '\0')
	{
		Str[i] = UDR_REG;
		i++;
	}
}