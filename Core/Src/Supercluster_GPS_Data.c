/****************************************************************************************************/
//
//							Name of file: Supercluster_GPS_Data.c
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
/*************************************** Libraries/Header files *************************************/
#include "Supercluster_GPS_Data.h"

/*************************************** Global variables *******************************************/
//none

/*************************************** Functions **************************************************/
//none

/****************************************************************************************************
//
//			Name of function: strtoke(char *str, const char *delim)
//			Description: This function behaves like strtok() except that it returns empty tokens also.
//						Found on https://stackoverflow.com/questions/42315585/split-string-into-tokens-in-c-when-there-are-2-delimiters-in-a-row
//
//			Input: char *str - pointer for the string to parsing | const char *delim - character that divides string
//			Output: string between two delimiters
//
****************************************************************************************************/
char* strtoke(char *str, const char *delim)
{
  static char *start = NULL; /* stores string str for consecutive calls */
  char *token = NULL; /* found token */
  /* assign new start in case */
  if (str) start = str;
  /* check whether text to parse left */
  if (!start) return NULL;
  /* remember current start as found token */
  token = start;
  /* find next occurrence of delim */
  start = strpbrk(start, delim);
  /* replace delim with terminator and move start to follower */
  if (start) *start++ = '\0';
  /* done */
  return token;
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_Data_Init_GPS()
//			Description: General function for initialization GPS module allowing to read and parse
//			GPS data
//
//			Input:	-
//			Output: -
//
****************************************************************************************************/
void Supercluster_Data_Init_GPS()
{
	//init global variables
	Circular_Buffer.Head = 0;
	Circular_Buffer.Tail = 0;
	Circular_Buffer.Line = 0;

	GPS_Measurement.sec = 0;
	GPS_Measurement.minutes = 0;
	GPS_Measurement.hours = 0;
	GPS_Measurement.days = 0;
	GPS_Measurement.months = 0;
	GPS_Measurement.years = 0;

	GPS_Measurement.lattitude = 0;
	GPS_Measurement.longitude = 0;
	GPS_Measurement.lat_direction = '-';
	GPS_Measurement.long_direction = '-';
	GPS_Measurement.altitude = 0;

	GPS_Measurement.vel_knots = 0;
	GPS_Measurement.vel_km = 0;

	GPS_Measurement.quality_FIX = 0;
	GPS_Measurement.quantity_Satellites = 0;
	GPS_Measurement.mode_FIX = 0;
	GPS_Measurement.dop = 0;
	GPS_Measurement.hDop = 0;
	GPS_Measurement.vDop =0;

	//init HAL
	Supercluster_UART_Receive_IT_GPS();
}//Works OK
/****************************************************************************************************
//
//			Name of function:  Supercluster_ReceiveUartChar_GPS(GPS_buffer *buffer)
//			Description: Function responsible for receiving character from the UART and adding them to the circular buffer
//
//
//			Input: A pointer to structure that holds the buffer data
//			Output: -
//
****************************************************************************************************/
void Supercluster_ReceiveUartChar_GPS(GPS_buffer *buffer)
{
	uint8_t temp_Head;
	temp_Head = (buffer->Head + 1) % GPS_SIZE_BUFFER_RX;

	if(temp_Head == buffer->Tail)
	{
		// lost data
	}
	else
	{
		if(GPS_ReceivedChar == 13)		// IN ASCII 13 = '/CR'
		{
			buffer->Line++;
			buffer->Head = temp_Head;
			buffer->bufferRX[temp_Head] = GPS_ReceivedChar;
		}
		else if (GPS_ReceivedChar == 0 || GPS_ReceivedChar == 10 )			//IN ASCII 0 = NULL, 10 = 'LF'
		{
			//do nothing
		}
		else
		{
			buffer->Head = temp_Head;
			buffer->bufferRX[temp_Head] = GPS_ReceivedChar;
		}
	}

	Supercluster_UART_Receive_IT_GPS();
}//Works OK
/****************************************************************************************************
//
//			Name of function:  Supercluster_GetChar_GPS(GPS_buffer *buffer)
//			Description: Function responsible for getting a character form buffer
//
//
//			Input: GPS_buffer *buffer - A pointer to structure that holds the buffer data
//			Output: The function returns an integer value representing the character from the buffer if available,
//					or -1 if there are no characters to retrieve.
//
****************************************************************************************************/
int8_t Supercluster_GetChar_GPS(GPS_buffer *buffer)
{
	if(buffer->Head == buffer->Tail)		return -1;

	else
	{
		buffer->Tail = (buffer->Tail + 1) % GPS_SIZE_BUFFER_RX;
		return buffer->bufferRX[buffer->Tail];
	}
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_GetLine_GPS(GPS_buffer *buffer)
//			Description: Function responsible for get NMEA sentence from buffer
//
//			Input: GPS_buffer *buffer - A pointer to structure that holds the buffer data
//			Output: -
//
****************************************************************************************************/
void Supercluster_GetLine_GPS(GPS_buffer *buffer)
{
	char temp_Char;
	char *workingLineBuffer = (char*)buffer->buffer_Working;

	if(buffer->Line)
	{
		while((temp_Char = Supercluster_GetChar_GPS(buffer)))
		{
			if(temp_Char == 13 ) break;

			*workingLineBuffer = temp_Char;
			workingLineBuffer++;
		}
		*workingLineBuffer = 0;
		buffer->Line--;
	}
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_ParseNMEA_GPRMC(GPS_Data *data)
//			Description: Function responsible for parsing the NMEA sentence $GPRMC received form the GPS module
//
//			Input: GPS_Data *data  - A pointer to structure that will hold the GPS data
//			Output: -
//
****************************************************************************************************/
void Supercluster_ParseNMEA_GPRMC(GPS_Data *data)
{
	char *ParseData;
	uint32_t temp_Data;

	//Time UTC
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)
	{
		temp_Data = atoi(ParseData);
		data->sec 		= temp_Data % 100;
		data->minutes 	= (temp_Data/100)  % 100;
		data->hours		= (temp_Data/10000) % 100;
	}

	// validity - A-ok, V-invalid
	ParseData = strtoke(NULL, ",");

	//Latitude
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)  		data->lattitude = atof(ParseData);

	//Latitude direction North/South
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)  		data->lat_direction = *ParseData;

	//Longitude
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)  		data->longitude = atof(ParseData);

	//Latitude direction North/South
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)  		data->long_direction = *ParseData;

	//Speed in knots
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)  		data->vel_knots = atof(ParseData);

	//Speed in km/h
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)  		data->vel_km = atof(ParseData);

	//True course
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)  		data->True_course = atof(ParseData);

	//date
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)
	{
		temp_Data = atoi(ParseData);
		data->years = 	(temp_Data % 100) + 2000;
		data->months = 	(temp_Data/100) 	%100;
		data->days = 	(temp_Data/10000)	%100;
	}

	/*
	   10  004.2      Variation
	   11  W          East/West
	   12  *70        checksum
	 */
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_ParseNMEA_GPGGA(GPS_Data *data)
//			Description: Function responsible for parsing the NMEA sentence $GPGGA received from the GPS data
//
//
//			Input: GPS_Data *data  - A pointer to structure that will hold the GPS data
//			Output: -
//
****************************************************************************************************/
void Supercluster_ParseNMEA_GPGGA(GPS_Data *data)
{
	/*
	 * 		eg3. $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
			1    = UTC of Position
			2    = Latitude
			3    = N or S
			4    = Longitude
			5    = E or W
			6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
			7    = Number of satellites in use [not those in view]
			8    = Horizontal dilution of position
			9    = Antenna altitude above/below mean sea level (geoid)
			10   = Meters  (Antenna height unit)
			11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and
				   mean sea level.  -=geoid is below WGS-84 ellipsoid)
			12   = Meters  (Units of geoidal separation)
			13   = Age in seconds since last update from diff. reference station
			14   = Diff. reference station ID#
			15   = Checksum
	 */

	char *ParseData;
	uint32_t temp_Data;

	//****************Time UTC*********************
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)
	{
		temp_Data 		= atoi(ParseData);
		data->sec 		= temp_Data % 100;
		data->minutes 	= (temp_Data/100)  % 100;
		data->hours		= (temp_Data/10000) % 100;
	}

	//****************Latitude*********************
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->lattitude = atof(ParseData);

	//****************Latitude N or S**************
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->lat_direction = *ParseData;

	//****************Longitude*********************
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->longitude = atof(ParseData);

	//****************Longitude E or W**************
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->long_direction = *ParseData;

	//*****GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->quality_FIX = atoi(ParseData);

	//*****Number of satellites in use [not those in view]
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->quantity_Satellites = atoi(ParseData);

	//***** Horizontal Dilution of Precision (HDOP)
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->hDop= atof(ParseData);

	//*****Antenna altitude above/below mean sea level (geoid)
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->altitude= atof(ParseData);

	//*******Height of geoid above WGS84 ellipsoid

	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) ; 		//czy trzeba tą informacje ?

	//13   = Age in seconds since last update from diff. reference station
	//14   = Diff. reference station ID#
	//15   = Checksum

}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_ParseNMEA_GPGSV(GPS_Data *data)
//			Description: This function is responsible for parsing the NMEA sentence '$GPGSV' received from the GPS module.
//
//
//			Input: GPS_Data *data  - A pointer to structure that will hold the GPS data
//			Output: -
//
****************************************************************************************************/
void Supercluster_ParseNMEA_GPGSV(GPS_Data *data)
{
	/*
			eg. $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
				$GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
				$GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D
				$GPGSV,1,1,13,02,02,213,,03,-3,000,,11,00,121,,14,13,172,05*67

			1    = Total number of messages of this type in this cycle
			2    = Message number
			3    = Total number of SVs in view				->>>>>>>>>>>>>>>>>>
			4    = SV PRN number										->>>>
			5    = Elevation in degrees, 90 maximum			//
			6    = Azimuth, degrees from true north, 000 to 359		//
			7    = SNR, 00-99 dB (null when not tracking)   ->>>>>>>>>>>>>>>>>>>
			8-11 = Information about second SV, same as field 4-7
			12-15= Information about third SV, same as field 4-7
			16-19= Information about fourth SV, same as field 4-7
	 *
	 */

	char *ParseData;
	uint8_t msg_number;

	//1    = Total number of messages of this type in this cycle
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) ;

	//2    = Message number
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) msg_number = atoi(ParseData);

	//3    = Total number of SVs in view
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) data->satellites_inView = atoi(ParseData);

	//4    = SV PRN number
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SV_PRN_number[(msg_number-1)*4] = atoi(ParseData);

	//5    = Elevation in degrees, 90 maximum
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->elevation[(msg_number-1)*4] = atoi(ParseData);

	//6    = Azimuth, degrees from true north, 000 to 359
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) 					data->azimuth[(msg_number-1)*4] = atoi(ParseData);

	//7    = SNR, 00-99 dB (null when not tracking)
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SNR[(msg_number-1)*4] = atoi(ParseData);

	//8-11 = Information about second SV, same as field 4-7
	// 8 = SV PRN number
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SV_PRN_number[(msg_number-1)*4 + 1] = atoi(ParseData);

	//9    = Elevation in degrees, 90 maximum
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) 					data->elevation[(msg_number-1)*4 + 1] = atoi(ParseData);

	//10    = Azimuth, degrees from true north, 000 to 359
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->azimuth[(msg_number-1)*4 + 1] = atoi(ParseData);

	//11    = SNR, 00-99 dB (null when not tracking)
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SNR[(msg_number-1)*4 + 1] = atoi(ParseData);

	// 12-15= Information about third SV, same as field 4-7
	//12 = SV PRN number
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SV_PRN_number[msg_number*4 - 2] = atoi(ParseData);

	//13    = Elevation in degrees, 90 maximum
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) 					data->elevation[msg_number*4 - 2] = atoi(ParseData);

	//14    = Azimuth, degrees from true north, 000 to 359
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) 					data->azimuth[msg_number*4 - 2] = atoi(ParseData);

	//15    = SNR, 00-99 dB (null when not tracking)
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SNR[msg_number*4 -2] = atoi(ParseData);

	//16-19= Information about fourth SV, same as field 4-7
	//16 = SV PRN number
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SV_PRN_number[msg_number*4 - 1] = atoi(ParseData);

	//17    = Elevation in degrees, 90 maximum
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) 					data->elevation[msg_number*4 - 1] = atoi(ParseData);

	//18    = Azimuth, degrees from true north, 000 to 359
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0) 					data->azimuth[msg_number*4 - 1] = atoi(ParseData);

	//19    = SNR, 00-99 dB (null when not tracking)
	ParseData = strtoke(NULL, ",");
	if(strlen(ParseData) > 0)					data->SNR[msg_number*4 -1] = atoi(ParseData);

}//Check the data in arrays (SV, SRN, Azimuth, elevation)
/****************************************************************************************************
//
//			Name of function: Supercluster_ParseNMEA_GPS(GPS_Data *data, GPS_buffer *buffer)
//			Description: This function is used to parse GPS data received in NMEA (National Marine Electronics Association) format.
//
//
//			Input: GPS_Data *data	  - A pointer to a structure that will hold the parsed GPS data.
//				   GPS_buffer *buffer - A pointer to structure that holds circular buffer parameters.
//			Output: -
//
****************************************************************************************************/
void Supercluster_ParseNMEA_GPS(GPS_Data *data, GPS_buffer *buffer)
{
	char *ParseData = strtoke((char*)buffer->buffer_Working, ",");

	if		(strcmp(ParseData, "$GPRMC") == 0 )  		Supercluster_ParseNMEA_GPRMC(data);
	else if(strcmp(ParseData, "$GPGGA") == 0 )			Supercluster_ParseNMEA_GPGGA(data);
	else if(strcmp(ParseData, "$GPGSV") == 0 )			Supercluster_ParseNMEA_GPGSV(data);

}//Works OK
/****************************************************************************************************
//
//			Name of function:	Supercluster_IsFix_GPS(GPS_Data *data)
//			Description: This function checks whether the GPS signal has a fix and returns
//						 an integer representing the fix quality.
//
//
//			Input:	GPS_Data *data 			- A pointer to a structure that will hold the parsed GPS data.
//			Output: data->quality_FIX  		- The function returns the fix quality as an integer (0, 1, or 2) based on the GPS signal status.
//
****************************************************************************************************/
uint8_t Supercluster_IsFix_GPS(GPS_Data *data)
{
	return data->quality_FIX;
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_Loop_GPS(GPS_Data *data, GPS_buffer *buffer)
//			Description: This function is the main loop for processing GPS data. The function performs the following tasks:
//							- Calls the function Supercluster_GetLine_GPS(buffer):
//							- Calls the function Supercluster_ParseNMEA_GPS(data, buffer):
//								(Please put this function to body loop)
//
//
//			Input: GPS_Data *data		- A pointer to a structure that will hold the parsed GPS data.
//				   GPS_buffer *buffer 	- A pointer to structure that holds circular buffer parameters
//			Output:  -
//
****************************************************************************************************/
void Supercluster_Loop_GPS(GPS_Data *data, GPS_buffer *buffer)
{
	Supercluster_GetLine_GPS(buffer);
	Supercluster_ParseNMEA_GPS(data, buffer);
}//Works OK
/****************************************************************************************************
//
//			Name of function: Supercluster_UART_GPS_Callback(UART_HandleTypeDef *huart)
//			Description: This function is an interrupt callback used to handle UART receive events
//						specifically for the GPS module. It is called when a new character
//						is received through the UART interface.
//
//						(Please put that function in HAL_UART_RxCpltCallback )
//
//
//			Input: UART_HandleTypeDef *huart - A pointer to the UART handle
//			Output:  -
//
****************************************************************************************************/
void Supercluster_UART_GPS_Callback(UART_HandleTypeDef *huart)
{
	if(huart == &GPS_UART )	Supercluster_ReceiveUartChar_GPS(&Circular_Buffer);
}//Works OK
/*************************************** End of file ************************************************/
