/****************************************************************************************************/
//
//							Name of file: Supercluster_GPS_HAL.h
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description: Hardware abstraction layer for GPS module done using UART
//					 (please update this library according to hardware version)
//
//		Revision:	0.1 Initial release - general file structure made, functions initially working
//
//		Notes:		- File ready for HIL tests only
//
//							All rights reserved to Supercluster
//
/****************************************************************************************************/

#ifndef INC_SUPERCLUSTER_GPS_HAL_H_
#define INC_SUPERCLUSTER_GPS_HAL_H_


/*************************************** Libraries/Header files *************************************/
#include "main.h"
#include "usart.h"



/*************************************** Definitions ************************************************/

#define GPS_UART huart1			//Set corresponding UART number that connected to GPS module
#define Supercluster_UART_Receive_IT_GPS()	HAL_UART_Receive_IT(&GPS_UART, (uint8_t*)&GPS_ReceivedChar, 1);

/*************************************** Global variables *******************************************/

//none
/*************************************** Type definitions and unions ********************************/

//none
/*************************************** Functions **************************************************/

//none
/*************************************** End of file ************************************************/
#endif /* INC_SUPERCLUSTER_GPS_HAL_H_ */
