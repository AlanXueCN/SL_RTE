#ifndef __BOARD_TOUCH_H
#define __BOARD_TOUCH_H
#include "APP_Include.h"
#include "BSP_Include.h"
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
extern u8  CMD_RDX;
extern u8  CMD_RDY;
//������оƬ��������	   
#define PEN  		PFin(11)  	//T_PEN
#define DOUT 		PBin(2)   	//T_MISO
#define TDIN 		PBout(1)  	//T_MOSI
#define TCLK 		PAout(5)  	//T_SCK
#define TCS  		PBout(0)  	//T_CS 
//����������������
extern	u8 touchtype;  //���������� 0Ϊ������  1Ϊ������ �������Ƴ�������
extern	u16 Xdown; 		 
extern	u16 Ydown;	   //�����������¾ͷ��صĵ�����ֵ
extern  u16 Xup;
extern  u16 Yup;       //������������֮��̧�𷵻صĵ�����ֵ  
//����������
/*********������SPIͨ�����ȡ������ADֵ*********************/
void TP_Write_Byte(u8 num);				  //�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 TP_Read_XorY(u8 xy);							//���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);				//˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);			//���ζ�ȡ��˫���������ȡ

/*********��������ʼ������*********************/
u8 Touch_Init(void);						 	//��ʼ��
/*********������ɨ�谴������*********************/
void TP_Scan(u8 tp);	        //tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
void TP_Draw_Point(u16 x,u16 y,u16 color);	//�ڰ��´���һ����				


#endif
