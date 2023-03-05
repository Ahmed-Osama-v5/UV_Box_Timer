/*
 * dio.c
 *
 *  Created on: Dec 22, 2019
 *      Author: Ahmed
 */
#include "dio.h"

/*
*Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	if(NULL != DIO_info)
	{
		switch (DIO_info->GPIO)
		{
			/*
		case (GPIOA):
			if(DIO_info->dir == OUTPUT)
			{
				DDRA |= (1 << DIO_info->pins);
			}
			else
			{
				DDRA &= ~(1 << DIO_info->pins);
			}
			break;
			*/
		case (GPIOB):
			if(DIO_info->dir == OUTPUT)
			{
				DDRB |= (1 << DIO_info->pins);
			}
			else
			{
				DDRB &= ~(1 << DIO_info->pins);
			}
			break;
		case (GPIOC):
			if(DIO_info->dir == OUTPUT)
			{
				DDRC |= (1 << DIO_info->pins);
			}
			else
			{
				DDRC &= ~(1 << DIO_info->pins);
			}
			break;
		case (GPIOD):
			if(DIO_info->dir == OUTPUT)
			{
				DDRD |= (1 << DIO_info->pins);
			}
			else
			{
				DDRD &= ~(1 << DIO_info->pins);
			}
			break;
		default:
			return E_NOK;
			break;
		}
	}
	else
	{
		return E_NOK;
	}
	return E_OK;
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value)
{
	switch(GPIO)
	{
		/*
	case (GPIOA):
		if(value)
		{
			PORTA |= (1 << pins);
		}
		else
		{
			PORTA &= ~(1 << pins);
		}
		break;
		*/
	case (GPIOB):
		if(value)
		{
			PORTB |= (1 << pins);
		}
		else
		{
			PORTB &= ~(1 << pins);
		}
		break;
	case (GPIOC):
		if(value)
		{
			PORTC |= (1 << pins);
		}
		else
		{
			PORTC &= ~(1 << pins);
		}
		break;
	case (GPIOD):
		if(value)
		{
			PORTD |= (1 << pins);
		}
		else
		{
			PORTD &= ~(1 << pins);
		}
		break;
	default:
		return E_NOK;
		break;
	}
	return E_OK;
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data)
{
	switch(GPIO)
	{
		/*
	case (GPIOA):
		if(PINA & (1 << pins))
		{
			*data = HIGH;
		}
		else
		{
			*data = LOW;
		}
		break;
		*/
	case (GPIOB):
		if(PINB & (1 << pins))
		{
			*data = HIGH;
		}
		else
		{
			*data = LOW;
		}
		break;
	case (GPIOC):
		if(PINC & (1 << pins))
		{
			*data = HIGH;
		}
		else
		{
			*data = LOW;
		}
		break;
	case (GPIOD):
		if(PIND & (1 << pins))
		{
			*data = HIGH;
		}
		else
		{
			*data = LOW;
		}
		break;
	default:
		return E_NOK;
		break;
	}
	return E_OK;
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle (uint8_t GPIO, uint8_t pins)
{
	switch(GPIO)
	{
		/*
	case (GPIOA):
		PORTA ^= (1 << pins);
		break;
		*/
	case (GPIOB):
		PORTB ^= (1 << pins);
		break;
	case (GPIOC):
		PORTC ^= (1 << pins);
		break;
	case (GPIOD):
		PORTD ^= (1 << pins);
		break;
	default:
		return E_NOK;
		break;
	}
	return E_OK;
}

void DIO_Internal_Pull_Up (uint8_t GPIO, uint8_t pins, uint8_t pullupState)
{
	switch(GPIO)
	{
		/*
	case (GPIOA):
		if(pullupState == PULL_UP_ACTIVATE)
		{
			PORTA |= (1 << pins);
		}
		else
		{
			PORTA &= ~(1 << pins);
		}
		break;
		*/
	case (GPIOB):
		if(pullupState == PULL_UP_ACTIVATE)
		{
			PORTB |= (1 << pins);
		}
		else
		{
			PORTB &= ~(1 << pins);
		}
		break;
	case (GPIOC):
		if(pullupState == PULL_UP_ACTIVATE)
		{
			PORTC |= (1 << pins);
		}
		else
		{
			PORTC &= ~(1 << pins);
		}
		break;
	case (GPIOD):
		if(pullupState == PULL_UP_ACTIVATE)
		{
			PORTD |= (1 << pins);
		}
		else
		{
			PORTD &= ~(1 << pins);
		}
		break;
	default:
		break;
	}
}
