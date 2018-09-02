#ifndef __RTE_CONFIG_H
#define __RTE_CONFIG_H
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
//<o>  RTE_VERSION
//<i> RTE�汾�ţ����嶨�������ĵ���
#define RTE_VERSION                 "2.1.0826"
// <e> RTE_USE_BGET
// <i> RTE_BGet������Ķ�̬�ڴ棬���ɶ����ɣ�ÿ����䲻ͬ��С��
// <i> ��SL_RTE�У�RTE_BGet����ؽṹ��Ϊ��̬����ʽ������ģ��������RTE_BGetʵ�ֶ�̬����
#ifndef RTE_USE_BGET
#define RTE_USE_BGET                1 
#endif
// <o> BGET_MEMSIZE
// <i> BGet�ڴ����Ĵ�С
// <i> Ĭ�ϴ�С: 1024
#ifndef BGET_MEMSIZE
#define BGET_MEMSIZE              1024
#endif
// </e>
// <e> RTE_USE_STDIO
// <i> c���Ա�׼stdio
// <i> ע�⣺��Դ���޵�����¿��Կ��ǲ�ʹ��
#ifndef RTE_USE_STDIO
#define RTE_USE_STDIO              1
#endif
// </e>
// <e> RTE_USE_STDLIB
// <i> c���Ա�׼stdlib
// <i> ע�⣺��Դ���޵�����¿��Կ��ǲ�ʹ��
#ifndef RTE_USE_STDLIB
#define RTE_USE_STDLIB             1
#endif
// </e>
// <e> RTE_USE_HUMMANINTERFACE
// <i> �˻������ӿ�
#ifndef RTE_USE_HUMMANINTERFACE
#define RTE_USE_HUMMANINTERFACE   1 
#endif
	// <e> HI_USE_SHELL
	// <i> ��shell����
	// <i> ע��: һ��shellָ����ռ�õ�ram�ռ�Ϊ12�ֽڣ��˴�С��ͳ��const char���������ģ�
	#ifndef HI_USE_SHELL
	#define HI_USE_SHELL          1
	#endif
		// <o> HI_SHELL_MAX_ARGS
		// <i> ����shell���Խ�������������ָ�������
		// <i> Ĭ�ϴ�С: 8
		#define HI_SHELL_MAX_ARGS      8
		// <o> HI_SHELL_MAX_BUFSIZE
		// <i> ����shellʹ�õ����ݻ����С
		// <i> Ĭ�ϴ�С: 32 [bytes] 
		#define HI_SHELL_MAX_BUFSIZE    		32
	// </e>
	// <e> HI_USE_RINGBUF
	// <i> ���ζ���
	#ifndef HI_USE_RINGBUF
	#define HI_USE_RINGBUF          1
	#endif
	// </e>
// </e>
// <e> RTE_USE_ROUNDROBIN
// <i> ��̬ʱ��Ƭ��ת����
// <i> ע��: һ��SoftTimer��ռ�õ�ram�ռ�Ϊ24�ֽڣ��˴�С��ͳ��const char���������ģ�
#ifndef RTE_USE_ROUNDROBIN
#define RTE_USE_ROUNDROBIN   1 
#endif
	// <e> RTE_USE_OS
	// <i> ʵʱ����ϵͳ
	#ifndef RTE_USE_OS
	#define RTE_USE_OS          1
	#endif
	// </e>
	#if RTE_USE_OS == 1
	
	#endif
// </e>
// <e> RTE_USE_STATEMACHINE
// <i> ״̬��ģ��
// <i> ע��: һ��StateMachine��ռ�õ�ram�ռ�Ϊ24�ֽڣ��˴�С��ͳ��const char���������ģ�
#ifndef RTE_USE_STATEMACHINE
#define RTE_USE_STATEMACHINE   1 
#endif
// </e>
// <e> RTE_USE_KVDB
// <i> KV���ݿ�
#ifndef RTE_USE_KVDB
#define RTE_USE_KVDB   1 
	// <o> KVDB_ERASE_MIN_SIZE
	// <i> KVDB��С������λ��С����FLASH�ṹ�й� ��λ��K��
	#ifndef KVDB_ERASE_MIN_SIZE
	#define KVDB_ERASE_MIN_SIZE         (128 * 1024)              /* it is 128K for compatibility */
	#endif
	// <e> KVDB_USE_PFS
	// <i> ���籣��ģʽ
	#ifndef KVDB_USE_PFS
	#define KVDB_USE_PFS          0
	#endif
	// </e>
	// <e> KVDB_USE_AUTO_UPDATE
	// <i> �Զ����£��������£�
	#ifndef KVDB_USE_AUTO_UPDATE
	#define KVDB_USE_AUTO_UPDATE  0
	#endif
	// </e>
	// <o> KVDB_USER_SETTING_SIZE
	// <i> �û����û���������С
	#ifndef KVDB_USER_SETTING_SIZE
	#define KVDB_USER_SETTING_SIZE             2048
	#endif
	// <o> KVDB_ADDR_OFFSET
	// <i> KVDB��ַƫ�ƣ�����FLASH�׵�ַ��Ϊʵ�ʵ�ַ��Ҳ���û���������С ��λ��K��
	#ifndef KVDB_ADDR_OFFSET
	#define KVDB_ADDR_OFFSET             			 128 * 1024 
	#endif
	// <o> FLASH_BASE
	// <i> FLASH�׵�ַ
	#ifndef KVDB_FLASH_BASE
	#define KVDB_FLASH_BASE                    0x00000000
	#endif
	// <o> KVDB_FM_VER_NUM
	// <i> �̼��汾�������⵽��Ʒ�洢�İ汾�����趨�汾�Ų�һ�£����Զ�׷��Ĭ�ϻ������������������Ļ���������
	#ifndef KVDB_FM_VER_NUM
	#define KVDB_FM_VER_NUM                 0
	#endif
#endif
// </e>
#endif
/****************** (C) COPYRIGHT SuChow University Shannon********************/
