/*
 * simulat_timer.h - ʵ��һ��ģ�ⶨʱ��
 */

#include "include.h"

#include "include/config.h"
#include "include/key.h"

#include "include/simulat_timer.h"

static volatile st_register st_r[SIMULAT_TIMER_NUM];



/**
 * st_init() - ��ʼ��ʵ��ģ�ⶨʱ�������pit��ʱ��
 *
 * ������ʱΪ1ms
 */
void st_base_init(void)
{
    uint32_t cnt;

    cnt = bus_clk_khz * 1;
    pit_init(SIMULAT_TIMER_PITX, cnt);  // 1msӲ����ʱ�ж�����
    return;
}


/**
 * st_init() - ��ʼ��һ��������ŵ�ģ�ⶨʱ��
 * @n: ��ʱ�����
 * @st_m: ��ʱģʽ
 * @cmr_v: ��ʱ���Ƚ�ֵ���ڷǱȽ�ģʽ����Ϊ����ֵ
 *
 * �����������0��ʾ��������˳�ʼ�����񣬷���~0��ʾ��ʱ���Ѵ򿪻򲻴���
 */
uint8_t st_init(uint8_t n, st_mode st_m, uint32_t cmr_v)
{
    if (st_r[n].flag.ten == 1 || n >= SIMULAT_TIMER_NUM)
    {
        return ~0;
    }
    st_r[n].flag.ten = 1;
    switch(st_m)
    {
    case COMPARE:
        st_r[n].flag.tfc = 1;
        st_r[n].cmr = cmr_v;
        break;
    case NO_COMPARE:
        st_r[n].flag.tfc = 0;
        break;
    default:
        break;
    }
    return 0;
}


/**
 * st_close() - �ر�һ��������ŵ�ģ�ⶨʱ��
 * @n: ��ʱ�����
 */
void st_close(uint8_t n)
{
    st_r[n].flag.ten = 0;
    return;
}

/**
 * st_tcf() - ��ѯtcfλ����
 * @n: ��ʱ�����
 *
 * ���������û�й��Ƚ�ֵ��ʱ�򷵻�0���ѹ��Ƚ�ֵ��ʱ�򷵻�1�����ڲ����ڻ�δ��
 */
uint8_t st_tcf(uint8_t n)
{
    uint8_t temp;

    if (st_r[n].flag.tcf == 1)
    {
        st_r[n].flag.tcf = 0;
        temp = 1;
    }
    else
    {
        temp = 0;
    }
    return temp;
}

/**
 * st_tov() - ��ѯtovλ����
 * @n: ��ʱ�����
 *
 * ���������û�����ʱ����0��û�����ʱ����1�����ڲ����ڻ�δ��
 */
uint8_t st_tov(uint8_t n)
{
    uint8_t temp;
    if (st_r[n].flag.tov == 1)
    {
        st_r[n].flag.tov = 0;
        temp = 1;
    }
    else
    {
        temp = 0;
    }
    return temp;
}

/**
 * st_count() - ��ѯ��ǰģ�ⶨʱֵ
 * @n: ��ʱ�����
 *
 * ���ڲ����ڻ�δ��
 */
uint32_t st_count(uint8_t n)
{
    return st_r[n].count;
}


/**
 * st_base() -��ʱ������ģ��N·��ʱ��
 *
 * ��ʱ����С�������������������õ�����
 */
void st_base(void)
{
    uint8_t i;

    for (i = 0; i < SIMULAT_TIMER_NUM; i++)
    {
        if (st_r[i].flag.ten == 1)          // ʹ��
        {
            if (++st_r[i].count == 0)       // ���
            {
                st_r[i].flag.tov = 1;
            } // ���
            if (st_r[i].flag.tfc == 1)      // �Ƚ�
            {
                if (st_r[i].count == st_r[i].cmr)
                {
                    st_r[i].count = 0;
                    st_r[i].flag.tcf = 1;
                }
            } // �Ƚ�
        } // ʹ��
    }
    
    if (st_tcf(0) == 1)
    {
        key_detect();
    }
    
    return;
}
