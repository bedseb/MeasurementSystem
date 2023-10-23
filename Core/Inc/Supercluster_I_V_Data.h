/****************************************************************************************************/
//
//							file: Supercluster_I_V_Data.h
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

#ifndef INC_SUPERCLUSTER_I_V_DATA_H_
#define INC_SUPERCLUSTER_I_V_DATA_H_
/*************************************** Libraries/Header files *************************************/
#include "Supercluster_I_V_HAL.h"
#include <stdint.h>
/*************************************** Definitions ************************************************/


// Note: Sum of PREV & NEW must be equal to 16. If greater keep in mind multiplies of 2
#define WEIGHT_PREV_V 15
#define WEIGHT_NEW_V 1
#define V_BITS_SHIFT 4

#define WEIGHT_PREV_I 15
#define WEIGHT_NEW_I 1
#define I_BITS_SHIFT 4
/*************************************** Global variables *******************************************/
// None
/*************************************** Functions **************************************************/
void Supercluster_Data_Init_I_V();
void Supercluster_Get_Data_I(float *data);
void Supercluster_Get_Data_V(float *data);
/*************************************** End of file ************************************************/
#endif /* INC_SUPERCLUSTER_I_V_DATA_H_ */
