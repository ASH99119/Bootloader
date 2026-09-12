#include "my_iic.h"

/* 粗略的微秒级延时函数 (针对 STM32 常见主频) 
   如果发现通讯失败，可以尝试把倍数 8 改大一点 (如 10 或 15) */
void IIC_delay_us(uint32_t us)
{
    uint32_t i;
    for(i = 0; i < us * 8; i++) 
    {
        __NOP(); 
    }
}

// 产生 IIC 起始信号
void IIC_Start(void)
{
    IIC_SDA_H();
    IIC_SCL_H();
    IIC_delay_us(4);
    IIC_SDA_L();  
    IIC_delay_us(4);
    IIC_SCL_L();  // 钳住总线，准备收发
}

// 产生 IIC 停止信号
void IIC_Stop(void)
{
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_delay_us(4);
    IIC_SCL_H();
    IIC_delay_us(4);
    IIC_SDA_H();  
    IIC_delay_us(4);
}

// 等待应答信号 (ACK)
// 返回值：1 失败，0 成功
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    IIC_SDA_H(); IIC_delay_us(1); 
    IIC_SCL_H(); IIC_delay_us(1);

    while(IIC_SDA_READ())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L(); 
    return 0;
}

// IIC 发送一个字节
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    IIC_SCL_L(); 
    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 7) 
            IIC_SDA_H();
        else 
            IIC_SDA_L();
        txd <<= 1;
        IIC_delay_us(2);
        IIC_SCL_H();
        IIC_delay_us(2);
        IIC_SCL_L();
        IIC_delay_us(2);
    }
}

// IIC 读取一个字节 (ack: 1发送ACK，0发送NACK)
uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    IIC_SDA_H(); 
    for(i = 0; i < 8; i++)
    {
        IIC_SCL_L();
        IIC_delay_us(2);
        IIC_SCL_H();
        receive <<= 1;
        if(IIC_SDA_READ()) receive++;
        IIC_delay_us(1);
    }
    
    IIC_SCL_L();
    if (!ack) IIC_SDA_H(); // NACK
    else IIC_SDA_L();      // ACK
    IIC_delay_us(2);
    IIC_SCL_H();
    IIC_delay_us(2);
    IIC_SCL_L();
    
    return receive;
}
