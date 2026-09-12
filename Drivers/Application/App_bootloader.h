#ifndef __APP_BOOTLOADER_H
#define __APP_BOOTLOADER_H
#include "Int_bootloader.h"
#include "usart.h"


typedef enum
{
    BOOTLOADER_STATUS_INIT,
    BOOTLOADER_STATUS_RUN,
    BOOTLOADER_STATUS_REC_DATA,
    BOOTLOADER_STATUS_CHECK_DATA,
    BOOTLOADER_STATUS_JUMP_APP
}Bootloader_status;



/**
 * @brief  初始化bootloader => 打印日志启动
 * 
 */
void APP_bootloader_init(void);

/**
 * @brief  等待用户传输确认
 * 
 */
void APP_bootloader_run(void);



/**
 * @brief  接收数据
 * 
 */
void APP_bootloader_rec_data(void);

/**
 * @brief  已经传输完成 检查数据
 * uint8_t 0:通过  1:数据错误
 */
uint8_t App_bootloader_check_data(void);


/**
 * @brief  跳转到应用程序
 * uint8_t 0:成功  1:失败
 */
uint8_t App_bootloader_jump_app(void);


/**
 * @brief  在main方法的while循环中调用 => 处理bootloader的工作
 */
void App_bootloader_work(void);


#endif  // ！__APP_BOOTLOADER_H
