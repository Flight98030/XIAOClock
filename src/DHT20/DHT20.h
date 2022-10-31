/*!
 *@file DFRobot_DHT20.cpp
 *@brief Define the basic structure of class DFRobot_DHT20, the implementation of basic methods.
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-24
 *@get from https://www.dfrobot.com
 *@https://github.com/DFRobot/DFRobot_DHT20
*/


#ifndef DFRobot_DHT20_H
#define DFRobot_DHT20_H

#include <stdio.h>
#include "stdlib.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "My_i2c/My_i2c.h"
//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#define DEV_DHT20_SCL_NUM       7			/*!< gpio number for SCL 	*** SCL的IO口*/
#define DEV_DHT20_SDA_NUM       6			/*!< gpio number for SDA 	*** SDA的IO口 */
#define DEV_DHT20_I2C_NUM       (i2c_port_t)0			/*!< I2C port number  		*** I2C编号*/

//extern Stream *dbg;
class DHT20
{
public:
  /*!
   * @brief Construct the function
   * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
   * @param address Chip IIC address, 0x38 in default.
   */
  DHT20(i2c_port_t i2c_num = DEV_DHT20_I2C_NUM, uint8_t address = 0x38);

  /**
   * @brief init function
   * @return Return 0 if initialization succeeds, otherwise return non-zero and error code.
   */
  int begin(void);
  
  void readData();
    
  /**
   * @brief Get ambient temperature, unit: °C
   * @return ambient temperature, measurement range: -40°C ~ 80°C
   */
  float Temperature;
    
  /**
   * @brief Get relative humidity, unit: %RH. 
   * @return relative humidity, measurement range: 0-100%
   */
  float Humidity;

private:

  /**
   * @brief Write command into sensor chip 
   * @param pBuf  Data included in command
   * @param size  The number of the byte of command
   */
    void  writeCommand(const void *pBuf,size_t size);
  /**
   * @brief Write command into sensor chip 
   * @param pBuf  Data included in command
   * @param size  The number of the byte of command
   * @return      Return 0 if the reading is done, otherwise return non-zero. 
   */
       
    
    uint8_t _address;
    i2c_port_t _i2c_num;
  
};

#endif
