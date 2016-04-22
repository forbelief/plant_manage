
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#include "include/pm_init.h"
#include "include/simulat_timer.h"
#include "include/key.h"
#include "include/knob.h"
#include "include/tft.h"
#include "include/config.h"
#include "include/pm_time.h"
#include "include/plan_handle.h"


void main(void)
{
    
    pm_init();

    st_init(0, COMPARE, 10);    // ��Ϊ����ɨ�躯���Ķ�ʱʹ�ã���simulat_timer.c�ĵײ���ʹ��
    st_init(1, COMPARE, 500);

    st_init(2, COMPARE, 1);
    st_init(3, COMPARE, 500);

    calendar_info sys_cal = {
        0, 1, 2, 3, 4, 2012, 0, 1
    };
    ds1302_set_time(&sys_cal);

    knob_enable();
    
    while (1)
    {
        /*
         * ʱ���Լ��ƻ�����
         */
        if (st_tcf(1) == 1)
        {
            maintain_system_time();

            calendar_info st= get_system_time();
            *get_value_of_kvp("st_y", 0) = st.year;
            *get_value_of_kvp("st_mo", 0) = st.month;
            *get_value_of_kvp("st_d", 0) = st.mday;
            *get_value_of_kvp("st_h", 0) = st.hour;
            *get_value_of_kvp("st_mi", 0) = st.min;
            *get_value_of_kvp("st_s", 0) = st.sec;
            tft_page_refresh();

            plan_handle();
        }

        /*
         * ������أ��϶�ʱ����Ӧ
         */
        if (st_tcf(2) == 1)
        {
            static int32_t log = 0, knob_v;
            if (log != (knob_v = get_knob_val()))
            {
                printf("knob = %d\n", knob_v);
                log = knob_v;
            }
        }

        /*
         * ������أ��ϳ�ʱ����Ӧ
         */
        if (st_tcf(3) == 1)
        {
            calendar_info st = get_system_time();
            printf("%d �� %d �� %d ���� %d �� %d ʱ %d �� %d ��\n", 
                    st.year, st.month, st.wday, st.mday, 
                    st.hour, st.min, st.sec);
            printf("sec = %d\n", calendar_to_sec(&st));
        }
        
        /*
         * ��������ע�Ჿ��
         */
        switch (get_key_mean(UP_KEY))
        {
        case N_KEY:
            //printf("�޼�\n");
            break;
        case S_KEY:
            printf("����\n");
            tft_left();
            break;
        case D_KEY:
            printf("˫��\n");
            tft_up();
            break;
        case L_KEY:
            printf("����\n");
            break;
        default:
            break;
        }
        switch (get_key_mean(DOWN_KEY))
        {
        case N_KEY:
            //printf("�޼�\n");
            break;
        case S_KEY:
            printf("����\n");
            tft_right();
            break;
        case D_KEY:
            printf("˫��\n");
            tft_down();
            break;
        case L_KEY:
            printf("����\n");
            break;
        default:
            break;
        }
        switch (get_key_mean(OK_KEY))
        {
        case N_KEY:
            //printf("�޼�\n");
            break;
        case S_KEY:
            printf("����\n");
            tft_ok();
            break;
        case D_KEY:
            printf("˫��\n");
            break;
        case L_KEY:
            printf("����\n");
            break;
        default:
            break;
        }
        switch (get_key_mean(RET_KEY))
        {
        case N_KEY:
            //printf("�޼�\n");
            break;
        case S_KEY:
            printf("����\n");
            tft_ret();
            break;
        case D_KEY:
            printf("˫��\n");
            break;
        case L_KEY:
            printf("����\n");
            break;
        default:
            break;
        }
    }
    return;
}
