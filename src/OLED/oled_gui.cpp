#include "OLED/oled_gui.h"
#include "OLED/oled.h"
#include "OLED/bmp.h"
#include "config.h"



extern bool Connected_fl;
extern bool Connected_st;

// uint16_t temperature;
// uint16_t humidity;
// u8 GuiNum = GuiTempHum;





//Start UI
void StartUp(void)
{
	OLED_Clear();
	#ifdef SHOW_CHINESE
		OLED_ShowChinese(4,5,5,16,1); 	//正(5)  
		OLED_ShowChinese(22,5,6,16,1); 	//在(6)
		OLED_ShowChinese(40,5,7,16,1); 	//开(7)
		OLED_ShowChinese(58,5,8,16,1);	//机(8)
		OLED_ShowChinese(76,5,30,16,1);	//。(30)
	#else
		OLED_ShowString(30,10, (u8 *)"Start", 24, 1);
		OLED_ShowString(42,34, (u8 *)".", 24, 1);
	#endif


	OLED_Refresh();

	vTaskDelay(500/ portTICK_RATE_MS);
	#ifdef SHOW_CHINESE
		OLED_ShowChinese(94,5,30,16,1);	//。(30)
	#else
		OLED_ShowString(54,34, (u8 *)".", 24, 1);
	#endif
	OLED_Refresh();

	vTaskDelay(500/ portTICK_RATE_MS);
	#ifdef SHOW_CHINESE
		OLED_ShowChinese(112,5,30,16,1);//。(30)
	#else
		OLED_ShowString(66,34, (u8 *)".", 24, 1);
	#endif
	OLED_Refresh();

	vTaskDelay(1000/ portTICK_RATE_MS);

	OLED_Clear();

}



//Temperature&Humidity&CO2
void OledGui_Temp_Hum(void)
{
	#ifdef SHOW_CHINESE
	OLED_ShowChinese(0,1,0,16,1); 	//温(0)
	OLED_ShowChinese(18,1,2,16,1); 	//度(2) 
	OLED_ShowString(35,1, (u8 *)":", 16, 1);
	OLED_ShowChinese(86,1,31,16,1); //℃(31)

	OLED_ShowChinese(0,18,1,16,1); 	//湿(1)
	OLED_ShowChinese(18,18,2,16,1); 	//度(2) 
	OLED_ShowString(35,18, (u8 *)":", 16, 1);
	OLED_ShowChinese(86,18,60,16,1); //%(60)
	#else
		OLED_ShowString(0,1, (u8 *)"Temp:", 16, 1);
		OLED_ShowChinese(88,1,31,16,1); //℃(31)
		OLED_ShowString(0,18,(u8 *)"Humi:", 16, 1);
		OLED_ShowChinese(88,18,60,16,1); //%(60)
	#endif

	

	OLED_ShowNum(44,1,(int)DHT20_1.Temperature,2,16,1);	//整数
	OLED_ShowString(61,1, (u8 *)".", 16, 1);
	OLED_ShowNum(66,1,((DHT20_1.Temperature-(int)DHT20_1.Temperature)*100),2,16,1);//小数 

	OLED_ShowNum(44,18,(int)DHT20_1.Humidity,2,16,1);	//整数
	OLED_ShowString(61,18, (u8 *)".", 16, 1);
	OLED_ShowNum(66,18,((DHT20_1.Humidity-(int)DHT20_1.Humidity)*100),2,16,1);//小数


	OLED_ShowString(0,34, (u8 *)"eCO2:", 16, 1);
	OLED_ShowNum(44,34,(int)SGP30_1.eCO2,3,16,1);	//整数
	
	#ifdef GET_O2_FUNCTION
	OLED_ShowString(0,49, (u8 *)"FiO2:", 16, 1);
	OLED_ShowNum(44,49,(int)MIX8410_1.O2results,3,16,1);	//整数
	OLED_ShowString(61,49, (u8 *)".", 16, 1);
	OLED_ShowNum(66,49,((MIX8410_1.O2results-(int)MIX8410_1.O2results)*100),2,16,1);//小数
	OLED_ShowChinese(88,49,60,16,1); //%(60)
	#endif
}

