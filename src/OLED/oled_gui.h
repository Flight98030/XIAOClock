#ifndef __OLED_GUI_H
#define __OLED_GUI_H


// #define FeaSetN	    	0
#define GuiTime 		1
#define GuiTempHum 		2
#define GuiWeather 		3

void StartUp(void);				//显示开机界面
void OledGui_FeaSet(void);		//设置界面
void OledGui_Temp_Hum(void);	//温湿度界面
void OledGui_Time(void);		//时间显示界面
void OledGui_Weather(void);		//天气显示界面

#endif
