/****************************************************************************************************/
//
//							Name of file:
//
//		Author: Sebastian Bednarski
//		Year:	2023
//
//		Description:
//
//		Revision:
//
//		Notes:
//							All rights reserved to Supercluster
//
/****************************************************************************************************/




/*************************************** Libraries/Header files *************************************/

#include "Supercluster_Body.h"


/*************************************** Definitions ************************************************/



/*************************************** Global variables *******************************************/

uint8_t _100ms_flag;
uint8_t _500ms_flag;
uint8_t _1000ms_flag;

uint8_t _100ms_tick;
uint16_t _500ms_tick;
uint16_t _1000ms_tick;

float new_voltage;
float new_current;

int32_t wypelnienie = 0;

/*************************************** Type definitions and unions ********************************/

//none

/*************************************** Functions **************************************************/

void Supercluster_Init(void)
{
	  Supercluster_Data_Init_GPS();
	//  Supercluster_Data_Init_I_V();
	  Supercluster_PWM_Init();


}


void Supercluster_Body_Loop()
{
	  Supercluster_Loop_GPS(&GPS_Measurement, &Circular_Buffer);
	  Supercluster_Get_Data_V(&new_voltage);
	  Supercluster_Get_Data_I(&new_current);

	  Supercluster_PWM_Generate(1, wypelnienie);


	if(_100ms_flag == TRUE)
	{
		_100ms_flag = FALSE;
	}
	if(_500ms_flag == TRUE)
	{
		_500ms_flag = FALSE;
	}
	if(_1000ms_flag == TRUE)
	{
		_1000ms_flag = FALSE;
	}
}
void Supercluster_SysTick()
{
	_100ms_tick++;
	_500ms_tick++;
	_1000ms_tick++;

	if(_100ms_tick >= 100)
	{
		_100ms_flag = TRUE;
		_100ms_tick = 0;
	}
	if(_500ms_tick >= 500)
	{
		_500ms_flag = TRUE;
		_500ms_tick = 0;
	}
	if(_1000ms_tick >= 1000)
	{
		_1000ms_flag = TRUE;
		_1000ms_tick = 0;
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	Supercluster_UART_GPS_Callback(huart);
}

/*************************************** End of file ************************************************/
