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
 * 8266
 */
#define WIFI_UARTX      UART0   // PTA1,PTA2

/*
 * ����ģ��,����̨
 */
#define DEBUG_UARTX		UART1   // PTC3,PTC4


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
// knob�ñ�����PTA��PTD�˿ڵ�
#define KNOB_A_PINX     PTD6
#define KNOB_B_PINX     PTD7
#define KNOB_KEY_PINX   PTD4



/*
 * ��ʾģ��
 */
#define TFT_UARTX       UART2  // PTD2,PTD3


/*
 * ʱ��ģ��
 */
#define DS1302_CE_PINX      PTB2
#define DS1302_CLK_PINX     PTB1
#define DS1302_IO_PINX      PTB0

/*
 * ����ִ��ģ��
 *
 */
#define LGRED_PINX      PTC12
#define LGBLUE_PINX     PTC13
#define LGUVB_PINX      PTC15
#define WATER_PINX      PTC16

/*
 * led���ƣ���ѡ��������˿�
 */

#endif // CONFIG_H
