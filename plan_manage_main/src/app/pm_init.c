/*
 * pm_init.c - ������Ӧ�ó���ĳ�ʼ��
 */

#include "include/simulat_timer.h"
#include "include/key.h"
#include "include/knob.h"
#include "include/debug.h"
#include "include/tft.h"
#include "include/config.h"
#include "include/time.h"


/*
 * pm_init() - ��ʼ��
 */
void pm_init(void)
{
    debug_init();
    key_init();
    knob_init();
    st_base_init();
    tft_init();
    ds1302_init();
    return;
}
