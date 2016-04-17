/*
 * knob.c - �ṩ��ť����Ľӿں���
 *
 * ʹ�õ���һ��������ť�����ü������ɼ�����
 */

#include <stdint.h>

#include "include.h"


#include "include/knob.h"
#include "include/config.h"



static volatile int32_t knob_val;




/*
 * knob_init() - Ϊ��ť�����ִ�еĳ�ʼ��
 *
 * ��ť�ļ���ʼ����Ĭ���ǽ�ֹ��
 */
void knob_init(void)
{
    gpio_Interrupt_init(KNOB_A_PINX, GPI_UP_PF, GPI_INP_EITH);
    gpio_Interrupt_init(KNOB_B_PINX, GPI_UP_PF, GPI_DISAB);
    // gpio_Interrupt_init(KNOB_KEY_PINX, GPI_UP_PF, GPI_INP_EITH);
    disable_irq(PortD_irq_no);

}


/*
 * knob_enable() - ʹ����ť������
 */
void knob_enable(void)
{
    enable_irq(PortD_irq_no);
    return;
}

/*
 * knob_disable() - ��ֹ��ť������
 */
void knob_disable(void)
{
    disable_irq(PortA_irq_no);
    return;
}



/*
 * knob_clear() - �����ťֵ
 */
void knob_clear(void)
{
    knob_val = 0;
    return;
}

/*
 * get_knob_val() - ������ťֵ
 */
int32_t get_knob_val(void)
{
    return knob_val;
}


/*
 * knob_detect() - ��ť���
 *
 * ���������Ҫ����ť��������A���������������ʱ���ã����������ж�ʱ
 */
void knob_detect(void)
{
    uint8_t knob_A, knob_B;

    knob_A = gpio_get(KNOB_A_PINX);
    knob_B = gpio_get(KNOB_B_PINX);
    if ((knob_A == 0 && knob_B == 1) || (knob_A == 1 && knob_B == 0))
    {
        knob_val++;
    }
    else // ��if ((knob_A == 0 && knob_B == 0) || (knob_A == 1 && knob_B == 1))
    {
        knob_val--;
    }
    return;
}
