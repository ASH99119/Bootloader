#include "Int_w25q64.h"




/**
 * @brief 拉低片选
 */
void Int_w25q64_start(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 拉高片选
 */
void Int_w25q64_stop(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_SET);
}
/**
 * @brief 写入一个字节
 * @param data 
 */
void Int_w25q64_write_byte(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, 100);

}
/**
 * @brief 读取一个字节
 * @return uint8_t 
 */
uint8_t Int_w25q64_read_byte(void)
{
    uint8_t data;
    HAL_SPI_Receive(&hspi1, &data, 1, 100);
    return data;
}


/**
 * @brief 读取芯片ID
 * @param mf_id 
 * @param device_id 
 * @param unique_id 
 */
void Int_w25q64_read_id(uint8_t *mf_id,uint16_t *device_id)
{
    //1. 拉低片选
    Int_w25q64_start();
    //2. 发送读取ID命令
    Int_w25q64_write_byte(W25Q64_READ_ID);
    //3. 读取制造商ID（mf_id）
    *mf_id = Int_w25q64_read_byte();
    uint8_t high = Int_w25q64_read_byte();
    uint8_t low = Int_w25q64_read_byte();
    *device_id = high << 8 | low;
    //4. 拉高片选
    Int_w25q64_stop();  
}   


//静态方法 等待芯片忙状态
static void Int_w25q64_wait_busy(void)
{
    //1. 拉低片选
    Int_w25q64_start();
    //2.读取状态寄存器
    while(1)
    {
        Int_w25q64_write_byte(W25Q64_READ_STATUS_REG); //读取状态寄存器命令
        uint8_t status = Int_w25q64_read_byte();
        //找到busy是最低位的值 为0表示不忙 为1表示忙
        if((status & 0x01) == 0)
        {
            break; //芯片不忙了
        }
    }

    //3. 拉高片选
    Int_w25q64_stop();
}

/**
 * @brief 读取数据
 * @param addr  一个22位 0x000000~0x3FF  FFF的地址   一次擦除4kb 一次写入时256字节
 * @param data 
 * @param len 
 */

void Int_w25q64_read_data(uint8_t block,uint32_t sector,uint8_t page,uint8_t addr,uint8_t *data, uint16_t len)
{
    //1.等待芯片不忙
    Int_w25q64_wait_busy();
    
    //2. 拉低片选
    Int_w25q64_start();

    //3. 发送读取数据命令
    Int_w25q64_write_byte(W25Q64_READ_DATA); //读取数据命令
    uint32_t addr_24 = block << 16 | sector <<12 | page << 8 | addr;
    Int_w25q64_write_byte(addr_24 >> 16) ;
    Int_w25q64_write_byte(addr_24 >> 8) ;
    Int_w25q64_write_byte(addr_24) ;
    for(int i = 0; i < len; i++)
    {
        data[i] = Int_w25q64_read_byte();
    }
    //4. 拉高片选
    Int_w25q64_stop();
}


static void Int_w25q64_write_enable(void)
{
    //1.等待芯片不忙
    Int_w25q64_wait_busy();
    //2. 拉低片选
    Int_w25q64_start();
    //3. 发送写使能命令
    Int_w25q64_write_byte(W25Q64_WRITE_ENABLE);
    //4. 拉高片选   
    Int_w25q64_stop();
}



/**
 * @brief 写入数据
 * @param addr  一个22位 0x000000~0x3FF  FFF的地址   一次擦除4kb 一次写入时256字节
 * 假设地址不超出1页的范围
 * 
 */
void Int_w25q64_write_data(uint8_t block,uint32_t sector,uint8_t page,uint8_t addr,uint8_t *data, uint16_t len)
{
    //1.写使能
    Int_w25q64_write_enable();

    //2. 拉低片选
    Int_w25q64_start();
    uint32_t addr_24 = block << 16 | sector <<12 | page << 8 | addr;
    Int_w25q64_write_byte(W25Q64_WRITE_DATA) ; //写入数据命令
    Int_w25q64_write_byte(addr_24 >> 16) ;
    Int_w25q64_write_byte(addr_24 >> 8) ;
    Int_w25q64_write_byte(addr_24) ;
    //3.写入数据
    for(int i = 0; i < len; i++)
    {
        Int_w25q64_write_byte(data[i]);
    }
    //4. 拉高片选
    Int_w25q64_stop();

    //5.等待芯片不忙
    Int_w25q64_wait_busy();
}


/**
 * @brief 擦除1扇区
 * 
 * @param block  块号 0~15
 * @param sector 扇区号 0~15
 */
void Int_w25q64_erase_sector(uint8_t block ,uint32_t sector)
{
    //1.写使能
    Int_w25q64_write_enable();
    //2. 拉低片选
    Int_w25q64_start();
    uint32_t addr =(uint32_t)block * 65536 + (uint32_t)sector * 4096;
    //3. 发送擦除扇区命令
    Int_w25q64_write_byte(W25Q64_EARSE_SECTOR);
    //4. 发送地址
    Int_w25q64_write_byte((addr >> 16) & 0xff) ;
    Int_w25q64_write_byte((addr >> 8) & 0xff) ;
    Int_w25q64_write_byte(addr & 0xff) ;
    //5.拉高片选
    Int_w25q64_stop();
}