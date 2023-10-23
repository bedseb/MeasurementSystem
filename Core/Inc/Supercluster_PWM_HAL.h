/****************************************************************************************************/
//
//							Name of file: Supercluster_PWM_HAL.c
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description: Hardware abstraction layer for PWM generate using timers
//					 (please update this library according to the hardware version.
//					  You must adjust your timer settings so that the pulse frequency is 1 us and counter period 20ms
//				 	  For example for the 80MHZ clock, prescaler is 80-1, and counter period 20000-1)
//
//		Revision:	0.1 Initial release - general file structure made, functions initially working
//
//		Notes:		- File ready for HIL tests only
//
//							All rights reserved to Supercluster
//
/****************************************************************************************************/
#ifndef INC_SUPERCLUSTER_PWM_HAL_H
#define INC_SUPERCLUSTER_PWM_HAL_H
/*************************************** Libraries/Header files *************************************/
#include "tim.h"
#include "Supercluster_PWM_data.h"
/*************************************** Definitions ************************************************/
#define PMW_OUT1 htim2				//Timer used to generate PWM1
#define PMW_OUT2 htim2				//Timer used to generate PWM2
#define PMW_OUT3 htim3				//Timer used to generate PWM3
#define PMW_OUT4 htim3				//Timer used to generate PWM4
#define PMW_OUT5 htim3				//Timer used to generate PWM5

#define PMW_CH1 TIM_CHANNEL_1		//Channel of timer used to generate PWM1
#define PMW_CH2 TIM_CHANNEL_2		//Channel of timer used to generate PWM2
#define PMW_CH3 TIM_CHANNEL_1		//Channel of timer used to generate PWM3
#define PMW_CH4 TIM_CHANNEL_2		//Channel of timer used to generate PWM4
#define PMW_CH5 TIM_CHANNEL_3		//Channel of timer used to generate PWM5
/*************************************** Global variables *******************************************/
//none
/*************************************** Type definitions and unions ********************************/
//none
/*************************************** Functions **************************************************/

void Supercluster_Init_PWM_HAL(void);
void Supercluster_Set_PWM_HAL(TIM_HandleTypeDef *htim, uint8_t channel, uint32_t duty_cycle);

/*************************************** End of file ************************************************/
#endif
