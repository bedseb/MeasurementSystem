/****************************************************************************************************/
//
//							Name of file: Supercluster_I_V_HAL.h
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
#ifndef INC_SUPERCLUSTER_I_V_HAL_H_
#define INC_SUPERCLUSTER_I_V_HAL_H_

/*************************************** Libraries/Header files *************************************/
#include "main.h"
#include "adc.h"
#include "dma.h"

/*************************************** Definitions ************************************************/

#define I_DATA_OFFSET -580.0f			// Offset for changing the ADC value to current measurement
#define I_DATA_GAIN 17.0f/4096.0f*0.2f	// Gain for changing the ADC value to current measurement
#define V_DATA_OFFSET -15.0f			// Offset for changing the ADC value to voltage measurement
#define V_DATA_GAIN 3.3f/4096.0f		// Gain for changing the ADC value to voltage measurement

/*************************************** Type definitions and unions ********************************/

//none

/*************************************** Global variables *******************************************/

//none

/*************************************** Functions **************************************************/
void Supercluster_Init_HAL_I_V();
void Supercluster_Get_HAL_V(uint16_t *data);
void Supercluster_Get_HAL_I(uint16_t *data);
/*************************************** End of file ************************************************/
#endif /* INC_SUPERCLUSTER_I_V_DATA_H_ */
