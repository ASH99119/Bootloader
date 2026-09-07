#ifndef INT_BOOTLOADER_H
#define INT_BOOTLOADER_H

// Include necessary headers
#include "main.h"
#include "usart.h"
#include "string.h"
// Define constants
#define BOOTLOADER_UART_REC_BUFF_LEN 512


//程序写入的起始位置 => A区起始位置 假设B区16K  0x4000     A区为(512-16)K  0x7C000
#define APP_START_ADDRESS 0x08004000
#define APP_END_ADDRESS 0x08080000

#define STACK_ADDR 0x20000000





/**
 * @brief 串口接收 =>准备接收A程序
 * 
 */

void Int_Bootloader_receive_app (void);



/**
 * @brief 跳转到A程序
 * uint8_t 0:成功  1:失败
 */
uint8_t Int_Bootloader_jump_to_app(void);

/**
 * @brief 外部可调用 提前擦除flash空间
 * 
 * 
 */
void Int_bootloader_erase_flash(uint32_t page_addr,uint16_t pages);

#endif // INT_BOOTLOADER_H
