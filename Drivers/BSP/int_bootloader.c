#include "int_bootloader.h"


//接收程序的缓冲区
uint8_t uart_rec_buff[BOOTLOADER_UART_REC_BUFF_LEN] = {0};
uint16_t uart_rec_len = 0; //接收数据的长度
uint16_t uart_rec_full_len = 0; //接收数据的总长度

//记录当前写入程序的偏移量
uint32_t flash_write_offset = 0;
//记录当前一次接收数据的时间
uint32_t last_rec_time = 0;

//末尾可能出现的单独字节
//标记最后一个字节是否是单独的
uint8_t last_byte_flag = 0;
//记录最后一个字节是谁
uint8_t last_byte = 0;

//擦除flash的函数
static void Int_flash_erase(void)
{
     uint8_t is_erase = 0;
     uint32_t page_addr = 0;
     for(uint16_t i = 0; i < uart_rec_len; i++)
    {
       //读取每个位置的值
        uint8_t data = *(volatile uint8_t*)(APP_START_ADDRESS + flash_write_offset + i);
        if(data != 0xFF)
        {
            is_erase = 1;
            //记录当前页的起始地址
            page_addr = (APP_START_ADDRESS + flash_write_offset + i) - (APP_START_ADDRESS + 
            flash_write_offset) % FLASH_PAGE_SIZE;
            break;
        }
    }
    //2.2如果需要擦除 则擦除当前页
    if(is_erase)
    {
        //擦除当前页
        FLASH_EraseInitTypeDef erase_init;
        //擦除单独页
        erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
        //擦除第1个bank的页
        erase_init.Banks = FLASH_BANK_1;
        //擦除当前页的起始地址
        erase_init.PageAddress = page_addr;
        //擦除1页
        erase_init.NbPages = 1;
        //擦除页的错误地址
        uint32_t page_error = 0;
        //flash擦除函数比较慢，擦除一页大约需要几十毫秒
        HAL_FLASHEx_Erase(&erase_init, &page_error);
    }
}
//写入flash的函数
static void Int_flash_write_with_last(void)
{
    for(uint16_t i = 0; i < uart_rec_len ; i+=2)
    {
        uint32_t flash_addr = APP_START_ADDRESS + flash_write_offset + i;
        uint16_t data16;
        if(i == 0)
        {   
            //拼接上一次的字节
            data16 = last_byte | uart_rec_buff[i] << 8;
        }
        else
        {
            data16 = uart_rec_buff[i-1] | uart_rec_buff[i] << 8;
        }
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_addr , data16);
    }
}
//写入flash的函数
static void Int_flash_write_no_last(void)
{ 
    for(uint16_t i = 0; i < uart_rec_len; i+=2)
    {   
        uint32_t flash_addr = APP_START_ADDRESS + flash_write_offset + i;
        uint16_t data16;

        if(i+1 < uart_rec_len)
        {
            data16 = uart_rec_buff[i] | uart_rec_buff[i+1] << 8;
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_addr , data16);     
        }
    }
}
//写入flash的函数
static void Int_flash_write_halfword(void)
{
    //本次之后无剩余
        if((uart_rec_len + last_byte_flag) % 2 == 0)
        {
            if(last_byte_flag)
            {
                //上次遗留有一个字节 => 这次需要作为第一个字节写入  1 5
                
                Int_flash_write_with_last();

                 //2.4 记录偏移量
                flash_write_offset += uart_rec_len + 1;
            }

            else
            {
                //正好能够写入 => 不再有遗留的字节   0 6 
                Int_flash_write_no_last();

                //2.4 记录偏移量
                flash_write_offset += uart_rec_len;
            } 
            last_byte_flag = 0;

        }

        //本次之后有剩余 
        else
        {
            if(last_byte_flag)
            {
                //上次有遗留字节 这次数量是偶数
                Int_flash_write_with_last();
                //修改最后剩下的字节
                last_byte = uart_rec_buff[uart_rec_len - 1];
                //记录偏移量
                flash_write_offset += uart_rec_len;

                
            }   
            else
            {
                //上次没有遗留字节 这次会留下一个
                Int_flash_write_no_last();
                last_byte = uart_rec_buff[uart_rec_len - 1];
                //2.4 记录偏移量
                flash_write_offset += uart_rec_len - 1;
            }
            last_byte_flag = 1;
        }
}