//Clock
void OledGui_Time(void)
{	

	#ifdef SHOW_CHINESE
		OLED_ShowNum(4,2,pcf.hour,2,24,1);		//小时
		OLED_ShowString(34,2, (u8 *)":", 24, 1);
		OLED_ShowNum(54,2,pcf.minute,2,24,1);	//分钟
		OLED_ShowString(84,2, (u8 *)":", 24, 1);
		OLED_ShowNum(102,2,pcf.sec,2,24,1);		//秒

		OLED_ShowString(0,35, (u8 *)"    /  /", 16, 1);
		OLED_ShowNum(0,35,(2000+pcf.year),4,16,1);	//年
		OLED_ShowNum(40,35,pcf.month,2,16,1);		//月
		OLED_ShowNum(64,35,pcf.day,2,16,1);			//日

		OLED_ShowChinese(80,35,24,16,1);	//星24
		OLED_ShowChinese(96,35,25,16,1);	//期25
		/* 星期 */
		switch(pcf.weekday){
			case 0:
				OLED_ShowChinese(112,35,23,16,1);	//日23
				break;
			case 1:
				OLED_ShowChinese(112,35,17,16,1);	//一17
				break;
			case 2:
				OLED_ShowChinese(112,35,18,16,1);	//二18
				break;
			case 3:
				OLED_ShowChinese(112,35,19,16,1);	//三19
				break;
			case 4:
				OLED_ShowChinese(112,35,20,16,1);	//四20
				break;
			case 5:
				OLED_ShowChinese(112,35,21,16,1);	//五21
				break;
			case 6:
				OLED_ShowChinese(112,35,22,16,1);	//六22
				break;

		}


	#else
		OLED_ShowNum(4,0,pcf.hour,2,24,1);		//小时
		OLED_ShowString(34,0, (u8 *)":", 24, 1);
		OLED_ShowNum(54,0,pcf.minute,2,24,1);	//分钟
		OLED_ShowString(84,0, (u8 *)":", 24, 1);
		OLED_ShowNum(102,0,pcf.sec,2,24,1);		//秒

		OLED_ShowString(25,30, (u8 *)"    /  /", 16, 1);
		OLED_ShowNum(25,30,(2000+pcf.year),4,16,1);	//年
		OLED_ShowNum(65,30,pcf.month,2,16,1);		//月
		OLED_ShowNum(89,30,pcf.day,2,16,1);			//日

		
		/* 星期 */
		switch(pcf.weekday){
			case 0:
				// OLED_ShowChinese(112,35,23,16,1);	//日23
				OLED_ShowString(40,47, (u8 *)"Sunday", 16, 1);
				break;
			case 1:
				// OLED_ShowChinese(112,35,17,16,1);	//一17
				OLED_ShowString(40,47, (u8 *)"Monday", 16, 1);
				break;
			case 2:
				// OLED_ShowChinese(112,35,18,16,1);	//二18
				OLED_ShowString(38,47, (u8 *)"Tuesday", 16, 1);
				break;
			case 3:
				// OLED_ShowChinese(112,35,19,16,1);	//三19
				OLED_ShowString(28,47, (u8 *)"Wednesday", 16, 1);
				break;
			case 4:
				// OLED_ShowChinese(112,35,20,16,1);	//四20
				OLED_ShowString(34,47, (u8 *)"Thursday", 16, 1);
				break;
			case 5:
				// OLED_ShowChinese(112,35,21,16,1);	//五21
				OLED_ShowString(41,47, (u8 *)"Friday", 16, 1);
				break;
			case 6:
				// OLED_ShowChinese(112,35,22,16,1);	//六22
				OLED_ShowString(35,47, (u8 *)"Saturday", 16, 1);
				break;

		}
	#endif
	
}


