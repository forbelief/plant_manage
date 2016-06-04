/* key.h - ��Ⲣ�ɼ�����������
 * �����������룬�޼���������˫����������
 * ʹ��һ����ʱ��ÿ10ms�ɼ�һ��io״̬��ʵ�ֵġ�
 */

#ifndef KEY_H
#define KEY_H


/*
 * key_mean - ������״̬��־����
 * N_KEY��ʾ�޼�����״̬��S_KEY��ʾ������D_KEY��ʾ˫����L_KEY��ʾ����
 */
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




void key_init(void);
void key_detect(void);
key_mean get_key_mean(key_type key_t);
void clear_key_m(void);




#endif // KEY_H
