#pragma once

#ifndef MIX8410_H
#define MIX8410_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



/*!
 *  @brief  Class that stores state and functions for interacting with
 *          MIX8410 Gas Sensor
 */
class MIX8410 {
public:
 	MIX8410(float Refer_Volatge = 3.3, uint8_t PinNumber = A0);

 	void readData();
 	
	float O2results;

private:
	float readO2Vout();
	float _Vref;
	uint8_t _PinADC;

    
};
#endif
