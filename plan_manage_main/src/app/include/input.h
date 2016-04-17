/* 
 * �ļ���input.h
 * ���ܣ����������������ת��Ϊ��Ƭ���ڴ��е�����
 */

#ifndef INPUT_H
#define INPUT_H


typedef struct InData_
{
    uint8_t isWifiOn : 1;
    uint8_t isPlanMode : 1;

    uint8_t isRedL : 1;
    uint8_t isBlueL : 1;
    uint8_t isUvbL : 1;

    uint8_t knobV;
} InData;


void InitInput(void);

void 











#endif // INPUT_H
