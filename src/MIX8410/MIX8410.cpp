
#include "MIX8410/MIX8410.h"


MIX8410::MIX8410(float Refer_Volatge, uint8_t PinNumber)
{
	_Vref = Refer_Volatge;
	_PinADC = PinNumber;
}


float MIX8410::readO2Vout()
{
    long sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(_PinADC);
    }
 
    sum >>= 5;
 
    float MeasuredVout = sum * (_Vref / 1023.0);
    return MeasuredVout;
}
 
void MIX8410::readData()
{
    // Vout samples are with reference to 3.3V
    float MeasuredVout = readO2Vout();
 
    //float Concentration = FmultiMap(MeasuredVout, VoutArray,O2ConArray, 6);
    //when its output voltage is 2.0V,
    float Concentration = MeasuredVout * 0.21 / 2.0;
    O2results = Concentration*100;

}