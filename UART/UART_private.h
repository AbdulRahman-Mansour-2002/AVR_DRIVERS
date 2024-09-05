#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_


#define UBRRH_REG		(*(volatile u8*)(0x40))
#define UBRRL_REG		(*(volatile u8*)(0x29))
#define UCSRC_REG		(*(volatile u8*)(0x40))
#define UCSRB_REG		(*(volatile u8*)(0x2A))
#define UCSRA_REG		(*(volatile u8*)(0x2B))
#define UDR_REG			(*(volatile u8*)(0x2C))

float UART_floatU2Xmode(enum UART_U2X_Options Local_U2X_mode, u32 Local_Baud_Rate, enum UART_ClockPolarity Local_Clock_polarity);
void UART_voidEnablemode(enum UART_mode Local_Enable_mode);
void UART_voidMPCMmode(enum UART_MPCM_Options Local_MPCM_mode);
void UART_voidInterruptmode(enum UART_InterruptMode Local_Interrupt_mode);
void UART_voidDataFrame(enum UART_DataFrame Local_Data_Frame);
void UART_voidParity_Stopmode(enum UART_Parity_and_Stop_Mode Local_Control_Frame);

void (*RXC_Interrupt)(void) = 0;
void (*TXC_Interrupt)(void) = 0;
void (*UDRE_Interrupt)(void) = 0;


#endif