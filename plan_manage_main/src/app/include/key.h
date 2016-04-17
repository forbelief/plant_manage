/* key.h - ��Ⲣ�ɼ�����������
 * �����������룬�޼���������˫����������
 * ʹ��һ����ʱ��ÿ10ms�ɼ�һ��io״̬��ʵ�ֵġ�
 */

#ifndef KEY_H
#define KEY_H


typedef enum key_mean_
{
    N_KEY = 0, S_KEY, D_KEY, L_KEY
} key_mean;



/*
 * key_type - ����������
 */
typedef enum key_type_
{
    UP_KEY = 0, DOWN_KEY, OK_KEY, RET_KEY, AMS_KEY, WIFI_KEY, MR_KEY, MB_KEY, MUVB_KEY
} key_type;



/*
 * key_init() - ����ģ���ʼ��
 *
 */
void key_init(void);


/*
 * key_detect() - ��ⰴ��
 *
 * Ҫ��ÿ��10ms����һ��
 */
void key_detect(void);



/*
 * get_key_mean() - �õ�ָ���İ�����Ϣ
 *
 * �����������ȡ����Ϣ��Ὣ��Ӧ�������±��ΪN_KEY
 */
key_mean get_key_mean(key_type key_t);




void clear_key_m(void);




#endif // KEY_H
