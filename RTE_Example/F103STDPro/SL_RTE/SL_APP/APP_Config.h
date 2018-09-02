/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
// <e> MEM_USE
// <i> SL_Lib�Դ����ڴ����
#define USE_MEM                1 
//<o> Global Dynamic Memory size [bytes]
//<i> ����SL_Lib��Ӧ�õĶ�̬�ڴ棬���ɶ����ɣ�ÿ����䲻ͬ��С��
//<i> Ĭ�ϴ�С: 4096
#ifndef MEM_BASE_SIZE
#define MEM_BASE_SIZE          1024
#endif
// </e>
// <e> KEY_USE
// <i> SL_Lib�Դ��Ļ��尴����
#define USE_KEY                0 
//<o> Key Pool size [bytes] 
//<i> ��������ش�С��
//<i> Ĭ�ϴ�С: 10
#define KEY_POOL_SIZE     		10  
//<o> Key Filter time [10ms] 
//<i> �����˲�ʱ�䣬ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼���
//<i> Ĭ�ϴ�С: 5*10ms
#define KEY_FILTER_TIME       5                  
//<o> Key Long Press time [10ms] 
//<i> ������������ʱ�䣬����1�룬��Ϊ�����¼���
//<i> Ĭ�ϴ�С: 100*10ms
#define KEY_LONG_TIME         100			            
// </e>
// <e> DEBUG_USE
// <i> SL_Lib�Դ��Ĵ��ڵ��Թ��ߡ�
/* Degug ----------------------------------------------------------*/
#define USE_DEBUG         		1
//<o> Debug Buffer Size [bytes] 
//<i> ���ڴ��ڵ��ԵĽ��ջ���Ĵ�С��
//<i> Ĭ�ϴ�С: 128
#define DEBUG_BUF_SIZE    		32
// </e>
// <e> OS_USE
// <i> SL_Lib�Ƿ������ڲ���ϵͳ�����¡�
#define USE_OS            		1
// </e>
// <e> SOFTCOMRX_USE
// <i> �Ƿ�ʹ��SL_Lib�Դ���ģ�⴮�ڽ��չ��ܡ�
#define USE_SOFTCOMRX         0
// </e>
// <e> SOFTCOMRX_USE
// <i> �Ƿ�ʹ��SL_Lib�Դ���ģ�⴮�ڷ��͹��ܡ�
#define USE_SOFTCOMTX         0
// </e>
// <e> MPU_USE
// <i> �Ƿ�ʹ��MPU���ܣ�����Դ���MPU��Ƭ�ӡ�
#define USE_MPU            0
// </e>
// <e> LOWPOWER_USE
// <i> �Ƿ�ʹ�õ͹��Ĺ��ܣ������ʹ��RTX������¡�
#define USE_LOWPOWER            1
// </e>
#endif
/****************** (C) COPYRIGHT SuChow University Shannon*****END OF FILE****/

