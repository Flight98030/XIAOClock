#include "My_i2c/My_i2c.h"
#include "stdlib.h"
// #include "stdio.h"
#include "esp_log.h"


static const char *TAG = "My_i2c.c";

/**
 * @brief i2c作为主设备初始化
 * 
 * @param i2c_num        I2C port number  0/1
 * @param i2c_sda_IO     SDA的GPIO号
 * @param i2c_scl_IO     SCL的GPIO号
 * @param i2c_FREQ_HZ    i2c频率     常见100kHz
 * @return esp_err_t 
 */
esp_err_t i2c_Master_init(  int i2c_num,
                            int i2c_sda_IO,
                            int i2c_scl_IO,
                            uint32_t i2c_FREQ_HZ)
{
    int i2c_master_port = i2c_num;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = i2c_sda_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = i2c_scl_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = i2c_FREQ_HZ,
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        ESP_LOGI(TAG,"i2c_param_config ERROR!!!\r");
        return err;
    }
    ESP_LOGI(TAG,"i2c_Install OK!!!\r");
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}



/**
 * @brief i2c作为从设备初始化
 * 
 * @param i2c_num           I2C port number  0/1
 * @param i2c_sda_IO        SDA的GPIO号
 * @param i2c_scl_IO        SCL的GPIO号
 * @param ESP_SLAVE_ADDR    作为从设备的地址
 * @return esp_err_t 
 */
esp_err_t My_i2c_slave_init(int i2c_num, 
                         int i2c_sda_IO,
                         int i2c_scl_IO,
                         uint16_t ESP_SLAVE_ADDR)
{
    int i2c_slave_port = i2c_num;
    i2c_config_t conf_slave = {
        .sda_io_num = i2c_sda_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = i2c_scl_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .mode = I2C_MODE_SLAVE,
        .slave.addr_10bit_en = 0,
        .slave.slave_addr = ESP_SLAVE_ADDR,
    };
    esp_err_t err = i2c_param_config(i2c_slave_port, &conf_slave);
    if (err != ESP_OK) {
        ESP_LOGI(TAG,"i2c_param_config ERROR!!!\n");
        return err;
    }
    ESP_LOGI(TAG,"i2c_Install OK!!!\n");
    return i2c_driver_install(i2c_slave_port, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}



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
 *
 */
esp_err_t i2c_master_WriteData( i2c_port_t i2c_num,
                                uint8_t slave_addr,
                                uint8_t *data_wr,
                                size_t size,
                                bool ACK_CHECK_EN)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr, ACK_CHECK_EN);   //从设备地址
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);     //数据
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


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
                            bool ACK_CHECK_EN)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr << 1 | WRITE_BIT, ACK_CHECK_EN);   //从设备地址
    i2c_master_write_byte(cmd, REG, ACK_CHECK_EN);     //命令
    i2c_master_write_byte(cmd, Command, ACK_CHECK_EN);      //数据
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


/**
 * @brief 
 * 
 * @param i2c_num 
 * @param slave_addr 
 * @param REG_addr 
 * @return uint8_t 
 */
// uint8_t My_i2c_master_read_byte(i2c_port_t i2c_num,
//                                 uint8_t slave_addr,
//                                 uint8_t REG_addr)
// {
//     uint8_t *data_rd;
//     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, slave_addr << 1 | WRITE_BIT, ACK_VAL);   //从设备地址
//     i2c_master_write_byte(cmd, REG_addr, ACK_VAL);     //寄存器
//     i2c_master_write_byte(cmd, slave_addr << 1 | READ_BIT, ACK_VAL);
//     i2c_master_read_byte(cmd, data_rd, NACK_VAL);
//     i2c_master_stop(cmd);
//     esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
//     i2c_cmd_link_delete(cmd);
//     // uint8_t data1;
//     // data1 = =*(uint8_t *)data_rd;
    
//     return data_rd;
// }

/**
 * @brief 
 * 
 * @param i2c_num 
 * @param slave_addr 
 * @param REG_addr 
 * @param data_rd 
 * @param size 
 * @return esp_err_t 
 */
esp_err_t i2c_Read_nbyte(i2c_port_t i2c_num,
                            uint8_t slave_addr,
                            uint8_t REG_addr,
                            uint8_t *data_rd,
                            size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr << 1 | WRITE_BIT, true);   //从设备地址
    i2c_master_write_byte(cmd, REG_addr, true);     //寄存器
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);


    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr << 1 | READ_BIT, true);

    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


/**
 * @brief 读从设备的数据
 * 
 * @param i2c_num       I2C port number
 * @param slave_addr    从设备地址
 * @param data_rd       数据
 * @param size          数据长度
 * @return esp_err_t 
 */
esp_err_t i2c_Read_Data(i2c_port_t i2c_num,
                                uint8_t slave_addr,
                                uint8_t *data_rd,
                                size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr << 1 | READ_BIT, true);

    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}