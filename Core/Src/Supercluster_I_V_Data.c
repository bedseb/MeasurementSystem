/****************************************************************************************************/
//
//							Name of file: Supercluster_I_V_Data.c
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description: General library for reading of the current and voltage measurements
//		done using ADC (please update respective HAL library according to hardware version)
//
//		Revision:	0.1 Initial release - general file structure made, functions initially working
//
//		Notes:		- File ready for HIL tests only
//
//							All rights reserved to Supercluster
//
/****************************************************************************************************/



/*************************************** Libraries/Header files *************************************/

#include "Supercluster_I_V_Data.h"

/*************************************** Global variables *******************************************/

uint32_t inter_prev_V;
uint32_t inter_prev_I;


volatile uint16_t BufforData[2];

/*************************************** Functions **************************************************/

/****************************************************************************************************/
//
//			Name of function: Supercluster_Data_Init_I_V()
//			Description: General function for initialization allowing to read
//			current and voltage value
//
//			Input:	-
//			Output: -
//
/****************************************************************************************************/
void Supercluster_Data_Init_I_V()
{
	// Init Global variables
	inter_prev_I = 0;
	inter_prev_V = 0;

	// Init HAL
	Supercluster_Init_HAL_I_V();
}// Works OK
/****************************************************************************************************/
//
//			Name of function: Supercluster_Filter_I(uint16_t in, uint16_t *out)
//			Description: Function filtering given current
//
//			Input: uint16_t in - input value of current from ADC [value from 0 to ADC_resolution]
//				   uint16_t *out - pointer to the memory location where the filtered current measurement will be saved
//			Output: -
//
/****************************************************************************************************/
void Supercluster_Filter_I(uint16_t in, uint16_t *out)
{
	uint32_t inter_in, inter_out;
	inter_in = in;
	inter_out = WEIGHT_PREV_I * inter_prev_I + WEIGHT_NEW_I * inter_in;
	*out = inter_out>>I_BITS_SHIFT;
	inter_prev_I = inter_out>>I_BITS_SHIFT;
}//Works OK
/****************************************************************************************************/
//
//			Name of function: Supercluster_Filter_V(uint16_t in, uint16_t *out)
//			Description: Function filtering given voltage
//
//			Input: uint16_t in - input value of voltage from ADC [value from 0 to ADC_resolution]
//				   uint16_t *out - pointer to the memory location where the filtered volatge measurement will be saved
//			Output: -
//
/****************************************************************************************************/
void Supercluster_Filter_V(uint16_t in, uint16_t *out)
{
	uint32_t inter_in, inter_out;
	inter_in = in;
	inter_out = WEIGHT_PREV_V * inter_prev_V + WEIGHT_NEW_V * inter_in;
	*out = inter_out>>V_BITS_SHIFT;
	inter_prev_V = inter_out>>V_BITS_SHIFT;
}//Works OK
/****************************************************************************************************/
//
//			Name of function:  Supercluster_Get_Data_I(float *data)
//			Description: Function returning calibrated current in Ampers [A]
//			Remark: function intended to be called each xx [ms] for filter to work propperly,
//			otherwise simple average from previous values will be calculated
//
//			Input:	float *data - pointer to the memory location where the current value  will be saved
//			Output: -
//
/****************************************************************************************************/
void Supercluster_Get_Data_I(float *data)
{
		uint16_t new_I;
		uint16_t filtered_I;
		// Get new data from HAL layer
		Supercluster_Get_HAL_I(&new_I);
		// Filter the data incoming
		Supercluster_Filter_I(new_I, &filtered_I);
		// Calculate float value returned
		*data = (float)  I_DATA_GAIN* ( filtered_I + I_DATA_OFFSET);
}//Works OK
/****************************************************************************************************/
//
//			Name of function: Supercluster_Get_Data_V(float *data)
//			Description: Function returning calibrated voltage in Volts [V]
//			Remark: function intended to be called each xx [ms] for filter to work propperly,
//			otherwise simple average from previous values will be calculated
//
//			Input:	float *data - pointer to the memory location where the voltage value  will be saved
//			Output: -
//
/****************************************************************************************************/
void Supercluster_Get_Data_V(float *data)
{
	uint16_t new_V;
	uint16_t filtered_V;
	// Get new data from HAL layer
	Supercluster_Get_HAL_V(&new_V);
	// Filter the data incoming
	Supercluster_Filter_V(new_V, &filtered_V);
	// Calculate float value returned
	*data = (float)  V_DATA_GAIN * (filtered_V + V_DATA_OFFSET);
}//Works OK
/*************************************** End of file ************************************************/
