/*
 * @Author: 彬_Linus
 * @Date: 2021-09-27 10:21:48
 * @LastEditTime: 2020-09-27 10:42:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \esp-idf
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "stdlib.h"
// #include "stdio.h"
#include "esp_log.h"
#include "driver/i2c.h"


#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define I2C_MASTER_FREQ_HZ              (100000)              /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE       0                     /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE       0                     /*!< I2C master doesn't need buffer */

#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */

#define ACK_VAL     0x0     /*!< I2C ack value */
#define NACK_VAL    0x1     /*!< I2C nack value */

#define WRITE_BIT   0   /*!< I2C master write */
#define READ_BIT    1   /*!< I2C master read */


/**
 * @brief i2c作为主设备初始化
 * 
 * @param i2c_num        I2C port number  0/1
 * @param i2c_sda_IO     SDA的GPIO号
 * @param i2c_scl_IO     SCL的GPIO号
 * @param i2c_FREQ_HZ    i2c频率     常见100kHz
 * @return esp_err_t 
 */
esp_err_t i2c_Master_init(int i2c_num, int i2c_sda_IO,
                          int i2c_scl_IO, uint32_t i2c_FREQ_HZ);

/**
 * @brief i2c作为从设备初始化
 * 
 * @param i2c_num           I2C port number  0/1
 * @param i2c_sda_IO        SDA的GPIO号
 * @param i2c_scl_IO        SCL的GPIO号
 * @param ESP_SLAVE_ADDR    作为从设备的地址
 * @return esp_err_t 
 */
esp_err_t My_i2c_slave_init(int i2c_num, int i2c_sda_IO,
                         int i2c_scl_IO, uint16_t ESP_SLAVE_ADDR);

/**
 * @brief 写n位数据
 * 
 * @param i2c_num       I2C port number
 * @param slave_addr    从设备地址
 * @param data_wr       数据
 * @param size          数据长度
 * @param ACK_CHECK_EN  是否有应答
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 */
esp_err_t i2c_master_WriteData(i2c_port_t i2c_num, uint8_t slave_addr,
                                 uint8_t *data_wr, size_t size,
                                 bool ACK_CHECK_EN);


/**
 * @brief 写命令+数据
 * 
 * @param i2c_num       I2C port number
 * @param slave_addr    从设备地址
 * @param cmd_data      命令
 * @param data_wr       写入的数据
 * @param ACK_CHECK_EN  是否应答
 * @return esp_err_t 
 *  _____________________________________________________________________________
 * | start | slave_addr + ack | write command + ack  | write data + ack  | stop |
 * --------|------------------|----------------------|-------------------|------|
 *
 */
esp_err_t i2c_WriteCommand( i2c_port_t i2c_num,
                            uint8_t slave_addr,
                            uint8_t REG,
                            uint8_t Command,
                            bool ACK_CHECK_EN);

/**
 * @brief 读从设备的数据
 * 
 * @param i2c_num       I2C port number
 * @param slave_addr    从设备地址
 * @param data_rd       数据
 * @param size          数据长度
 * @param ACK_CHECK_EN  是否应答
 * @return esp_err_t 
 */
esp_err_t i2c_Read_Data(i2c_port_t i2c_num,
                        uint8_t slave_addr,
                        uint8_t *data_rd,
                        size_t size);


// uint8_t My_i2c_master_read_byte(i2c_port_t i2c_num,
//                                 uint8_t slave_addr,
//                                 uint8_t REG_addr);

esp_err_t i2c_Read_nbyte(i2c_port_t i2c_num,
                            uint8_t slave_addr,
                            uint8_t REG_addr,
                            uint8_t *data_rd,
                            size_t size);




#ifdef __cplusplus
}
#endif