/**
  * @brief  串口开启中断接收之后，触发空闲帧时使用的回调函数
  * 串口协议稳定性差，发送长文件的时候，容易丢失字节
  * 修改波特率可以提升稳定性        高波特率性能更好，低波特率性能差
  * 但是波特率过高，容易出现丢失字节的情况
  * HAL串口代码繁琐， 如果再中断回调函数中调用串口输出 会非常占用资源
  * @param  huart UART handle.
  * @param  Size Amount of data received.
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        //接收到数据 记录当前的STM32系统时间
        last_rec_time = HAL_GetTick();

        //保存接收数据的长度
        uart_rec_len = Size;
        uart_rec_full_len += uart_rec_len;

        //底层调用fputc=>重定向为串口输出，中断打印太占性能
       // printf("buff:%d", uart_rec_full_len);

        //将接收的数据写入到Flash中
        //1.解锁flash
        HAL_FLASH_Unlock();
        
        //2.擦除flash,判断当前写入地址是否为新的一页 => 需要擦除
        //2.1 遍历需要写入的地址  长度为当前接收的数据长度 如果全部内容都是0xff 则说明已经全部擦除过了
        //擦除一页需要的时间为 20-30ms
        Int_flash_erase();

        //2.3写入flash,使用16位写入 => 贴合实际情况
        //flash 写入一次16位 40us   一次256字节   时间256*40us = 10.24ms
        Int_flash_write_halfword();
         
        //3.重新加锁
        HAL_FLASH_Lock();
        
         //清空缓冲区
        memset(uart_rec_buff, 0, BOOTLOADER_UART_REC_BUFF_LEN);
        
        
        //清空掉初始化串口使用之前的所有问题
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, uart_rec_buff, BOOTLOADER_UART_REC_BUFF_LEN);

    }
}




/**
 * @brief 串口接收 =>准备接收A程序
 * 
 */

void Int_Bootloader_receive_app (void)
{
    //清空掉初始化串口使用之前的所有问题
    __HAL_UART_CLEAR_OREFLAG(&huart1);
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    //带有中断的串口接收函数
    //和不带中断的串口接收函数的区别在于：不带中断的串口接收函数会阻塞程序的执行，直到接收到指定长度的数据或者超时。而带中断的串口接收函数会在接收到数据时触发中断，允许程序继续执行其他任务，同时在中断服务程序中处理接收到的数据。
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, uart_rec_buff, BOOTLOADER_UART_REC_BUFF_LEN);

}



/**
 * @brief 跳转到A程序
 * uint8_t 0:成功  1:失败
 */
uint8_t Int_Bootloader_jump_to_app(void)
{

    typedef void (*pFunc)(void);

    //1.校验
    //栈顶地址的值
    uint32_t app_stack_ptr = *(volatile uint32_t*)(APP_START_ADDRESS);
    uint32_t app_reset_handle = *(volatile uint32_t*)(APP_START_ADDRESS + 4);
    
    // ================== 添加在这里 ==================
    printf("app_stack_ptr: 0x%08X\r\n", app_stack_ptr);
    printf("app_reset_handle: 0x%08X\r\n", app_reset_handle);
    // ===============================================
    
    //1.1校验栈顶地址
    if((app_stack_ptr & 0xFFFF0000)!=STACK_ADDR)
    {
        //栈顶地址不合法
        printf("stack addr error\r\n");
        return 1;
    }

    //1.2校验复位中断地址
    if((app_reset_handle <APP_START_ADDRESS || app_reset_handle > APP_END_ADDRESS))
    {
        //复位中断地址不合法
        printf("reset handle addr error\r\n");
        return 1;
    }

    //2.注销bootloader程序
    //2.1关闭中断
    __disable_irq();
    
    //注销HAL库设置  注销掉外设的配置  不会去注销内核
    HAL_DeInit();


    //2.2设置堆栈指针
    __set_MSP(app_stack_ptr);

    //2.3重定向中断向量表
    SCB->VTOR = APP_START_ADDRESS;

    //2.4跳转到A程序的复位中断地址
    pFunc jump_tp_app = (pFunc)app_reset_handle;
    //跳转代码之后的内容是执行不到的
    jump_tp_app();
    
    return 0;
}

/**
 * @brief 外部可调用 提前擦除flash空间
 * 
 * 
 */
void Int_bootloader_erase_flash(uint32_t page_addr,uint16_t pages)
{       //1.解锁flash
        HAL_FLASH_Unlock();
        //擦除当前页
        FLASH_EraseInitTypeDef erase_init;
        //擦除单独页
        erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
        //擦除第1个bank的页
        erase_init.Banks = FLASH_BANK_1;
        //擦除当前页的起始地址
        erase_init.PageAddress = page_addr;
        //擦除1页
        erase_init.NbPages = pages;
        //擦除页的错误地址
        uint32_t page_error = 0;
        //flash擦除函数比较慢，擦除一页大约需要几十毫秒
        HAL_FLASHEx_Erase(&erase_init, &page_error);
}