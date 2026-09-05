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

void Int_Bootloader_Init(void);



/**
 * @brief 跳转到A程序
 * 
 */
void Int_Bootloader_jump_to_app(void);



#endif // INT_BOOTLOADER_H
