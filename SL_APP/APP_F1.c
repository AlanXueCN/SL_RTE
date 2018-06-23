#include "APP_Include.h"
#include "BSP_ComF1.h"
#if USE_KEY == 1
	#include "BSP_Key.h"
#endif
int fputc(int ch, FILE *f)
{
	if(ch=='\n')
		BSP_USART_Putc(COM_1,'\r');
  BSP_USART_Putc(COM_1,ch);
  return ch;
}
void APP_SYS_Standby(void)
{ 	
	__WFI();			    //ִ��WFIָ��,�������ģʽ		 
}	     

void APP_SYS_SoftReset(void)
{   
	SCB->AIRCR =0X05FA0000|(uint32_t)0x04;	  
} 

void APP_Assert(char* file, uint32_t line)
{ 
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	/* ����һ����ѭ��������ʧ��ʱ������ڴ˴��������Ա����û���� */
	while (1)
	{
	}
}
/* Callback function for system */
static void SysTimer_Callback(void* UserParameters) {
#if USE_DEBUG == 1
	APP_Debug_Poll();
#endif
#if USE_KEY == 1
	BSP_KEY_Poll();
#endif
}
void APP_RunEnvInit(void)
{
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); 
#ifdef RTE_Compiler_EventRecorder
  // Initialize and start Event Recorder
  EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
#endif
	APP_BGet_Init();
#if USE_DEBUG == 1
	APP_Debug_Init();
#endif
	APP_SoftTimer_Init();
	APP_SoftTimer_Create(TIMER_SYS,10, 1, 0, SysTimer_Callback, (void *)0);	
}
