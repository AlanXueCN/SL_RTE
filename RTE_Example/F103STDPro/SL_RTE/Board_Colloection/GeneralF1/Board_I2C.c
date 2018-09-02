#include "Board_I2C.h"
#include "APP_SoftTimer.h"
void I2C_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct; 
	RCC_APB2PeriphClockCmd( I2C_GPIOCLK, ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = I2C_SDAPin | I2C_SCLPin ;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct); 
}
static void I2C_SDASetOut(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = I2C_SDAPin;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);  
}
static void I2C_SDASetIn(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = I2C_SDAPin;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}
void I2C_StartSignal(void)
{
	I2C_SDASetOut();
	I2C_SDASetHigh();  
	I2C_SCLSetHigh(); 
	APP_SoftTimer_DelayUS(4);  
	I2C_SDASetLow();        
	APP_SoftTimer_DelayUS(4);    
	I2C_SCLSetLow();   
}
void I2C_StopSignal(void)
{
	I2C_SDASetOut();
	I2C_SCLSetLow();    
	I2C_SDASetLow(); 	
	APP_SoftTimer_DelayUS(4);               
	I2C_SCLSetHigh();  
	I2C_SDASetHigh(); 
	APP_SoftTimer_DelayUS(4); 
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�

uint8_t I2C_WaitAck(void)
{
	I2C_SDASetIn();      //SDA����Ϊ����  
	I2C_SDASetHigh();
	APP_SoftTimer_DelayUS(1);	   
	I2C_SCLSetHigh();
	APP_SoftTimer_DelayUS(1);	 
	uint8_t ucErrTime=0;
	while(I2C_SDARead())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_StopSignal();
			return 1;
		}
	}
	I2C_SCLSetLow();//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
static void I2C_Ack(void)
{
	I2C_SCLSetLow();
	I2C_SDASetOut();
	I2C_SDASetLow();
	APP_SoftTimer_DelayUS(2);
	I2C_SCLSetHigh();
	APP_SoftTimer_DelayUS(2);
	I2C_SCLSetLow();
}
//������ACKӦ��		    
static void I2C_NAck(void)
{
	I2C_SCLSetLow();
	I2C_SDASetOut();
	I2C_SDASetHigh();
	APP_SoftTimer_DelayUS(2);
	I2C_SCLSetHigh();
	APP_SoftTimer_DelayUS(2);
	I2C_SCLSetLow();
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void I2C_SendByte(uint8_t txd)
{                        
	uint8_t t;   
	I2C_SDASetOut(); 	    
	I2C_SCLSetLow();//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
		if(((txd&0x80)>>7))
			I2C_SDASetHigh();
		else
			I2C_SDASetLow();
		txd<<=1; 	  
		APP_SoftTimer_DelayUS(2);   //��TEA5767��������ʱ���Ǳ����
		I2C_SCLSetHigh();
		APP_SoftTimer_DelayUS(2); 
		I2C_SCLSetLow();	
		APP_SoftTimer_DelayUS(2);
	}	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t i,receive=0;
	I2C_SDASetIn();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
		I2C_SCLSetLow(); 
		APP_SoftTimer_DelayUS(2);
		I2C_SCLSetHigh();
		receive<<=1;
		if(I2C_SDARead())receive++;   
		APP_SoftTimer_DelayUS(1); 
	}					 
	if (!ack)
		I2C_NAck();//����nACK
	else
		I2C_Ack(); //����ACK   
	return receive;
}
