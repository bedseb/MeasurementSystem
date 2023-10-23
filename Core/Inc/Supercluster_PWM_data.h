/****************************************************************************************************/
//
//							Name of file: Supercluster_PWM_Data.h
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
#ifndef INC_SUPERCLUSTER_PWM_DATA_H
#define INC_SUPERCLUSTER_PWM_DATA_H

/*************************************** Libraries/Header files *************************************/
#include "Supercluster_PWM_HAL.h"

/*************************************** Definitions ************************************************/
#define PWM_MAX_US 2000			//max duty cycle 2ms
#define PWM_MIN_US 1000			//min duty cycle 1ms

/*************************************** Global variables *******************************************/
//none

/*************************************** Type definitions and unions ********************************/
//none

/*************************************** Functions **************************************************/
void Supercluster_PWM_Init(void);
uint8_t Supercluster_PWM_Generate(uint8_t PWM_ChannelNumber, int32_t duty_cycle);
/*************************************** End of file ************************************************/
#endif
