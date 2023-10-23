/****************************************************************************************************/
//
//							Name of file: Supercluster_GPS_Data.h
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description: General library for reading and parsing data from GPS using UART
//					 (please update respective HAL library according to hardware version)
//
//		Revision:	0.1 Initial release - general file structure made, functions initially working
//
//		Notes:		- File ready for HIL tests only
//
//							All rights reserved to Supercluster
//
/****************************************************************************************************/

#ifndef INC_SUPERCLUSTER_GPS_DATA_H_
#define INC_SUPERCLUSTER_GPS_DATA_H_

/*************************************** Libraries/Header files *************************************/
#include "Supercluster_GPS_HAL.h"
#include <string.h>
#include "stdlib.h"
#include <stdint.h>

/*************************************** Definitions ************************************************/

#define GPS_SIZE_BUFFER_RX 200
#define GPS_SIZE_BUFFER_WORKING 100


/*************************************** Global variables *******************************************/

volatile uint8_t GPS_ReceivedChar;

/*************************************** Type definitions and unions ********************************/

/************************************************************************************************
			Name of structure: GPS_Data
			Description: This structure hold the data from GPS
************************************************************************************************/
typedef struct
{
	uint8_t sec;						//***************************
	uint8_t hours;						//***************************
	uint8_t minutes;					//******DATE AND TIME********
	uint8_t days;						//***************************
	uint8_t months;						//***************************
	uint8_t years;						//***************************

	float lattitude;					//***************************
	char lat_direction;					//***************************
	float longitude;					//******LOCALIZATION*********
	char long_direction;				//***************************
	float altitude;						//***************************
	float True_course;

	float vel_knots;					//********VELOCITY***********
	float vel_km;						//***************************

	uint8_t satellites_inView;			//***************************
	uint8_t quantity_Satellites;		//***************************
	uint8_t quality_FIX;				//***************************   // 0 - no Fix, 1 - Fix, 2 - Dif. Fix
	uint8_t mode_FIX;					//***SATELLITES PARAMETERS***	// 1 - no Fiz, 2 - 2D, 3 - 3D
	float dop;							//***************************	// Dilution of precision
	float vDop;							//***************************	// Dilution of precision for flat coords
	float hDop;							//***************************	// Dilution of precision for height

	uint8_t SNR[24]; 					//***************************	// Signal to noise ratio
	uint8_t SV_PRN_number[24];			//***************************	//SV PRN number
	uint8_t elevation[24];				//***************************   //Elevation in degrees, 90 maximum
	uint8_t azimuth[24];				//***************************	//Azimuth, degrees from true north, 000 to 359

}GPS_Data;


/************************************************************************************************
//			Name of structure: GPS_buffer
//			Description: This structure hold the circular buffor data responsible for receiving GPS data
************************************************************************************************/
typedef struct
{
	uint8_t bufferRX[GPS_SIZE_BUFFER_RX];				//***************************
	uint8_t Head;										//****CIRCULAR BUFFER********
	uint8_t Tail;										//***************************
	uint8_t Line;										//***************************
	uint8_t buffer_Working[GPS_SIZE_BUFFER_WORKING];	//***************************
}GPS_buffer;

GPS_buffer Circular_Buffer;
GPS_Data GPS_Measurement;


/*************************************** Functions **************************************************/
void Supercluster_Data_Init_GPS();
void Supercluster_ReceiveUartChar_GPS(GPS_buffer *buffer);
void Supercluster_Loop_GPS(GPS_Data *data, GPS_buffer *buffer);
uint8_t Supercluster_IsFix_GPS(GPS_Data *data);
void Supercluster_UART_GPS_Callback(UART_HandleTypeDef *huart);

/*************************************** End of file ************************************************/

#endif /* INC_SUPERCLUSTER_GPS_DATA_H_ */
