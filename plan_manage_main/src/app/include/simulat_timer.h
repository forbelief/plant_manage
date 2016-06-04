/*
 * simulat_timer.h - ʵ��һ��ģ�ⶨʱ��
 */

#ifndef SIMULAT_TIMER_H
#define SIMULAT_TIMER_H

#include <stdint.h>

/*
 * ģ�ⶨʱ��������
 */
#define SIMULAT_TIMER_NUM   5



typedef struct st_register_
{
    uint32_t count;      // ��ʱ������
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


void st_base_init(void);
uint8_t st_init(uint8_t n, st_mode st_m, uint32_t cmr_v);
void st_close(uint8_t n);
uint8_t st_tcf(uint8_t n);
uint8_t st_tov(uint8_t n);
uint32_t st_count(uint8_t n);
void st_base(void);







#endif /* SIMULAT_TIMER_H */
