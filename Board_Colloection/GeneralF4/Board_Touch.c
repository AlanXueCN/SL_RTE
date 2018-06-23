#include "Board_Touch.h"
#include "Board_E2PROM.h"
#include "Board_LCDFSMC.h"
#include "R_Touch_Adjust.h"
//Ĭ��Ϊtouchtype=0������.
u8 touchtype=0;  //���������� 0Ϊ������  1Ϊ������ �������Ƴ�������
	
u16 Xdown=0; 		 
u16 Ydown=0;	   //�����������¾ͷ��صĵ�����ֵ
u16 Xup=0;
u16 Yup=0; 			 //������������֮��̧�𷵻صĵ�����ֵ 

u8  CMD_RDX=0XD0;
u8  CMD_RDY=0X90;

/****************************************************************************
* ��    ��: void TP_Write_Byte(u8 num) 
* ��    �ܣ�SPIд����,������ICд��1byte���� 
* ��ڲ�����num:Ҫд�������
* ���ز�������
* ˵    ����       
****************************************************************************/
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 
		APP_SoftTimer_DelayUS(1);
		TCLK=1;		 //��������Ч	        
	}		 			    
} 	

/****************************************************************************
* ��    ��: u16 TP_Read_AD(u8 CMD)
* ��    �ܣ�SPI������ ,�Ӵ�����IC��ȡadcֵ
* ��ڲ�����CMD:ָ��
* ���ز���: ����������	   
* ˵    ����       
****************************************************************************/  
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//������ʱ�� 	 
	TDIN=0; 	//����������
	TCS=0; 		//ѡ�д�����IC
	TP_Write_Byte(CMD);//����������
	APP_SoftTimer_DelayUS(6); 
	TCLK=0; 	     	    
	APP_SoftTimer_DelayUS(1);    	   
	TCLK=1;		//��1��ʱ�ӣ����BUSY
	APP_SoftTimer_DelayUS(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	//�½�����Ч  	    	   
		APP_SoftTimer_DelayUS(1);    
 		TCLK=1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//ֻ�и�12λ��Ч.
	TCS=1;		//�ͷ�Ƭѡ	 
	return(Num);   
}

/****************************************************************************
* ��    ��: u16 TP_Read_XorY(u8 xy)
* ��    �ܣ���ȡһ������ֵ(x����y)
* ��ڲ�����xoy:ָ�CMD_RDX/CMD_RDY
* ���ز���: ����������	   
* ˵    ����������ȡ5������,����Щ������������, Ȼ��ȥ����ͺ���1����,ȡƽ��ֵ       
****************************************************************************/ 
u16 TP_Read_XorY(u8 xoy)
{
	u16 i, j;
	u16 buf[5];
	u16 sum=0;
	u16 temp;
	
	for(i=0;i<5;i++)buf[i]=TP_Read_AD(xoy);		 		    
	for(i=0;i<5-1; i++)  //����
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=1;i<5-1;i++)sum+=buf[i];
	temp=sum/(5-2*1);
	return temp;   
} 

/****************************************************************************
* ��    ��: u8 TP_Read_XY(u16 *x,u16 *y)
* ��    �ܣ���ȡx,y����
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ������Сֵ��������50.       
****************************************************************************/ 
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XorY(CMD_RDX);
	ytemp=TP_Read_XorY(CMD_RDY);	  												   
	if(xtemp<50||ytemp<50)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}

/****************************************************************************
* ��    ��: u8 TP_Read_XY2(u16 *x,u16 *y)
* ��    �ܣ�����2�ζ�ȡ������IC
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ����        
****************************************************************************/

u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+50)||(x1<=x2&&x2<x1+50))  //ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+50)||(y1<=y2&&y2<y1+50)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  

/****************************************************************************
* ��    ��: u8 TP_Read_XY2(u16 *x,u16 *y)
* ��    �ܣ���һ����(2*2�ĵ�)	
* ��ڲ�����x,y:����
            color:��ɫ
* ���ز���: ��  
* ˵    ����        
****************************************************************************/
void TP_Draw_Point(u16 x,u16 y,u16 color)
{	    
	LCD_Handle.BrushColor = color;
	LCD_DrawPoint(x,y);  //���ĵ� 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}	


u16 x;
u16 y;
u8 TPtime;

/****************************************************************************
* ��    ��: void TP_Scan(u8 tp)
* ��    �ܣ���������ɨ��	
* ��ڲ�����type:0,��Ļ����;1,��������(У׼�����ⳡ����)
* ���ز���: ��  
* ˵    ����        
****************************************************************************/
void TP_Scan(u8 type)
{			   
		Xup=0xffff;
    Yup=0xffff;	 
	if(PEN==0)//�а�������
	{
		if(type)TP_Read_XY2(&x,&y);//��ȡ��������
		else if(TP_Read_XY2(&x,&y))//��ȡ��Ļ����
		{
	 		x=xfac*x+xoff;//�����ת��Ϊ��Ļ����
			y=yfac*y+yoff;  
	 	} 	
		Xdown=320-y;
		Ydown=x;
		TPtime++;		   
	}else
	{    
		 if(TPtime>2)
		 {
			 		Xup=320-y;
		      Yup=x;	 
		 }
		 TPtime=0;
		 Xdown=0xffff;
     Ydown=0xffff;	 
	}
}	

//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
u8 Touch_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;	  
		
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOB,C,Fʱ��

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;//PB1/PB2 ����Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PF11�����������
	  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��	
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;//PC13����Ϊ�������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��	
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��	
   
		TP_Read_XY(&Xdown,&Ydown);//��һ�ζ�ȡ��ʼ��	 
		AT24C02_Init();		  //��ʼ��24CXX
		if(TP_Get_Adjdata())return 0;//�Ѿ�У׼
		else			   //δУ׼
		{ 										    
			LCD_Clear(WHITE);//����
			TP_Adjust();  	//��ĻУ׼ 
			TP_Save_Adjdata();	 
		}			
		TP_Get_Adjdata();	
	  return 1; 									 
}
