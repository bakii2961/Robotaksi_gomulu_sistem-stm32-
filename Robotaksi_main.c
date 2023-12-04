
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


extern DMA_HandleTypeDef hdma_uart4_rx;
#define RxBuffersSize 15
char RxBuffer[RxBuffersSize];
int value1;
int value2;
int value3;
int value4;
int sol;
int sag;
int len;
int neg =-1;
char baki[3];
int sonDeger=0;
int donuyorMu=0;
int t=20;
int pwm_val = 30;
int value ;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
	if(huart->Instance== UART4)
	{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart4, RxBuffer, RxBuffersSize);
	__HAL_DMA_DISABLE_IT(&hdma_uart4_rx,DMA_IT_HT);
	for(int i= Size;i<RxBuffersSize;i++)
		RxBuffer[i]=0;
	    value1 = atoi(RxBuffer);
	    if (value1 %5 == 0){ // if dögüsü ekledim
	    	value = value1;
	    }
	    if(donuyorMu == 0)
		    {
				if(value > sonDeger)
				{
					sol = 0;
					sag = (value - sonDeger);
					donuyorMu=1;
				}
				else if (value<sonDeger)
				{
					sag = 0;
					sol = -1 * (value - sonDeger);
					donuyorMu=1;
				}
				else
				{
					sag=0;
					sol=0;

				}

				sonDeger = value;
			}

	    value2 = RxBuffer[5];
	    value3 = RxBuffer[6];
	    value4= RxBuffer[7];
	}
	}

void SystemClock_Config(void);

#define DIR_PIN GPIO_PIN_2
#define DIR_PORT GPIOA
#define STEP_PIN GPIO_PIN_3
#define STEP_PORT GPIOA
#define ENABLE_PIN GPIO_PIN_0
#define ENABLE_PORT GPIOA
#define TIM_8_PORT GPIOC
#define TIM_8_PIN GPIO_PIN_6

int stepDelay = 350;

void microDelay (uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim1, 0);
  while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
}

int main(void)
{
  
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  HAL_UARTEx_ReceiveToIdle_DMA(&huart4, RxBuffer, RxBuffersSize);
  __HAL_DMA_DISABLE_IT(&hdma_uart4_rx,DMA_IT_HT);
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
  while (1)
  {

if (value2==49){

	HAL_GPIO_WritePin(led9_GPIO_Port, led9_Pin, GPIO_PIN_SET);
 }


if (value2==48){

	HAL_GPIO_WritePin(led9_GPIO_Port, led9_Pin, GPIO_PIN_RESET);
}


if (value3==49){
	HAL_GPIO_WritePin(led10_GPIO_Port, led10_Pin, GPIO_PIN_SET);
  }


if (value3==48){
	HAL_GPIO_WritePin(led10_GPIO_Port, led10_Pin, GPIO_PIN_RESET);
}


if (value4==49){

	HAL_GPIO_WritePin(led11_GPIO_Port, led11_Pin, GPIO_PIN_SET);
	  	  	  	 }


if (value4==48){

	HAL_GPIO_WritePin(led11_GPIO_Port, led11_Pin, GPIO_PIN_RESET);
}




		  	 

void drk(){
	int x;
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_SET);
	for(x=0; x<sag; x=x+1)
 {
		HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_SET);
		microDelay(300);
		HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_RESET);
		microDelay(stepDelay);
}


HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);
	for(x=0; x<sol; x=x+1)
{
		HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_SET);
		microDelay(300);
		HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_RESET);
		microDelay(stepDelay);
}
donuyorMu=0;
}

if(donuyorMu==1)
{
drk();
}

  }
  


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

 
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

 
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}


void Error_Handler(void)
{
  
  __disable_irq();
  while (1)
  {
  }
  
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
  
}
#endif /* USE_FULL_ASSERT */
