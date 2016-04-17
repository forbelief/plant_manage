/*
 * simulat_timer.h - ʵ��һ��ģ�ⶨʱ��
 */

#ifndef SIMULAT_TIMER_H
#define SIMULAT_TIMER_H

#include <stdint.h>

/*
 * ģ�ⶨʱ��������
 */
#define SIMULAT_TIMER_NUM   3



typedef struct st_register_
{
    uint8_t count;      // ��ʱ������
    uint32_t cmr;       // �ȽϼĴ���
    struct
    {
        uint8_t ten : 1;    // ʹ�ܱ�־
        uint8_t tov : 1;    // �����־
        uint8_t tfc : 1;    // �������б�־
        uint8_t tcf : 1;    // �Ƚϱ�־
    } flag;
} st_register;


typedef enum st_mode_
{
    COMPARE = 0,
    NO_COMPARE
} st_mode;

/*
 * st_init() - ��ʼ��ʵ��ģ�ⶨʱ�������pit��ʱ��
 *
 * ������ʱΪ1ms
 */
void st_base_init(void);


/*
 * st_init() - ��ʼ��һ��������ŵ�ģ�ⶨʱ��
 * @n: ��ʱ�����
 * @st_m: ��ʱģʽ
 * @cmr_v: ��ʱ���Ƚ�ֵ���ڷǱȽ�ģʽ����Ϊ����ֵ
 *
 * �����������0��ʾ��������˳�ʼ�����񣬷���~0��ʾ��ʱ���Ѵ򿪻򲻴���
 */
uint8_t st_init(uint8_t n, st_mode st_m, uint32_t cmr_v);


/*
 * st_close() - �ر�һ��������ŵ�ģ�ⶨʱ��
 * @n: ��ʱ�����
 */
void st_close(uint8_t n);

/*
 * st_tcf() - ��ѯtcfλ����
 * @n: ��ʱ�����
 *
 * ���������û�й��Ƚ�ֵ��ʱ�򷵻�0���ѹ��Ƚ�ֵ��ʱ�򷵻�1�����ڲ����ڻ�δ��
 */
uint8_t st_tcf(uint8_t n);



/*
 * st_tov() - ��ѯtovλ����
 * @n: ��ʱ�����
 *
 * ���������û�����ʱ����0��û�����ʱ����1�����ڲ����ڻ�δ��
 */
uint8_t st_tov(uint8_t n);



/*
 * st_count() - ��ѯ��ǰģ�ⶨʱֵ
 * @n: ��ʱ�����
 *
 * ���ڲ����ڻ�δ��
 */
uint32_t st_count(uint8_t n);



/********************ISR******************/

/*
 * st_base() -��ʱ������ģ��N·��ʱ��
 *
 * ��ʱ����С�������������������õ�����
 */
void st_base(void);







#endif /* SIMULAT_TIMER_H */
