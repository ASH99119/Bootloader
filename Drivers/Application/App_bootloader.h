#ifndef __APP_BOOTLOADER_H
#define __APP_BOOTLOADER_H

#include "Int_w24c02.h"
#include "int_bootloader.h"


//添加校验的密钥
#define CHECK_KEY_ADDR 0x09
#define CHECK_KEY 0x5A6B
// 存储更新状态的位置
#define CHECK_UPDATE_ADDR 0x08 
//更新状态的值
#define BOOT_UPDATE 0x01
#define BOOT_NO_UPDATE 0x02





/**
 * @brief  //判断当前是否需要进行更新
 * @retval None
 */
void App_Bootloader_Check_Update(void);

/**
 * @brief  //执行更新操作
 * @retval None
 */
void App_bootloader_Update(void);


/**
 * @brief  //执行跳转操作
 * @retval None
 */
void App_bootloader_Jump_App(void);




#endif /* __APP_BOOTLOADER_H */
