/*!
 *@file DFRobot_DHT20.cpp
 *@brief Define the basic structure of class DFRobot_DHT20, the implementation of the basic methods
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-24
 *@get from https://www.dfrobot.com
 *@https://github.com/DFRobot/DFRobot_DHT20
*/

#include "DHT20/DHT20.h"


DHT20::DHT20(i2c_port_t i2c_num, uint8_t address)
{
    _address = address;
    _i2c_num = i2c_num;
}

int DHT20::begin() {
  	uint8_t readCMD[1]={0};
  	uint8_t data;
  	vTaskDelay(100/portTICK_RATE_MS);
  	// i2c_WriteCommand(_i2c_num,_address,0x20,0x2f,true);
  	i2c_Read_nbyte(_i2c_num,_address,0x71, readCMD,1);
  	//check if the IIC communication works 
  	data = readCMD[0];
  	//Serial.println(data);
  	if((data | 0x8) == 0){
  	   return 1;
  	}
  	if(data == 255) return 1;
  	return 0;
}


//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//////////////////////////////////
// float DFRobot_DHT20::getTemperature() {
//  	uint8_t readCMD[3]={0xac,0x33,0x00};
//  	uint8_t data[6] = {0};
//  	int retries = 10;
//  	float temperature;
//  	// when the returned data is wrong, request to get data again until the data is correct. 
 	
//  	i2c_master_WriteData(_i2c_num,
// 						_address<<1 | WRITE_BIT,
// 						readCMD,3,true);
 	
//  	vTaskDelay(100/portTICK_RATE_MS);

// 	i2c_Read_Data(_i2c_num,_address,data,6);

//  	uint32_t temp = data[3] & 0xff;
//  	uint32_t temp1 = data[4] & 0xff;
//  	uint32_t rawData = 0;
//  	rawData = ((temp&0xf)<<16)+(temp1<<8)+(data[5]);
//  	DBG(rawData);
//  	DBG((temp&0xf)<<16);
//  	temperature = (float)rawData/5242 -50;
//  	DBG(temperature)
//  	return temperature;
// }

// float DFRobot_DHT20::getHumidity() {
//   	uint8_t readCMD[3]={0xac,0x33,0x00};
//   	uint8_t data[6] = {0};
//   	int retries = 10;
//   	float humidity;
//   	// when the returned data is wrong, request to get data again until the data is correct. 
//   	i2c_master_WriteData(_i2c_num,
// 						_address<<1 | WRITE_BIT,
// 						readCMD,3,true);
 	
//  	vTaskDelay(100/portTICK_RATE_MS);

// 	i2c_Read_Data(_i2c_num,_address,data,6);

//   	uint32_t temp = data[1] & 0xff;
//   	uint32_t temp1 = data[2] & 0xff;
//   	uint32_t rawData = 0;
//   	rawData = (temp<<12)+(temp1<<4)+((data[3]&0xf0)>>4);
//   	DBG(rawData);
//   	DBG(temp<<12);
//   	humidity = (float)rawData/0x100000;
//   	DBG(humidity)
//   	return humidity;
// }


void DHT20::readData() {
 	uint8_t readCMD[3]={0xac,0x33,0x00};
  	uint8_t data[6] = {0};
  	int retries = 10;
	float temperature;
  	float humidity;
  	// when the returned data is wrong, request to get data again until the data is correct. 
  	i2c_master_WriteData(_i2c_num,
						_address<<1 | WRITE_BIT,
						readCMD,3,true);
 	
 	vTaskDelay(100/portTICK_RATE_MS);

	i2c_Read_Data(_i2c_num,_address,data,6);


	//Temperature
	uint32_t temp = data[3] & 0xff;
 	uint32_t temp1 = data[4] & 0xff;
 	uint32_t rawData = 0;
 	rawData = ((temp&0xf)<<16)+(temp1<<8)+(data[5]);
 	// DBG(rawData);
 	// DBG((temp&0xf)<<16);
 	temperature = (float)rawData/5242 -50;
 	printf("Temp: %4.2f\n",temperature);
	Temperature = temperature;

	//Humi
	temp = data[1] & 0xff;
  	temp1 = data[2] & 0xff;
  	rawData = 0;
  	rawData = (temp<<12)+(temp1<<4)+((data[3]&0xf0)>>4);
  	DBG(rawData);
  	DBG(temp<<12);
  	humidity = (float)rawData/0x100000*100;
  	printf("Humi: %4.2f\n",humidity);
	Humidity = humidity;
  
}