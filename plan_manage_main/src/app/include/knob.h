/*
 * knob.h - �ṩ��ť����Ľӿں���
 *
 * ʹ�õ���һ��������ť�����ü������ɼ�����
 */

#ifndef KNOB_H
#define KNOB_H






void knob_init(void);


void knob_enable(void);

void knob_disable(void);

void knob_clear(void);

int32_t get_knob_val(void);

void knob_detect(void);


#endif /* KNOB_H */
