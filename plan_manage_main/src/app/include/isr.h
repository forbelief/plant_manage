/*
 * isr.h - ���º궨���жϺţ���ӳ���ж�����������жϺ�����ַ��
 * ʹ��ָ����������жϷ�������
 *
 * ֻ����"vectors.c"���������ұ�����"vectors.h"�����ĺ��棡����
 */

#ifndef ISR_H
#define ISR_H 



#ifdef VECTOR_028
#undef VECTOR_028
#define VECTOR_028	UART0_isr
#endif

#ifdef VECTOR_038
#undef VECTOR_038
#define VECTOR_038	PIT_isr
#endif

#ifdef VECTOR_046
#undef VECTOR_046
#define VECTOR_046	PTA_isr
#endif


#ifdef VECTOR_047
#undef VECTOR_047
#define VECTOR_047	PTD_isr
#endif



void PIT_isr(void);


void PTA_isr(void);


void PTD_isr(void);


void UART0_isr(void);


#endif /* ISR_H */
