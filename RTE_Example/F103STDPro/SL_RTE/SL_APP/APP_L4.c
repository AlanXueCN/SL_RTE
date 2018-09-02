#include "APP_L4.h"
#include "stm32L4xx.h"
//�������ģʽ	  
void APP_SYS_Standby(void)
{ 	
	__WFI();			    //ִ��WFIָ��,�������ģʽ		 
}	     
//ϵͳ��λ   
void APP_SYS_SoftReset(void)
{   
	SCB->AIRCR =0X05FA0000|(uint32_t)0x04;	  
} 	
//���������ж�
void APP_SYS_EnableInterrupts(void)
{
	/* Disable interrupts */
	__enable_irq();
}
void APP_SYS_DisableInterrupts(void)
{
	/* Disable interrupts */
	__disable_irq();
}
void APP_Assert(char* file, uint32_t line)
{ 
	APP_STDPrintf("[ASSERT]Wrong parameters value: file %s on line %d\n", file, line);
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
}
void APP_Init(void)
{
#if USE_MPU == 1
	MPU_Memory_Protection();
#endif
	APP_SoftTimer_Init();
	APP_SoftTimer_Create(TIMER_SYS,10, 1, 1, SysTimer_Callback, (void *)0);
	APP_MemInit();
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); 
#if USE_DEBUG == 1
	APP_Debug_Init();
#endif
	HAL_Init();
}
