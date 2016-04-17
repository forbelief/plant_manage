/*
 * debug.h - ����
 */

#ifndef DEBUG_H
#define DEBUG_H


#define REC_DEBUG_UART_LEN  20


enum rec_uart_flag_
{
    A = 0, C, B, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};




extern volatile float recval_debug_uart[26];




/*
 * debug_init() - ��ʼ�����Թ���
 */
void debug_init(void);

/*
 * str_to_fp() - ���ַ���ת��Ϊ������
 * @str: �ַ�����ַ
 * @len: �ַ�������
 *
 * �������������ַ�����ʽӦ����c���Ա�׼�����ʽ������û��'\0'
 */
float str_to_fp(char str[], uint8_t len);

/*
 * rec_debug_uart() - ����uart����ĵ�����Ϣ����ת��Ϊ������
 *
 */
void rec_debug_uart(void);









#endif /* DEBUG_H */
