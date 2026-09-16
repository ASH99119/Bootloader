#ifndef __INT_W25Q64_H
#define __INT_W25Q64_H

#include "spi.h"

/*
 * @brief SPI设备在使用的时候片选引脚才需要拉低 平时都需要拉高的
 */
#define W25Q64_READ_ID 0x9F
#define W25Q64_READ_STATUS_REG 0x05
#define W25Q64_READ_DATA 0x03
#define W25Q64_WRITE_DATA 0x02
#define W25Q64_EARSE_SECTOR 0x20
#define W25Q64_WRITE_ENABLE 0x06


/**
 * @brief 拉低片选
 */
void Int_w25q64_start(void);

/**
 * @brief 拉高片选
 */
void Int_w25q64_stop(void);

/**
 * @brief 写入一个字节
 * @param data 
 */
void Int_w25q64_write_byte(uint8_t data);

/**
 * @brief 读取一个字节
 * @return uint8_t 
 */
uint8_t Int_w25q64_read_byte(void);

/**
 * @brief 读取芯片ID
 * @param mf_id 
 * @param device_id 
 * @param unique_id 
 */
void Int_w25q64_read_id(uint8_t *mf_id,uint16_t *device_id);

/**
 * @brief 读取数据
 * @param addr  一个22位 0x000000~0x3FF  FFF的地址   一次擦除4kb 一次写入时256字节
 * @param data 
 * @param len 
 */
//void Int_w25q64_read_data(uint32_t addr,uint8_t *data,uint16_t len);
void Int_w25q64_read_data(uint8_t block,uint32_t sector,uint8_t page,uint8_t addr,uint8_t *data, uint16_t len);



/**
 * @brief 写入数据
 * @param addr  一个22位 0x000000~0x3FF  FFF的地址   一次擦除4kb 一次写入时256字节
 * @param data 
 * @param len 
 */
void Int_w25q64_write_data(uint8_t block,uint32_t sector,uint8_t page,uint8_t addr,uint8_t *data, uint16_t len);


/**
 * @brief 擦除1扇区
 * 
 * @param block  块号 0~15
 * @param sector 扇区号 0~15
 */
void   Int_w25q64_erase_sector(uint8_t block ,uint32_t sector);

#endif /* __INT_W25Q64_H */
