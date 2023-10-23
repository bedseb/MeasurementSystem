/****************************************************************************************************/
//
//							Name of file: Supercluster_PWM_HAL.c
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description: Hardware abstraction layer for PWM generate using timers
//					 (please update this library according to the hardware version.
//					  You must adjust your timer settings so that the pulse frequency is 1 us
//				 	  For example for the 80MHZ clock, prescaler is 80-1, and counter period 20000-1)
//
//		Revision:	0.1 Initial release - general file structure made, functions initially working
//
//		Notes:		- File ready for HIL tests only
//
//							All rights reserved to Supercluster
//
/****************************************************************************************************/
/*************************************** Libraries/Header files *************************************/
#include "Supercluster_PWM_HAL.h"

/*************************************** Global variables *******************************************/
//none

/*************************************** Functions **************************************************/
/****************************************************************************************************
//
//			Name of function: Supercluster_Init_PWM_HAL()
//			Description: General function for initialization of the hardware layer allowing to generate
//			PWM using timers
//
//			Input:	-
//			Output: -
//
****************************************************************************************************/
void Supercluster_Init_PWM_HAL()
{
	__HAL_TIM_SET_COMPARE(&PMW_OUT1, PMW_CH1, PWM_MIN_US);
	HAL_TIM_PWM_Start(&PMW_OUT1, PMW_CH1);

	__HAL_TIM_SET_COMPARE(&PMW_OUT2, PMW_CH2, PWM_MIN_US);
	HAL_TIM_PWM_Start(&PMW_OUT2, PMW_CH2);

	__HAL_TIM_SET_COMPARE(&PMW_OUT3, PMW_CH3, PWM_MIN_US);
	HAL_TIM_PWM_Start(&PMW_OUT3, PMW_CH3);

	__HAL_TIM_SET_COMPARE(&PMW_OUT4, PMW_CH4, PWM_MIN_US);
	HAL_TIM_PWM_Start(&PMW_OUT4, PMW_CH4);

	__HAL_TIM_SET_COMPARE(&PMW_OUT5, PMW_CH5, PWM_MIN_US);
	HAL_TIM_PWM_Start(&PMW_OUT5, PMW_CH5);
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_Set_PWM_HAL(TIM_HandleTypeDef *htim, uint8_t channel, uint32_t PWM_us_value)
//			Description: Function to generate PWM
//
//			Input:	TIM_HandleTypeDef *htim - A pointer to structure that hold timer parameters
//					uint8_t channel			- A timer channel number
//					uint32_t PWM_us_value 	- A time in us when the high state is set
//			Output: -
//
***************************************************************************************************/
void Supercluster_Set_PWM_HAL(TIM_HandleTypeDef *htim, uint8_t channel, uint32_t PWM_us_value)
{
	__HAL_TIM_SET_COMPARE(htim, channel, PWM_us_value);
}//Works OK
/*************************************** End of file ************************************************/
