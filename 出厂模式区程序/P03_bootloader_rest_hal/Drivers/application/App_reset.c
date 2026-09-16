#include "App_reset.h"
#include <stdio.h>

uint8_t app_rec_start_buff[64] = {0};
uint16_t app_rec_start_len = 0; //接收数据的长度

//记录接收程序的总长度
uint32_t app_rec_total_len = 0;

//发送完成标签
uint8_t flag = 0;

//记录当前应用层的状态
Bootloader_status boot_status = BOOTLOADER_STATUS_INIT;

//记录上次接收的时间
extern uint32_t last_rec_time;

//记录接收实际数据的长度
extern uint16_t uart_rec_full_len;




/**
 * @brief  初始化bootloader => 打印日志启动
 * 
 */
    void APP_bootloader_init(void)
    {
        printf("bootloader start\r\n");
        printf("wait for user send data\r\n");
        printf("send 'start:len' to start\r\n");
        boot_status = BOOTLOADER_STATUS_INIT;
    
    }




/**
 * @brief  等待用户传输确认
 * 如果发送的指令不对  不需要重启程序  重新发送start：len启动命令即可
 * 
 */
void APP_bootloader_run(void)
{
    //使用非中断方式接受 => 区分接受程序
    //挂起等待接收 =>一直等待接受到buff满 或者 收到idle空闲帧
    HAL_UARTEx_ReceiveToIdle(&huart1, app_rec_start_buff, 64,&app_rec_start_len,0xFFFFFFFF);
    if(app_rec_start_len > 0)
    {
        //判断数据是否包含start:len
        char *start_str = strstr((char*)app_rec_start_buff, "start:");
        if(start_str != NULL)
        {
            //保存len的值
            app_rec_total_len = atoi((char*)start_str + 6);
            if(app_rec_total_len > 0)
            {
                printf("app len: %d\r\n", app_rec_total_len);
                //修改状态到下个阶段
                boot_status = BOOTLOADER_STATUS_RUN;
            }
            else
            {
                printf("len error\r\n");
            }
        }
        else
        {
            printf("data error\r\n");
        }
    }
}


/**
 * @brief  接收数据
 * 
 */
void APP_bootloader_rec_data(void)
{
    //接受完成折后 修改状态为检查数据App_bootloader_check_data()
   
    //软件方式 从接收到一次程序数据开始算 从idle空闲帧开始算等待2s
    if(last_rec_time != 0 && HAL_GetTick() - last_rec_time > 2000)
    {
        boot_status = BOOTLOADER_STATUS_CHECK_DATA;
    }
}



/**
 * @brief  已经传输完成 检查数据
 * uint8_t 0:通过  1:错误
 */
uint8_t App_bootloader_check_data(void)
{
    if(uart_rec_full_len == app_rec_total_len)
    {
        //长度一致 没问题
        printf("app rec done\r\n");
        boot_status = BOOTLOADER_STATUS_JUMP_APP;
        return 0;
    }
    else
    {
        printf("app rec error or timeout\r\n");
    }
    return 1;
}



/**
 * @brief  跳转到应用程序
 * uint8_t 0:成功  1:失败
 */
uint8_t App_bootloader_jump_app(void)
{
    printf("jump to app\r\n");
    uint8_t ret = Int_Bootloader_jump_to_app();
    return ret;
}



/**
 * @brief  在main方法的while循环中调用 => 处理bootloader的工作
 */
void App_bootloader_work(void)
{
    switch(boot_status)
    {
        case BOOTLOADER_STATUS_INIT:
            //等待用户传输确认
            APP_bootloader_run();
            break;

        case BOOTLOADER_STATUS_RUN:
            //接收数据的准备工作
            //确认写入flash =>提前擦除flash空间   擦除10页20k
            Int_bootloader_erase_flash(APP_START_ADDRESS, 10);
            printf("flash erase done\r\n");
            printf("ready to receive app\r\n");
            boot_status = BOOTLOADER_STATUS_REC_DATA;
            flag = 0;
            Int_Bootloader_receive_app();
            break;
            
        case BOOTLOADER_STATUS_REC_DATA:
            //等待接收完成
            APP_bootloader_rec_data();
            break;


        case BOOTLOADER_STATUS_CHECK_DATA:
            //接收完成 => 检查数据
            if(App_bootloader_check_data() == 1)
            {
                printf("app rec error,system reset\r\n");
                NVIC_SystemReset();
            }
            break;

        case BOOTLOADER_STATUS_JUMP_APP:
            if(App_bootloader_jump_app() == 1)
            {
                printf("jump to app error\r\n");
                NVIC_SystemReset();
            }
            break;
    }
}
