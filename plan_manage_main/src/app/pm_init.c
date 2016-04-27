/*
 * pm_init.c - ������Ӧ�ó���ĳ�ʼ��
 */

#include "include/simulat_timer.h"
#include "include/key.h"
#include "include/knob.h"
#include "include/debug.h"
#include "include/tft.h"
#include "include/config.h"
#include "include/pm_time.h"
#include "include/plan_handle.h"
#include "include/pm_flash.h"
#include "include/orient.h"


/*
 * pm_init() - ��ʼ��
 */
void pm_init(void)
{
    pm_flash_init();
    debug_init();
    key_init();
    knob_init();
    st_base_init();
    tft_init();
    ds1302_init();
    plan_handle_init();     // �ƻ������ʼ��������flash��ʼ��֮��
    orient_init();
    return;
}
