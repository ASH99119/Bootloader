#ifndef _INT_W24C02_H
#define _INT_W24C02_H

#include "main.h"

#define W24C02_ADDR 0xA0
#define W24C02_ADDR_R (W24C02_ADDR | 0x01)

#define W24C02_ADDR_SIZE 8
#define W24C02_PAGE_SIZE 8


/**
 * @brief  读取一个字节
 * @param  byte_addr: The address to read the byte from.
 * @retval The byte of data read.
 */
uint8_t Int_w24c02_read_byte(uint8_t byte_addr);

/**
 * @brief  写入一个字节
 * @param  byte_addr: The address to write the byte to.
 * @param  data: The byte of data to write.
 * @retval None
 */
void Int_w24c02_write_byte(uint8_t byte_addr, uint8_t data);

/**
 * @brief  读取多个字节
 * @param  byte_addr: The address to read the bytes from.
 * @param  data: A pointer to the buffer where the read bytes will be stored.
 * @param  len: The number of bytes to read.
 * @retval None
 */
void Int_w24c02_read_bytes(uint8_t byte_addr, uint8_t *data, uint16_t len);


/**
 * @brief  写入多个字节
 * @param  byte_addr: The address to write the bytes to.
 * @param  data: A pointer to the buffer containing the bytes to write.
 * @param  len: The number of bytes to write.
 * @retval None
 */
void Int_w24c02_write_bytes(uint8_t byte_addr, uint8_t *data, uint16_t len);


#endif /* _INT_W24C02_H */