extern WetherData Weather;
//cloudy,thunder,sunny,snows,rain
//天气
void OledGui_Weather(void)
{

	#ifdef SHOW_CHINESE
		if(!Connected_fl){
			OLED_ShowChinese(4,4,32,16,1);	//未32
			OLED_ShowChinese(22,4,33,16,1);	//连33
			OLED_ShowChinese(40,4,34,16,1);	//网34
		}
		else if(Connected_st != Connected_fl){
			OLED_Clear();
			Connected_st = Connected_fl;
		}

		if(Connected_fl){
			if((Weather.code == 0) || (Weather.code == 1)){
				// OLED_ShowPicture(0,0,60,60,Sunny,1);
				OLED_ShowChinese(0,22,35,16,1);	//晴35
				OLED_ShowChinese(18,22,37,16,1);	//天37
			}
			else if((Weather.code >= 4) && (Weather.code <= 8)){
				// OLED_ShowPicture(0,0,60,60,Cloudy,1);
				OLED_ShowChinese(0,22,38,16,1);	//多38
				OLED_ShowChinese(18,22,39,16,1);	//云39
			}
			else if(Weather.code == 9){
				// OLED_ShowPicture(0,0,60,60,Overcast,1);
				OLED_ShowChinese(0,22,36,16,1);	//阴36
				OLED_ShowChinese(18,22,37,16,1);	//天37
			}
			else if(Weather.code == 13){
				// OLED_ShowPicture(0,0,60,60,Light_Rain,1);
				OLED_ShowChinese(0,22,40,16,1);	//小40
				OLED_ShowChinese(18,22,43,16,1);	//雨43
			}
			else if(Weather.code == 14){
				// OLED_ShowPicture(0,0,60,60,Moderate_Rain,1);
				OLED_ShowChinese(0,22,41,16,1);	//中41
				OLED_ShowChinese(18,22,43,16,1);	//雨43
			}
			else if(Weather.code == 15){
				// OLED_ShowPicture(0,0,60,60,Heavy_Rain,1);
				OLED_ShowChinese(0,22,42,16,1);	//大42
				OLED_ShowChinese(18,22,43,16,1);	//雨43
			}
			else if((Weather.code >= 16) && (Weather.code <= 18)){
				// OLED_ShowPicture(0,0,60,60,Heavy_Rain,1);
				OLED_ShowChinese(0,22,44,16,1);	//暴44
				OLED_ShowChinese(18,22,43,16,1);	//雨33
			}

			/*显示位置*/
			OLED_ShowChinese(0,0,53,16,1); 	//位
			OLED_ShowChinese(18,0,54,16,1);	//置
			OLED_ShowString(36,0, (u8 *)":", 16, 1);

			if(Weather.City == "Shenzhen")
			{
				OLED_ShowChinese(54,0,47,16,1); 	//深
				OLED_ShowChinese(72,0,48,16,1);		//圳
				// OLED_ShowString(60,0, (u8 *)Weather.City1, 16, 1);
			}
			else if (Weather.City == "Guangzhou")
			{
				OLED_ShowChinese(54,0,49,16,1); 	//广
				OLED_ShowChinese(72,0,50,16,1);		//州
			}
			else if (Weather.City == "Dongguan")
			{
				OLED_ShowChinese(54,0,51,16,1); 	//东
				OLED_ShowChinese(72,0,52,16,1);		//莞
			}
			else if (Weather.City == "Shanghai")
			{
				OLED_ShowChinese(54,0,55,16,1); 	//上
				OLED_ShowChinese(72,0,56,16,1);		//海
			}
			else if (Weather.City == "Hangzhou")
			{
				OLED_ShowChinese(54,0,57,16,1); 	//杭
				OLED_ShowChinese(72,0,50,16,1);		//州
			}
			else if (Weather.City == "Beijing")
			{
				OLED_ShowChinese(54,0,58,16,1); 	//北
				OLED_ShowChinese(72,0,59,16,1);		//京
			}


			else OLED_ShowString(54,0, (u8 *)Weather.City1, 16, 1);

			OLED_ShowChinese(0,44,45,16,1);		//室45
			OLED_ShowChinese(18,44,46,16,1);	//外46
			OLED_ShowChinese(36,44,0,16,1);		//温0
			OLED_ShowChinese(54,44,2,16,1);		//度2
			OLED_ShowString(72,44, (u8 *)":", 16, 1);
			OLED_ShowNum(80,44,(u32)Weather.temp,2,16,1);	//心知天气温度
			OLED_ShowChinese(100,44,31,16,1); 	//℃(31)
		}

	#else
		if(!Connected_fl){
			OLED_ShowString(0,2, (u8 *)"Not connected", 24, 1);
			OLED_ShowString(0,26,(u8 *)"to the network",24, 1);
		}
		else if(Connected_st != Connected_fl){
			OLED_Clear();
			Connected_st = Connected_fl;
		}

		if(Connected_fl){

			/*显示位置*/
			// OLED_ShowString(0,0, (u8 *)"L:", 16, 1);
			OLED_ShowString(0,0, (u8 *)Weather.City1, 16, 1);


			if((Weather.code == 0) || (Weather.code == 1)){
				// OLED_ShowPicture(0,0,60,60,Sunny,1);
				OLED_ShowString(0,16,(u8 *)"Sunny",24,1);
			}
			else if((Weather.code >= 4) && (Weather.code <= 8)){
				// OLED_ShowPicture(0,0,60,60,Cloudy,1);
				OLED_ShowString(0,16,(u8 *)"Cloudy",24,1);
			}
			else if(Weather.code == 9){
				// OLED_ShowPicture(0,0,60,60,Overcast,1);
				OLED_ShowString(0,16,(u8 *)"Overcast",24,1);
			}
			else if(Weather.code == 13){
				// OLED_ShowPicture(0,0,60,60,Light_Rain,1);
				OLED_ShowString(0,16,(u8 *)"LightRain",24,1);
			}
			else if(Weather.code == 14){
				// OLED_ShowPicture(0,0,60,60,Moderate_Rain,1);
				OLED_ShowString(0,16,(u8 *)"Moderate",24,1);
			}
			else if(Weather.code == 15){
				// OLED_ShowPicture(0,0,60,60,Heavy_Rain,1);
				OLED_ShowString(0,16,(u8 *)"HeavyRain",24,1);
			}
			else if((Weather.code >= 16) && (Weather.code <= 18)){
				// OLED_ShowPicture(0,0,60,60,Heavy_Rain,1);
				OLED_ShowString(0,16,(u8 *)"HeavyRain",24,1);
			}


			OLED_ShowString(0,48, (u8 *)"OAT:", 16, 1);
			OLED_ShowNum(38,48,(u32)Weather.temp,2,16,1);	//心知天气温度
			OLED_ShowChinese(62,48,31,16,1); 	//℃(31)
		}

	#endif
		
}


