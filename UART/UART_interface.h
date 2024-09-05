#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_


enum UART_MPCM_Options {UART_Single, UART_Multi};
enum UART_U2X_Options {UART_Async_NormalSpeed, UART_Async_DoubleSpeed, UART_sync};
enum UART_mode {UART_Disable, UART_Tx, UART_Rx, UART_Tx_or_Rx};
enum UART_InterruptMode {UART_InterruptDisable, UART_RxInterruptEnable, UART_TxInterruptEnable, UART_UDRInterruptEnable,
	 UART_Tx_Rx_InterruptEnable, UART_Tx_UDR_InterruptEnable, UART_Rx_UDR_InterruptEnable, UART_Tx_Rx_UDR_InterruptEnable};
enum UART_DataFrame {UART_5bitData, UART_6bitData, UART_7bitData, UART_8bitData, UART_9bitData};
enum UART_Parity_and_Stop_Mode {UART_PArityDisable_1Stop, UART_PArityDisable_2Stop, UART_PArityEven_1Stop, UART_PArityEven_2Stop,
UART_PArityOdd_1Stop, UART_PArityOdd_2Stop};
enum UART_ClockPolarity {UART_FallingEdge, UART_RisingEdge}; // Only in Synchronous mode


typedef struct
{
	enum UART_mode Enable_mode;
	enum UART_U2X_Options U2X_mode;
	enum UART_MPCM_Options MPCM_mode;
	enum UART_InterruptMode Interrupt_mode;
	enum UART_DataFrame Data_Frame;
	enum UART_Parity_and_Stop_Mode Control_Frame;
	enum UART_ClockPolarity Clock_polarity;
	u32 Baud_Rate;
}UART_t;


UART_t UART_UART_tInit(enum UART_mode Local_Enable_mode, enum UART_U2X_Options Local_U2X_mode,
					   enum UART_MPCM_Options Local_MPCM_mode, enum UART_InterruptMode Local_Interrupt_mode, 
					   enum UART_DataFrame Local_Data_Frame, enum UART_Parity_and_Stop_Mode Local_Control_Frame,
					   enum UART_ClockPolarity Local_Clock_polarity, u32 Local_Baud_Rate);

void UART_voidTransmitByte(u8 Byte, UART_t UART0);

void UART_voidTransmitString(u8* Str, UART_t UART0);

u8 UART_u8ReciveByte(UART_t UART0);

void UART_voidReciveString(UART_t UART0, u8* Str);

//void UART_SetCallBacks(void(*CallBack)(void));


#endif