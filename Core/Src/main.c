/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
//#include "int_bootloader.h"
//#include "App_bootloader.h"
#include "Int_w24c02.h"
#include "my_iic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
extern uint16_t uart_rec_full_len; //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ýµï¿½ï¿½Ü³ï¿½ï¿½ï¿½
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//#define REC_BUFF_LEN 512
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // uint8_t receive_buff[16] = {0};
  // uint16 receive_len = 0;
  //Int_Bootloader_Init();


  //ï¿½ï¿½Ê¼ï¿½ï¿½bootloader => ï¿½ï¿½Ó¡ï¿½ï¿½Ö¾ï¿½ï¿½ï¿½ï¿½
  //APP_bootloader_init();

  //²âÊÔEEPROM¶ÁÐ´
  //1.Ð´ÈëÊý¾ÝÖ®ºó ÐèÒªµÈ´ý5msÒÔÉÏ ²ÅÄÜ¶ÁÈ¡ ·ñÔò¶Á²»µ½Êý¾Ý,ÔÚÄÚ²¿ÒÑ¾­Ð´ÁËHAL_Delay(5)ËùÒÔÕâÀïÎÒ²»Ð´ÁË
  // Int_w24c02_write_byte(0x00, 'c');
  // //HAL_Delay(5);
  // uint8_t byte = Int_w24c02_read_byte(0x00);
  // printf("Read byte: %c\r\n", byte);

  //2.Ð´ÈëµÄÊý¾Ý³¬¹ýÒ»Ò³ »á´ÓÕâÒ»Ò³µÄ¿ªÍ·ÔÙ´ÎÐ´Èë£¬ÓÉÓÚÎÒÔÚInt_w24c02_write_bytesº¯ÊýÖÐÒÑ¾­Ð´ÁËÒ»¶Î¼ÆËãµØÖ·Æ«ÒÆµÄ×Ô¶¯ÇÐ·ÖÂß¼­ËùÒÔ²»»á³öÏÖ78910456ÕâÑùµÄ»Ø¾íÁË

  Int_w24c02_write_bytes(0x00, "123456789012345678910", 21);
  HAL_Delay(5); 
  uint8_t buff[21] = {0};
  Int_w24c02_read_bytes(0x00, buff, 21);

  printf("Read bytes: %s\r\n", buff);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  
  while (1)
  {
 //ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶Ï½ï¿½ï¿½ï¿½ => ï¿½ï¿½ï¿½ï¿½Ö¡ï¿½Ð¶ï¿½ => idleï¿½ï¿½ï¿½ï¿½Ö¡Ä£Ê½ =>ï¿½Õµï¿½ï¿½ï¿½ï¿½Ýºï¿½Ð´Ò»ï¿½ï¿½ï¿½ï¿½Ö·Öµï¿½æ´¢ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿½
  //1ï¿½ï¿½ï¿½ï¿½ï¿½Õ»ï¿½ï¿½ï¿½  2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿½ 3ï¿½ï¿½ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½Ý»ï¿½ï¿½ï¿½  4:Êµï¿½Ê½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿½  5ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½
  //ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½Ì«Ð¡ï¿½ï¿½ï¿½×³ï¿½ï¿½Ö¿ï¿½ï¿½ï¿½ï¿½Ð¶Ï¶ï¿½Ê§  => ï¿½ï¿½Òªï¿½ï¿½Ö¤Êµï¿½Ê½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã»ï¿½Ð²ï¿½ï¿½ï¿½
  //Òµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?256ï¿½Ö½ï¿½
    // HAL_UARTEx_ReceiveToIdle(&huart1, receive_buff, REC_BUFF_LEN ,&receive_len, 0xffff);
    // if(receive_len > 0)
    // {
    //1ï¿½ï¿½ï¿½ï¿½ï¿½Õ»ï¿½ï¿½ï¿½  2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿½ 3ï¿½ï¿½ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½Ý»ï¿½ï¿½ï¿½ 4ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½

    //   HAL_UART_Transmit(&huart1, receive_buff, receive_len, 0xffff);
    //   memset(receive_buff, 0, 16);
    //   receive_len = 0;
    // }



   //1ï¿½ï¿½ï¿½ï¿½ï¿½Õ»ï¿½ï¿½ï¿½  2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿½ 3ï¿½ï¿½ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½Ý»ï¿½ï¿½ï¿½ 4ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½
    //ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½ï¿½Ð§ =>1. ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½È²ï¿½Îª0   2.ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½Ý»ï¿½ï¿½æ²»Îªï¿½ï¿½

    // HAL_UART_Receive(&huart1, receive_buff, 16, 0xffff);
    // if(strlen((char*)receive_buff) > 0)
    // {
    //1ï¿½ï¿½ï¿½ï¿½ï¿½Õ»ï¿½ï¿½ï¿½  2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿½ 3ï¿½ï¿½ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½Ý»ï¿½ï¿½ï¿½  4ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½
    //   HAL_UART_Transmit(&huart1, receive_buff, 16, 0xffff);
    //   memset(receive_buff, 0, 16);
    // }


    // printf("uart_rec_full_len:%d\r\n", uart_rec_full_len);
    // HAL_Delay(3000);
    // Int_Bootloader_jump_to_app();


    App_bootloader_work();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
