#include "Int_w24c02.h"
#include "my_iic.h"
/**
 * @brief  读取一个字节
 * @param  byte_addr: The address to read the byte from.
 * @retval The byte of data read.
 */
uint8_t Int_w24c02_read_byte(uint8_t byte_addr)
{
    //流程介绍:启动信号 发送写从设备地址  发送字节地址 启动信号 发送读从设备地址 读取数据   NACK停止信号

    //uint8_t data = 0;
    //硬件IIC  (1)句柄编号 I2C句柄  (2)设备地址 芯片固定值  (3)字节地址  数据写入的位置 (4)数据存放地址 (5)地址长度 8位 (6)数据长度   (7)超时时间 
    //HAL_I2C_Mem_Read(&hi2c1, W24C02_ADDR_R, byte_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    //return data;

    //使用软件IIC实现
    uint8_t data = 0;
    /* 软件 IIC 替代 HAL_I2C_Mem_Read 的完整过程 */
    IIC_Start();  
    IIC_Send_Byte(W24C02_ADDR);   // 1. 发送写设备地址 (0xA0)，用于定位内部指针
    IIC_Wait_Ack(); 

    IIC_Send_Byte(byte_addr);     // 2. 发送目标字节地址
    IIC_Wait_Ack();         

    IIC_Start();                  // 3. 重新产生起始信号
    IIC_Send_Byte(W24C02_ADDR_R); // 4. 发送读设备地址 (0xA1)
    IIC_Wait_Ack();  

    data = IIC_Read_Byte(0);      // 5. 读取 1 个字节数据，并发送 NACK (0) 停止接收
    IIC_Stop(); 

    return data;
}

/**
 * @brief  写入一个字节
 * @param  byte_addr: The address to write the byte to.
 * @param  data: The byte of data to write.
 * @retval None
 */
void Int_w24c02_write_byte(uint8_t byte_addr, uint8_t data)
{
    // 流程介绍:启动信号 发送写从设备地址  发送字节地址 发送数据 停止信号
    //(1)句柄编号 I2C句柄  (2)设备地址 芯片固定值  (3)字节地址  数据写入的位置 (4)数据存放地址 (5)地址长度 8位  (6)数据长度   (7)超时时间
    //HAL_I2C_Mem_Write(&hi2c1, W24C02_ADDR, byte_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // 使用软件IIC写入一个字节
    /* 软件 IIC 替代 HAL_I2C_Mem_Write 的完整过程 */
    IIC_Start();  

    IIC_Send_Byte(W24C02_ADDR);   // 1. 发送写设备地址 (0xA0)
    IIC_Wait_Ack();

    IIC_Send_Byte(byte_addr);     // 2. 发送目标字节地址
    IIC_Wait_Ack();                                                                                                      

    IIC_Send_Byte(data);          // 3. 发送要写入的数据
    IIC_Wait_Ack();                 

    IIC_Stop();                   // 4. 产生停止信号，结束传输

    HAL_Delay(5);                 // 5. 关键补充：等待 EEPROM 内部烧写完成
}

/**
 * @brief  读取多个字节
 * @param  byte_addr: The address to read the bytes from.
 * @param  data: A pointer to the buffer where the read bytes will be stored.
 * @param  len: The number of bytes to read.
 * @retval None
 */
void Int_w24c02_read_bytes(uint8_t byte_addr, uint8_t *data, uint16_t len)
{
    // 流程介绍:启动信号 发送写从设备地址  发送字节地址 启动信号 发送读从设备地址 读取数据  ACK 读取数据 ACK.... 读取最后一个数据 NACK 停止信号
    //硬件IIC  (1)句柄编号 I2C句柄  (2)设备地址 芯片固定值  (3)字节地址  数据写入的位置 (4)数据存放地址 (5)地址长度 8位 (6)数据长度   (7)超时时间 
    //HAL_I2C_Mem_Read(&hi2c1, W24C02_ADDR_R, byte_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1000);

    //使用软件IIC实现
    /* 软件 IIC 替代 HAL_I2C_Mem_Read 的完整过程 */
    uint16_t i;

    // 使用软件IIC实现
    /* 软件 IIC 替代 HAL_I2C_Mem_Read 的完整过程 */
    
    IIC_Start();  
    IIC_Send_Byte(W24C02_ADDR);   // 1. 发送写设备地址 (0xA0)，用于设定内部地址指针
    IIC_Wait_Ack(); 

    IIC_Send_Byte(byte_addr);     // 2. 发送目标字节地址
    IIC_Wait_Ack();         

    IIC_Start();                  // 3. 重新产生起始信号 (Re-Start)
    IIC_Send_Byte(W24C02_ADDR_R); // 4. 发送读设备地址 (0xA1)，准备接收数据
    IIC_Wait_Ack();  

    // 5. 循环读取 len 个字节数据
    for(i = 0; i < len; i++)
    {
        // 关键判断：
        // 如果是最后一个字节，发送 NACK (0)，告诉 EEPROM 不要再发了
        // 如果不是最后一个字节，发送 ACK (1)，告诉 EEPROM 继续发下一个字节
        if(i == (len - 1))
        {
            data[i] = IIC_Read_Byte(0); 
        }
        else
        {
            data[i] = IIC_Read_Byte(1); 
        }
    }
    
    IIC_Stop();                   // 6. 产生停止信号，结束总线通讯
}


