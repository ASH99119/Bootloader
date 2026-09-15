#include "App_bootloader.h"

uint8_t app_boot_update_status = BOOT_NO_UPDATE; //默认不需要更新


/**
 * @brief  //判断当前是否需要进行更新
 * @retval None
 */
void App_Bootloader_Check_Update(void)
{
    //读取3个字节的数据
    uint8_t data[3];
    Int_w24c02_read_bytes(CHECK_UPDATE_ADDR, data, 3);
    //1.检查密钥是否正确   高8位在前
    uint16_t key = (data[1] << 8) | data[2];
    if (key != CHECK_KEY)
    {
        //密钥不正确，不进行更新   重置密钥
        data[0] = BOOT_NO_UPDATE;
        data[1] = (uint8_t)(CHECK_KEY >> 8);
        data[2] = (uint8_t)(CHECK_KEY );
        Int_w24c02_write_bytes(CHECK_UPDATE_ADDR, data, 3);
    }
    else
    {
        //密钥正确 读取状态值 判断当前是否需要更新
        app_boot_update_status = data[0];

    }
}





/**
 * @brief  //执行更新操作
 * @retval None
 */
void App_bootloader_Update(void)
{
    if(app_boot_update_status == BOOT_UPDATE)
    {
        //将W25Q64中的程序写入到flash中
        printf("Starting application update...\n");
    }
    else if(app_boot_update_status == BOOT_NO_UPDATE)
    {
        //恢复出厂设置
        printf("Restoring factory settings...\n");
    }
    else if(app_boot_update_status == BOOT_RESET)
    {
        //恢复出厂设置
        printf("Restoring factory settings...\n");
    }
    else
    {
        //不需要更新
        printf("No need to update the application.\n");
    }
}


/**
 * @brief  //执行跳转操作
 * @retval None
 */
void App_bootloader_Jump_App(void)
{
    //不管更新与否 最后都需要执行跳转的操作 到A程序中
    Int_Bootloader_jump_to_app();
}
