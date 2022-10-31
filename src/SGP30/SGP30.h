/**
 * @file SGP30.h
 * @author 	彬_Linus
 * @brief 	此组件仅适用于ESP32
 *          需要My_i2c组件支持
 * @brief	This component only supports ESP32
 *          This component requires a component named My_i2c
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <stdio.h>
#include "stdlib.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "My_i2c/My_i2c.h"

// the i2c address
#define SGP30_I2CADDR_DEFAULT 0x58


#define DEV_SGP30_SCL_NUM       7			/*!< gpio number for SCL 	*** SCL的IO口*/
#define DEV_SGP30_SDA_NUM       6			/*!< gpio number for SDA 	*** SDA的IO口 */
#define DEV_SGP30_I2C_NUM       (i2c_port_t)0			/*!< I2C port number  		*** I2C编号*/



/*!
 *  @brief  Class that stores state and functions for interacting with
 *          SGP30 Gas Sensor
 */
class SGP30 {
public:
 	SGP30(i2c_port_t i2c_num = DEV_SGP30_I2C_NUM, uint8_t address = SGP30_I2CADDR_DEFAULT);
 	void begin();

	void readData();
 	

 	/** The last measurement of the IAQ-calculated Total Volatile Organic
 	 *  Compounds in ppb. This value is set when you call {@link IAQmeasure()} **/
 	uint16_t TVOC;

 	/** The last measurement of the IAQ-calculated equivalent CO2 in ppm. This
 	 *  value is set when you call {@link IAQmeasure()} **/
 	uint16_t eCO2;

 	
 	

private:


    uint8_t _address;
    i2c_port_t _i2c_num;
};
