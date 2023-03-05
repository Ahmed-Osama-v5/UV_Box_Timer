/*
 * main.c
 *
 * Created: 3/3/2023 9:53:26 PM
 *  Author: Osama
 */ 

#define F_CPU	8000000UL
#include <xc.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "config.h"
#include "dio.h"
#include "timer.h"
#include "eeprom.h"

#define MINUTE_EE_ADDR			0x00
#define SECOND_EE_ADDR			0x01

#define BTN_CONFIDENCE_PRESSED	10
#define ONE_SEC_OVS				122
#define DEFAULT_MINUTES			1
#define DEFAULT_SECONDS			0

typedef enum
{
	OK_BTN,
	UP_BTN,
	DN_BTN
}BTN_t;

typedef enum
{
	STOP,
	RUN
}SYSTEM_STATE_t;

/* CA segment LUT */
const uint8_t segArray[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
	
SYSTEM_STATE_t systemState = STOP;
uint16_t OK_BTN_ConfidenceLow = 0, UP_BTN_ConfidenceLow = 0, DN_BTN_ConfidenceLow = 0;
volatile uint8_t minutes = 1, seconds = 0, ovfs = 0;
uint8_t storedMinutes = 0, storedSeconds = 0;

void systemInit(void);
void segmentWrite(uint8_t data);
void displayWrite();
uint8_t isBTN_Pressed(BTN_t btn);
void timerCBK(void);

int main(void)
{
	systemInit();
	sei();
	
    while(1)
    {
		displayWrite();
		switch(systemState)
		{
		case STOP:
			if(isBTN_Pressed(UP_BTN) == HIGH)
			{
				if(seconds < 59)
					seconds++;
				else
				{
					seconds = 0;
					if(minutes < 59)
						minutes++;
					else
						minutes = 0;
				}
			}
			else
			{
				/* Do nothing */
			}
			if(isBTN_Pressed(DN_BTN) == HIGH)
			{
				if(seconds > 0)
				seconds--;
				else
				{
					seconds = 59;
					if(minutes > 0)
					minutes--;
					else
					minutes = 59;
				}
			}
			else
			{
				/* Do nothing */
			}
			if(isBTN_Pressed(OK_BTN) == HIGH)
			{
				storedMinutes = minutes;
				storedSeconds = seconds;
				/* Start timer */
				Timer_Start(TIMER_CH0, 0);
				/* Engage relay */
				DIO_Write(RELAY_GPIO, RELAY_PIN, HIGH);
				/* Turn on LED */
				DIO_Write(LED_GPIO, LED_PIN, HIGH);
				systemState = RUN;
			}
			else
			{
				/* Do nothing */
			}
			break;
		case RUN:
			break;
		default:
			/* Do nothing */
			break;
		}
    }
}

void timerCBK(void)
{
	if(ovfs < ONE_SEC_OVS)
		ovfs++;
	else
	{
		ovfs = 0;
		if(seconds > 0)
		{
			seconds--;
		}
		else
		{
			if(minutes > 0)
			{
				minutes--;
				seconds = 59;
			}
			else
			{
				/* timer expired */
				systemState = STOP;
				/* Disengage relay */
				DIO_Write(RELAY_GPIO, RELAY_PIN, LOW);
				/* Turn off LED */
				DIO_Write(LED_GPIO, LED_PIN, LOW);
				minutes = storedMinutes;
				seconds = storedSeconds;
				Timer_Stop(TIMER_CH0);
				/* Store timer value into EEPROM */
				EEPROM_update(MINUTE_EE_ADDR, minutes);
				EEPROM_update(SECOND_EE_ADDR, seconds);
			}
		}
		
	}
}

uint8_t isBTN_Pressed(BTN_t btn)
{
	uint8_t btnState = HIGH;
	switch(btn)
	{
	case OK_BTN:
		DIO_Read(OK_BTN_GPIO, OK_BTN_PIN, &btnState);
		if(btnState == LOW)
		{
			if(OK_BTN_ConfidenceLow < BTN_CONFIDENCE_PRESSED)
				OK_BTN_ConfidenceLow++;
			else
			{
				OK_BTN_ConfidenceLow = 0;
				return HIGH;
			}
		}
		else
			return LOW;
		break;
	case UP_BTN:
		DIO_Read(UP_BTN_GPIO, UP_BTN_PIN, &btnState);
		if(btnState == LOW)
		{
			if(UP_BTN_ConfidenceLow < BTN_CONFIDENCE_PRESSED)
				UP_BTN_ConfidenceLow++;
			else
			{
				UP_BTN_ConfidenceLow = 0;
				return HIGH;
			}
		}
		else
			return LOW;
		break;
	case DN_BTN:
		DIO_Read(DN_BTN_GPIO, DN_BTN_PIN, &btnState);
		if(btnState == LOW)
		{
			if(DN_BTN_ConfidenceLow < BTN_CONFIDENCE_PRESSED)
				DN_BTN_ConfidenceLow++;
			else
			{
				DN_BTN_ConfidenceLow = 0;
				return HIGH;
			}
		}
		else
			return LOW;
		break;
	default:
		return LOW;
		break;
	}
	return LOW;
}

void segmentWrite(uint8_t data)
{
	if(segArray[data] & 0x01)
		DIO_Write(SEGMENT_A_GPIO, SEGMENT_A_PIN, HIGH);
	else
		DIO_Write(SEGMENT_A_GPIO, SEGMENT_A_PIN, LOW);
		
	if(segArray[data] & 0x02)
		DIO_Write(SEGMENT_B_GPIO, SEGMENT_B_PIN, HIGH);
	else
		DIO_Write(SEGMENT_B_GPIO, SEGMENT_B_PIN, LOW);
		
	if(segArray[data] & 0x04)
		DIO_Write(SEGMENT_C_GPIO, SEGMENT_C_PIN, HIGH);
	else
		DIO_Write(SEGMENT_C_GPIO, SEGMENT_C_PIN, LOW);
		
	if(segArray[data] & 0x08)
		DIO_Write(SEGMENT_D_GPIO, SEGMENT_D_PIN, HIGH);
	else
		DIO_Write(SEGMENT_D_GPIO, SEGMENT_D_PIN, LOW);
		
	if(segArray[data] & 0x10)
		DIO_Write(SEGMENT_E_GPIO, SEGMENT_E_PIN, HIGH);
	else
		DIO_Write(SEGMENT_E_GPIO, SEGMENT_E_PIN, LOW);
		
	if(segArray[data] & 0x20)
		DIO_Write(SEGMENT_F_GPIO, SEGMENT_F_PIN, HIGH);
	else
		DIO_Write(SEGMENT_F_GPIO, SEGMENT_F_PIN, LOW);
		
	if(segArray[data] & 0x40)
		DIO_Write(SEGMENT_G_GPIO, SEGMENT_G_PIN, HIGH);
	else
		DIO_Write(SEGMENT_G_GPIO, SEGMENT_G_PIN, LOW);
}

void displayWrite()
{
	/* Display minutes tens */
	segmentWrite((uint8_t)((minutes / 10) % 10));
	DIO_Write(DISPLAY_1_GPIO, DISPLAY_1_PIN, LOW);
	DIO_Write(DISPLAY_2_GPIO, DISPLAY_2_PIN, HIGH);
	DIO_Write(DISPLAY_3_GPIO, DISPLAY_3_PIN, HIGH);
	DIO_Write(DISPLAY_4_GPIO, DISPLAY_4_PIN, HIGH);
	_delay_ms(3);
	
	/* Display minutes ones */
	segmentWrite((uint8_t)(minutes % 10));
	DIO_Write(SEGMENT_DP_GPIO, SEGMENT_DP_PIN, LOW);
	DIO_Write(DISPLAY_1_GPIO, DISPLAY_1_PIN, HIGH);
	DIO_Write(DISPLAY_2_GPIO, DISPLAY_2_PIN, LOW);
	DIO_Write(DISPLAY_3_GPIO, DISPLAY_3_PIN, HIGH);
	DIO_Write(DISPLAY_4_GPIO, DISPLAY_4_PIN, HIGH);
	_delay_ms(3);
	
	/* Display seconds tens */
	segmentWrite((uint8_t)((seconds / 10) % 10));
	DIO_Write(SEGMENT_DP_GPIO, SEGMENT_DP_PIN, HIGH);
	DIO_Write(DISPLAY_1_GPIO, DISPLAY_1_PIN, HIGH);
	DIO_Write(DISPLAY_2_GPIO, DISPLAY_2_PIN, HIGH);
	DIO_Write(DISPLAY_3_GPIO, DISPLAY_3_PIN, LOW);
	DIO_Write(DISPLAY_4_GPIO, DISPLAY_4_PIN, HIGH);
	_delay_ms(3);
	
	/* Display seconds ones */
	segmentWrite((uint8_t)(seconds % 10));
	DIO_Write(DISPLAY_1_GPIO, DISPLAY_1_PIN, HIGH);
	DIO_Write(DISPLAY_2_GPIO, DISPLAY_2_PIN, HIGH);
	DIO_Write(DISPLAY_3_GPIO, DISPLAY_3_PIN, HIGH);
	DIO_Write(DISPLAY_4_GPIO, DISPLAY_4_PIN, LOW);
	_delay_ms(3);
	
}

void systemInit(void)
{
	/* Init GPIOs */
	
	/* Init LED */
	DIO_Cfg_s dioStr = {0};
	dioStr.GPIO = LED_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = LED_PIN;
	DIO_init(&dioStr);
	
	/* Init Relay */
	dioStr.GPIO = RELAY_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = RELAY_PIN;
	DIO_init(&dioStr);
	
	/* Init BTNs */
	dioStr.GPIO = OK_BTN_GPIO;
	dioStr.dir = INPUT;
	dioStr.pins = OK_BTN_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = UP_BTN_GPIO;
	dioStr.dir = INPUT;
	dioStr.pins = UP_BTN_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = DN_BTN_GPIO;
	dioStr.dir = INPUT;
	dioStr.pins = DN_BTN_PIN;
	DIO_init(&dioStr);
	
	/* Init 7-seg */
	dioStr.GPIO = SEGMENT_A_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_A_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = SEGMENT_B_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_B_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = SEGMENT_C_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_C_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = SEGMENT_D_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_D_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = SEGMENT_E_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_E_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = SEGMENT_F_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_F_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = SEGMENT_G_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_G_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = SEGMENT_DP_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = SEGMENT_DP_PIN;
	DIO_init(&dioStr);
	
	
	dioStr.GPIO = DISPLAY_1_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = DISPLAY_1_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = DISPLAY_2_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = DISPLAY_2_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = DISPLAY_3_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = DISPLAY_3_PIN;
	DIO_init(&dioStr);
	
	dioStr.GPIO = DISPLAY_4_GPIO;
	dioStr.dir = OUTPUT;
	dioStr.pins = DISPLAY_4_PIN;
	DIO_init(&dioStr);
	
	/* Init timer0 */
	Timer_cfg_s timer0 = {0};
	timer0.Timer_CH_NO = TIMER_CH0;
	timer0.Timer_Mode = TIMER_MODE;
	timer0.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
	timer0.Timer_Prescaler = TIMER_PRESCALER_256;
	timer0.CBK_Ptr = timerCBK;
	Timer_Init(&timer0);
	
	/* Retrieve timer value from EEPROM */
	EEPROM_read(MINUTE_EE_ADDR, &storedMinutes);
	EEPROM_read(SECOND_EE_ADDR, &storedSeconds);
	minutes = storedMinutes;
	seconds = storedSeconds;
}