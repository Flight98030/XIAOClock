
#include "SGP30/SGP30.h"


/*!
 *  @brief  Instantiates a new SGP30 class
 */
SGP30::SGP30(i2c_port_t i2c_num , uint8_t address ) 
{
	_address = address;
    _i2c_num = i2c_num;
}


void SGP30::begin()
{
	uint8_t readCMD[2]={0x20,0x03};
	i2c_master_WriteData(_i2c_num,
						_address<<1 | WRITE_BIT,
						readCMD,3,true);

	vTaskDelay(100/portTICK_RATE_MS);
}

void SGP30::readData()
{
	uint8_t readCMD[2]={0x20,0x08};
  	uint8_t data[6] = {0};
	uint16_t CO2Data,TVOCData;
	
	i2c_master_WriteData(_i2c_num,
						_address<<1 | WRITE_BIT,
						readCMD,2,true);
 	
 	vTaskDelay(50/portTICK_RATE_MS);

	i2c_Read_Data(_i2c_num,_address,data,6);

	CO2Data = (data[0]<<8) | data[1]; 	//CO2拼接
	TVOCData = (data[3]<<8) | data[4];	//TVOC拼接

	if((CO2Data <= 400 && TVOCData == 0) || (CO2Data >= 65023) || (CO2Data == 0 && TVOCData == 0) || (CO2Data == 0))
	{
		i2c_master_WriteData(_i2c_num,
						_address<<1 | WRITE_BIT,
						readCMD,2,true);
 	
 		vTaskDelay(50/portTICK_RATE_MS);

		i2c_Read_Data(_i2c_num,_address,data,6);
		CO2Data = (data[0]<<8) | data[1]; 	//CO2拼接
		TVOCData = (data[3]<<8) | data[4];	//TVOC拼接
		vTaskDelay(500/portTICK_RATE_MS);

	}

	else {
		eCO2 = CO2Data;
		TVOC = TVOCData;
	}
		


}