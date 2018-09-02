#include "Board_SoftCom.h"
#include "APP_SoftTimer.h"
enum{
	COM_START_BIT,
	COM_D0_BIT,
	COM_D1_BIT,
	COM_D2_BIT,
	COM_D3_BIT,
	COM_D4_BIT,
	COM_D5_BIT,
	COM_D6_BIT,
	COM_D7_BIT,
	COM_STOP_BIT,
};
static uint8_t SoftComRecState = COM_STOP_BIT;
static uint8_t SoftComRecData = 0;
SoftCom_Data_t SoftComData;
static void Board_SoftComTimer4Init(SoftComBaud_e BuadRateCalForTimer)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)BuadRateCalForTimer; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���			 
}
void Board_SoftComInit(SoftComBaud_e BuadRateCalForTimer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStruct;
  RCC_APB2PeriphClockCmd(SOFTCOM_GPIOCLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	APP_RingBuffer_InitDataBuf(&SoftComData.ComData,8);
	APP_RingBuffer_InitQuene(&SoftComData.ComQuene,8);
	APP_RingBuffer_ClearQuene(&SoftComData.ComQuene);
	
#if USE_SOFTCOMTX == 1
	 //SoftWare Serial TXD
	GPIO_InitStructure.GPIO_Pin = SOFTCOM_TXPIN;	    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz	 
  GPIO_Init(SOFTCOM_PORT, &GPIO_InitStructure);	  				
  GPIO_SetBits(SOFTCOM_PORT,SOFTCOM_TXPIN); 		
#endif	
	 
#if USE_SOFTCOMRX == 1
	//SoftWare Serial RXD
	GPIO_InitStructure.GPIO_Pin = SOFTCOM_RXPIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(SOFTCOM_PORT, &GPIO_InitStructure);	 

	GPIO_EXTILineConfig(SOFTCOM_PORTSOURCR, SOFTCOM_PINSOURCR);
	EXTI_InitStruct.EXTI_Line = SOFTCOM_RXINTERLINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش����ж�
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStructure.NVIC_IRQChannel = SOFTCOM_IRQ ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	
	Board_SoftComTimer4Init(BuadRateCalForTimer);
#endif
}
//void IO_TXD(uint8_t sendpin,uint16_t baudtime,uint8_t Data)
//{
//	uint8_t i = 0;
//	PCout(sendpin) = 0;  
//	APP_SoftTimer_DelayUS(baudtime);
//	for(i = 0; i < 8; i++)
//	{
//		if(Data&0x01)
//			PCout(sendpin) = 1;  
//		else
//			PCout(sendpin) = 0; 	
//		
//		APP_SoftTimer_DelayUS(baudtime);
//		Data = Data>>1;
//	}
//	PCout(sendpin) = 1;
//	APP_SoftTimer_DelayUS(baudtime);
//}


//void VisualUSART_Send(uint8_t sendpin,u16 baudtime,u8 *buf, u8 len)
//{
//	u8 t;
//	if(sendpin==10)
//	{
//		PCout(11) = 0;
//		PCout(12) = 0;
//	}
//	else if(sendpin==11)
//	{
//		PCout(10) = 0;
//		PCout(12) = 0;
//	}
//	else if(sendpin==12)
//	{
//		PCout(10) = 0;
//		PCout(11) = 0;
//	}
//	for(t = 0; t < len; t++)
//	{
//		IO_TXD(sendpin,baudtime,buf[t]);
//	}
//	if(sendpin==10)
//	{
//		PCout(11) = 1;
//		PCout(12) = 1;
//	}
//	else if(sendpin==11)
//	{
//		PCout(10) = 1;
//		PCout(12) = 1;
//	}
//	else if(sendpin==12)
//	{
//		PCout(10) = 1;
//		PCout(11) = 1;
//	}
//}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(SOFTCOM_RXINTERLINE) != RESET)
	{
		if(GPIO_ReadInputDataBit(SOFTCOM_PORT,SOFTCOM_RXPIN) == 0) 
		{
			if(SoftComRecState == COM_STOP_BIT)
			{
				SoftComRecState = COM_START_BIT;
				TIM_Cmd(TIM4, ENABLE);
			}
		}
		EXTI_ClearITPendingBit(SOFTCOM_RXINTERLINE);
	}
}
void TIM4_IRQHandler(void)
{  
	if(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);	
		SoftComRecState++;
		if(SoftComRecState == COM_STOP_BIT)
		{
			TIM_Cmd(TIM4, DISABLE);
			if(SoftComRecData == 0xAA||SoftComData.ComData.pu8Databuf[0] == 0xAA)
			{
				SoftComData.ComData.pu8Databuf[SoftComData.ComData.u16Datalength] = SoftComRecData;
				SoftComData.ComData.u16Datalength++;
			}
			if(SoftComRecData == 0xFF)
			{
				APP_RingBuffer_EnQuene(&SoftComData.ComQuene,SoftComData.ComData.pu8Databuf,
								SoftComData.ComData.u16Datalength);
				memset(SoftComData.ComData.pu8Databuf,0,SoftComData.ComData.u16Datalength);
				SoftComData.ComData.u16Datalength = 0;
			}
			return;
		}
		if(GPIO_ReadInputDataBit(SOFTCOM_PORT,SOFTCOM_RXPIN))
		{
			SoftComRecData |= (1 << (SoftComRecState - 1));
		}
		else
		{
			SoftComRecData &= ~(1 << (SoftComRecState - 1));
		}	
  }		
}
