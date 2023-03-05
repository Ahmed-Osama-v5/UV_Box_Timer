#ifndef CONFIG_H
#define CONFIG_H


#include "dio.h"

/* BTNs DIO config */
#define OK_BTN_GPIO	GPIOB
#define OK_BTN_PIN	1

#define UP_BTN_GPIO	GPIOB
#define UP_BTN_PIN	2

#define DN_BTN_GPIO	GPIOB
#define DN_BTN_PIN	0


/* LED DIO config */
#define LED_GPIO	GPIOD
#define LED_PIN		7


/* RELAY DIO config */
#define RELAY_GPIO	GPIOD
#define RELAY_PIN	6


/* 7-seg DIO config */
#define SEGMENT_A_GPIO	GPIOC
#define SEGMENT_A_PIN	5

#define SEGMENT_B_GPIO	GPIOC
#define SEGMENT_B_PIN	2

#define SEGMENT_C_GPIO	GPIOD
#define SEGMENT_C_PIN	2

#define SEGMENT_D_GPIO	GPIOD
#define SEGMENT_D_PIN	4

#define SEGMENT_E_GPIO	GPIOD
#define SEGMENT_E_PIN	5

#define SEGMENT_F_GPIO	GPIOC
#define SEGMENT_F_PIN	3

#define SEGMENT_G_GPIO	GPIOD
#define SEGMENT_G_PIN	1

#define SEGMENT_DP_GPIO	GPIOD
#define SEGMENT_DP_PIN	3

#define DISPLAY_1_GPIO	GPIOD
#define DISPLAY_1_PIN	0

#define DISPLAY_2_GPIO	GPIOC
#define DISPLAY_2_PIN	4

#define DISPLAY_3_GPIO	GPIOC
#define DISPLAY_3_PIN	1

#define DISPLAY_4_GPIO	GPIOC
#define DISPLAY_4_PIN	0

#endif // CONFIG_H
