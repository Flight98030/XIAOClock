#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"	
#include "driver/gpio.h"

typedef uint8_t     u8 ;
typedef uint16_t    u16;
typedef uint32_t    u32;

//-----------------OLED�˿ڶ���---------------- 
#define OLED_SCL        7
#define OLED_SDA        6
#define IIC_CTRL_NUM    I2C_NUM_0              /*!< I2C port number */

#define ACK_CHECK_ENABLE    0x1                    /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DISABLE   0x0                   /*!< I2C master will not check ack from slave */

#define OLED_CMD        0	//д����
#define OLED_DATA       1	//д����

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

// void OLED_Task(void *arg);



#ifdef __cplusplus
}
#endif


