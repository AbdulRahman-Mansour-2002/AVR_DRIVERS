



#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

enum GPIOPortID_t {GPIO_PORTA,GPIO_PORTB,GPIO_PORTC,GPIO_PORTD};
enum GPIODirection_t{GPIO_INPUT,GPIO_OUTPUT,GPIO_INPUT_PULLUP};
enum GPIOState_t{GPIO_LOW,GPIO_HIGH};

void MGPIO_voidSetPinDirection(enum GPIOPortID_t PortID, u8 PinNumber,enum GPIODirection_t Direction);
void MGPIO_voidSetPinValue(enum GPIOPortID_t PortID, u8 PinNumber, enum GPIOState_t Value);

void MGPIO_voidSetPortDirection(enum GPIOPortID_t PortID,enum GPIODirection_t Direction);
void MGPIO_voidSetPortValue(enum GPIOPortID_t PortID, u8 Value);

void MGPIO_voidTogglePin(enum GPIOPortID_t PortID, u8 PinNumber);
void MGPIO_voidTogglePort(enum GPIOPortID_t PortID);

u8 MGPIO_u8GetPinValue(enum GPIOPortID_t PortID, u8 PinNumber);
u8 MGPIO_u8GetPortValue(enum GPIOPortID_t PortID);

#endif /* GPIO_INTERFACE_H_ */