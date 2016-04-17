/* 
 * config.h - ������������
 *
 */

#ifndef CONFIG_H
#define CONFIG_H




/*
 * ģ�ⶨʱ��ʹ�õĶ�ʱ��
 */
#define SIMULAT_TIMER_PITX		PIT1


/*
 * ����ģ��
 */
#define DEBUG_UARTX		UART1



/*
 * ����ģ��
 */
#define UP_KEY_PINX     PTE20
#define DOWN_KEY_PINX   PTA17
#define OK_KEY_PINX     PTE23
#define RET_KEY_PINX    PTA4
#define AMS_KEY_PINX    PTA5
#define WIFI_KEY_PINX   PTD7
#define MR_KEY_PINX     PTE30
#define MB_KEY_PINX     PTA16
#define MUVB_KEY_PINX   PTD6

#define KNOB_A_PINX     PTD6
#define KNOB_B_PINX     PTD7
#define KNOB_KEY_PINX   PTD4    // PTD5



/*
 * ��ʾģ��
 */
#define TFT_UARTX       UART2  // PTD2,PTD3




#endif // CONFIG_H