/**
 * @brief  写入多个字节
 * EEPROM中 一次只能写入一页 (16字节)，如果要写入多页数据，需要分批写入，每次写入一页，写完后需要等待EEPROM内部烧写完成。
 * 0x00 -> 0x10  从0x05开始写 =>也只能写到0x10
 * @param  byte_addr: The address to write the bytes to.
 * @param  data: A pointer to the buffer containing the bytes to write.
 * @param  len: The number of bytes to write.
 * @retval None
 */
void Int_w24c02_write_bytes(uint8_t byte_addr, uint8_t *data, uint16_t len)
{
    // 流程介绍:启动信号 发送写从设备地址  发送字节地址 发送数据 发送数据 ...... 停止信号
    //(1)句柄编号 I2C句柄  (2)设备地址 芯片固定值  (3)字节地址  数据写入的位置 (4)数据存放地址 (5)地址长度 8位  (6)数据长度   (7)超时时间
    //HAL_I2C_Mem_Write(&hi2c1, W24C02_ADDR, byte_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1000);


    // 使用软件IIC写入多个字节
    /* 软件 IIC 替代 HAL_I2C_Mem_Write 的完整过程 */
    //无保护，超过页大小自动从页首开始写入，导致数据回卷
    // uint16_t i;
    
    // IIC_Start();  
    // IIC_Send_Byte(W24C02_ADDR);   // 发送写设备地址 (0xA0)
    // IIC_Wait_Ack();

    // IIC_Send_Byte(byte_addr);     // 发送起始内部地址
    // IIC_Wait_Ack();                                                                                                      

    // // 【致命缺陷区域】：没有任何分页保护，直接将所有字节连续发往芯片
    // for(i = 0; i < len; i++)
    // {
    //     IIC_Send_Byte(data[i]);
    //     IIC_Wait_Ack();
    // }

    // IIC_Stop();                   // 循环结束后才发停止信号，触发一次性刻录
    // HAL_Delay(5);                 // 等待烧写




    // 使用软件IIC实现：带页对齐处理的连续写入算法，防止回卷。一页写满后向下写入
    //方法：(1)循环单字节写入 => 实现代码简单 效率低
    //(2)软件判断写入具体哪几页 1页写入一次
    // 关键点：W24C02 每页大小为 8 字节，写入时需要考虑页边界，避免数据回卷
    //(2)方法：计算当前页剩余空间，决定本次写入长度，写完后更新地址和剩余长度，循环直到写完所有数据
    uint16_t i = 0;

    while (len > 0)
    {
        // 1. 计算当前页还能写多少个字节
        // 取余数可以知道当前地址在页内的偏移，用页大小减去偏移就是本页剩余空间
        uint8_t page_remain = W24C02_PAGE_SIZE - (byte_addr % W24C02_PAGE_SIZE);
        
        // 2. 决定本次写入的长度
        // 如果剩余要写的数据长度 < 本页剩余空间，就只写所需长度；否则填满当前页
        uint8_t write_len = (len < page_remain) ? len : page_remain;

        // 3. 执行 I2C 物理写入时序 (页写模式)
        IIC_Start();  
        IIC_Send_Byte(W24C02_ADDR);   // 发送写设备地址 (0xA0)
        IIC_Wait_Ack();

        IIC_Send_Byte(byte_addr);     // 发送目标字节地址
        IIC_Wait_Ack();                                                                                                      

        // 连续发送 write_len 个数据，不产生停止信号
        for (uint8_t j = 0; j < write_len; j++)
        {
            IIC_Send_Byte(data[i++]);
            IIC_Wait_Ack();
        }

        IIC_Stop();                   // 产生停止信号，触发 EEPROM 开始内部页烧写

        HAL_Delay(5);                 // 等待 EEPROM 内部烧写完成 (按页等待，大大提升速度)

        // 4. 更新地址和剩余长度，准备写下一页 (如果有)
        byte_addr += write_len;
        len -= write_len;
    }
}