/*
void OLED_Task(void *arg)
{
	OLED_Init();
	OLED_ColorTurn(0);  //0正常显示，1 反色显示
    OLED_DisplayTurn(1);//0正常显示 1 屏幕翻转显示
    StartUp();
    OLED_Clear();
    uint8_t GuiNum_f = GuiTime;
    // uint8_t t = ' ';
	while(1)
	{
		// OLED_ShowPicture(0,0,128,64,BMP1,1);
		// OLED_Refresh();
		// vTaskDelay(500/ portTICK_RATE_MS);
		// OLED_Clear();
		// OLED_ShowString(8,16,(u8 *)"ZHONGJINGYUAN",16,1);
		// OLED_ShowString(20,32,(u8 *)"2014/05/01",16,1);
		// OLED_ShowString(0,48,(u8 *)"ASCII:",16,1);  
		// OLED_ShowString(63,48,(u8 *)"CODE:",16,1);
		// OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	   
		// t++;
		// if(t>'~')t=' ';
		// OLED_ShowNum(103,48,t,3,16,1);
		// OLED_Refresh();
		// vTaskDelay(500/ portTICK_RATE_MS);
		// OLED_Clear();
		// OLED_ShowChinese(0,0,0,16,1);  //16*16 中
	    // OLED_ShowChinese(16,0,0,24,1); //24*24 中
		// OLED_ShowChinese(24,20,0,32,1);//32*32 中
	    // OLED_ShowChinese(64,0,0,64,1); //64*64 中
		// OLED_Refresh();
	    // vTaskDelay(500/ portTICK_RATE_MS);
  		// OLED_Clear();
		// OLED_ShowString(0,0,(u8 *)"ABC",8,1);//6*8 “ABC”
		// OLED_ShowString(0,8,(u8 *)"ABC",12,1);//6*12 “ABC”
	    // OLED_ShowString(0,20,(u8 *)"ABC",16,1);//8*16 “ABC”
		// OLED_ShowString(0,36,(u8 *)"ABC",24,1);//12*24 “ABC”
	  	// OLED_Refresh();
		// vTaskDelay(500/ portTICK_RATE_MS);
		// OLED_ScrollDisplay(11,4,1);
        if(GuiNum != GuiNum_f) {
            OLED_Clear();
            GuiNum_f = GuiNum;
        }
        switch (GuiNum)
		{
			case GuiTempHum:
				OledGui_Temp_Hum();		//温湿度界面
				break;
			case GuiTime:
				OledGui_Time();			//时钟界面
				break;
			case GuiWeather:
				OledGui_Weather();		//天气界面
				break;
		}
        OLED_Refresh();
	    vTaskDelay(100/ portTICK_RATE_MS);

	}
}
*/
