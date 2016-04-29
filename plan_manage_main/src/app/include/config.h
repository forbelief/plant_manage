/*
 * config.h - ������������
 *
 */

#ifndef CONFIG_H
#define CONFIG_H




/*
 * ģ�ⶨʱ��ʹ�õĶ�ʱ��
 */
#define SIMULAT_TIMER_PITX      PIT1


/*
 * ����ģ��,��8266
 */
#define WIFI_UARTX      UART0   // PTA1,PTA2
#define DEBUG_UARTX     UART0   // PTA1,PTA2

/*
 * ��̨
 */
#define ORIENT_UARTX     UART1   // PTC3,PTC4                                                                             
#define RE_DE_PINX       PTC1

/*
 * ����ģ��
 */
#define UP_KEY_PINX     PTA4
#define DOWN_KEY_PINX   PTA5
#define OK_KEY_PINX     PTE20
#define RET_KEY_PINX    PTE30
#define AMS_KEY_PINX    PTD5
#define WIFI_KEY_PINX   PTD7
#define MR_KEY_PINX     PTE23
#define MB_KEY_PINX     PTA16
#define MUVB_KEY_PINX   PTD4
// knob�ñ�����PTA��PTD�˿ڵ�
#define KNOB_A_PINX     PTD7
#define KNOB_B_PINX     PTD6
#define KNOB_KEY_PINX   PTE20



/*
 * ��ʾģ��
 */
#define TFT_UARTX       UART2  // PTD2,PTD3


/*
 * ʱ��ģ��
 */
#define DS1302_CE_PINX      PTB2
#define DS1302_CLK_PINX     PTB1
#define DS1302_IO_PINX      PTB3

/*
 * ����ִ��ģ��
 *
 */
#define LGRED_PINX      PTC17
#define LGBLUE_PINX     PTC16
#define LGUVB_PINX      PTC13
#define WATER_PINX      PTD1

/*
 * led���ƣ���ѡ��������˿�
 */
#define LG1_PINX      PTC9
#define LG2_PINX      PTC8
#define LG3_PINX      PTC7
#endif // CONFIG_H
