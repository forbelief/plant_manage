
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
    enter_critical();
    pm_init();
    exit_critical();

    st_init(0, COMPARE, 10);    // ��Ϊ����ɨ�躯���Ķ�ʱʹ�ã���simulat_timer.c�ĵײ���ʹ��
    st_init(1, COMPARE, 100);

#ifdef PM_DEBUG
    st_init(2, COMPARE, 1);
    st_init(3, COMPARE, 500);
#endif /* PM_DEBUG */

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

#ifdef PM_DEBUG
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
#endif /* PM_DEBUG */

        key_func();
    }
    return;
}
