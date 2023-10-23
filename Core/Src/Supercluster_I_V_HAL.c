/****************************************************************************************************/
//
//							Name of file: Supercluster_I_V_HAL.c
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description: Hardware abstraction layer for reading of the current and voltage measurements
//					 done using ADC (please update this library according to hardware version)
//
//
//		Revision:	0.1 Initial release - general file structure made, functions initially working
//
//		Notes:		- File ready for HIL tests only
//
//							All rights reserved to Supercluster
//
/****************************************************************************************************/



/*************************************** Libraries/Header files *************************************/
#include "Supercluster_I_V_HAL.h"

/*************************************** Global variables *******************************************/

volatile uint16_t BufforData[2];

/*************************************** Functions **************************************************/


/****************************************************************************************************
//
//			Name of function: Supercluster_Init_HAL_I_V()
//			Description: General function for initialization of the hardware layer allowing to read
//			current and voltage value
//
//			Input:	-
//			Output: -
//
*******************************************************************************************************/
void Supercluster_Init_HAL_I_V()
{
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)BufforData, 2);
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_Get_HAL_I(uint16_t *data)
//			Description: Function to get actually value of current from ADC
//
//			Input:	uint16_t *data - pointer to the memory location where the current measurement will be saved
//			Output: -
//
****************************************************************************************************/
void Supercluster_Get_HAL_I(uint16_t *data)
{
	*data = (BufforData[0]);
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_Get_HAL_V(uint16_t *data)
//			Description: Function to get actually value of voltage from ADC
//
//			Input:	uint16_t *data - pointer to the memory location where the current measurement will be saved
//			Output: -
//
****************************************************************************************************/
void Supercluster_Get_HAL_V(uint16_t *data)
{
	*data = (BufforData[1]);
}//Works OK
/*************************************** End of file ************************************************/
