/****************************************************************************************************/
//
//							Name of file: Supercluster_PWM_Data.c
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description: General library for generate PWM using timers
//					 (please update respective HAL library according to hardware version)
//
//		Revision:	0.1 Initial release - general file structure made, functions initially working
//
//		Notes:		- File ready for HIL tests only
//
//							All rights reserved to Supercluster
//
/****************************************************************************************************/
/*************************************** Libraries/Header files *************************************/
#include <Supercluster_PWM_data.h>

/*************************************** Global variables *******************************************/
//none

/*************************************** Functions **************************************************/
/****************************************************************************************************
//
//			Name of function: Supercluster_PWM_Init()
//			Description: General function for initialization allowing to generate PWM using timers
//
//			Input:	-
//			Output: -
//
****************************************************************************************************/
void Supercluster_PWM_Init()
{
	Supercluster_Init_PWM_HAL();
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_PWM_Generate(uint8_t PWM_ChannelNumber, int32_t duty_cycle)
//			Description: General function for generate PWM according the duty cycle set between (-100, 100)
//						for -100 the function generate high state for 1ms
//						for  100 the function generate high state for 2ms
//
//			Input:	uint8_t PWM_ChannelNumber 	- channel number, user can choose one of five channels
//					int32_t duty_cycle			- duty cycle, the user can put in the limit (-100, 100)
//			Output: 1 when the channel number was correct
//					0 when the channel number was put incorrect
//
****************************************************************************************************/
uint8_t Supercluster_PWM_Generate(uint8_t PWM_ChannelNumber, int32_t duty_cycle)
{
	TIM_HandleTypeDef tim_pwm;
	uint8_t pwm_channel;
	uint32_t pwm_duty_us;

	if(duty_cycle > 100)	duty_cycle = 100;
	else if(duty_cycle < -100)	duty_cycle = -100;

	switch (PWM_ChannelNumber)
	{
	case 1:
		tim_pwm = PMW_OUT1;
		pwm_channel = PMW_CH1;
		break;
	case 2:
		tim_pwm = PMW_OUT2;
		pwm_channel = PMW_CH2;
		break;
	case 3:
		tim_pwm = PMW_OUT3;
		pwm_channel = PMW_CH3;
		break;
	case 4:
		tim_pwm = PMW_OUT4;
		pwm_channel = PMW_CH4;
		break;
	case 5:
		tim_pwm = PMW_OUT5;
		pwm_channel = PMW_CH5;
		break;
	default:
		return 0;
	}
	pwm_duty_us = (PWM_MIN_US + PWM_MAX_US)/2 + (5 * duty_cycle); // Tutaj jako jedno makro
	Supercluster_Set_PWM_HAL(&tim_pwm, pwm_channel, pwm_duty_us);
	return 1;
}//Works OK
/*************************************** End of file ************************************************/
