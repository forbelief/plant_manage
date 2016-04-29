
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
#include "include/pm_flash.h"
#include "include/orient.h"



void main(void)
{

    printf("\n\n\n����ʼִ�У�����\n\n\n");

    pm_init();

    st_init(0, COMPARE, 10);    // ��Ϊ����ɨ�躯���Ķ�ʱʹ�ã���simulat_timer.c�ĵײ���ʹ��
    st_init(1, COMPARE, 100);

    st_init(2, COMPARE, 1);
    st_init(3, COMPARE, 500);

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
            calendar_info test_t;
            test_t.year = START_YEAR;
            test_t.month = 1;
            test_t.mday = 1;
            test_t.hour = 0;
            test_t.min = 0;
            test_t.sec = 0;
            uint32_t test_sec = calendar_to_sec(&test_t);
            printf("test_sec = %d\n", test_sec);
            
            static uint8_t lg_test;
            lg_test = !lg_test;
            gpio_set(LG1_PINX, 0);
            gpio_set(LG2_PINX, 0);
            gpio_set(LG3_PINX, 0);
            
            if (gpio_get(AMS_KEY_PINX))
            {
                printf("�ƻ�!!!!!!\n");
            }
            else
            {
                printf("�ֶ�!!!!!!\n");
            }
        }

        key_func();
    }
    return;
}
