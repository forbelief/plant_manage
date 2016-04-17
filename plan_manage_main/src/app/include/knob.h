/*
 * knob.h - �ṩ��ť����Ľӿں���
 *
 * ʹ�õ���һ��������ť�����ü������ɼ�����
 */

#ifndef KNOB_H
#define KNOB_H

#include "arm_cm0.h"
#include "vectors.h"




/*
 * knob_init() - Ϊ��ť�����ִ�еĳ�ʼ��
 *
 * ��ť�ļ���ʼ����Ĭ���ǽ�ֹ��
 */
void knob_init(void);



/*
 * knob_enable() - ʹ����ť������
 */
void knob_enable(void);


/*
 * knob_disable() - ��ֹ��ť������
 */
void knob_disable(void);


/*
 * knob_clear() - �����ťֵ
 */
void knob_clear(void);

/*
 * get_knob_val() - ������ťֵ
 */
int32_t get_knob_val(void);


/*
 * knob_detect() - ��ť���
 *
 * ���������Ҫ����ť��������A���������������ʱ���ã����������ж�ʱ
 */
void knob_detect(void);


#endif /* KNOB_H */
