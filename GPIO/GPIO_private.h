#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_


typedef struct{
	volatile u8 PIN;
	volatile u8 DDR;
	volatile u8 PORT;
}PORT_t;

#define PORTA_REG			((volatile PORT_t *)(0x39))
#define PORTB_REG			((volatile PORT_t *)(0x36))
#define PORTC_REG			((volatile PORT_t *)(0x33))
#define PORTD_REG			((volatile PORT_t *)(0x30))


volatile PORT_t* PGPIO_PORT_tPtrGetRegister(enum GPIOPortID_t PortID);

#endif /* GPIO_PRIVATE_H_ */




