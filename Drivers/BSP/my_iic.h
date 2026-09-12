#ifndef __MY_IIC_H
#define __MY_IIC_H

#include "main.h"

#define IIC_SCL_H()   HAL_GPIO_WritePin(GPIOC, IIC_SCL_Pin, GPIO_PIN_SET)
#define IIC_SCL_L()   HAL_GPIO_WritePin(GPIOC, IIC_SCL_Pin, GPIO_PIN_RESET)

#define IIC_SDA_H()   HAL_GPIO_WritePin(GPIOC, IIC_SDL_Pin, GPIO_PIN_SET)
#define IIC_SDA_L()   HAL_GPIO_WritePin(GPIOC, IIC_SDL_Pin, GPIO_PIN_RESET)

/* 读取 SDA 引脚电平 (由于是开漏输出，直接读即可) */
#define IIC_SDA_READ() HAL_GPIO_ReadPin(GPIOC, IIC_SDL_Pin)

/* 对外提供的底层函数接口 */
void IIC_delay_us(uint32_t us);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(uint8_t ack);


#endif /* __MY_IIC_H */
