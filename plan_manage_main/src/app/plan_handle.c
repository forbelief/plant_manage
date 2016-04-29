/*
 * plan_handle.c - �ƻ�����ģ��
 *
 * �ƻ�����ָ���Ǹ����ڴ��еļƻ����ݺ�ʱ�����ݵó�������ݣ�������ݰ����̵�����
 * �š�ֲ������ֵ��
 * ����ֲ������ֵ����������ɼƻ����ڴ������ȡ�
 * ����ԭ���ǣ���ʼ����ʱ����������ظ����������Եõ�һ�����伯�ϣ��жϵ�ǰʱ���Ƿ�
 * �������ʱ�����伯���е�һ�������е�һ��ʱ��㡣�ɴ˵õ��̵����źţ����ڼ̵�����
 * �ű���Ϊ��Ч��ʱ�򽫼ƻ����ڴ�����һ��
 * ��Ҫ����������У���η��ʼƻ�������ʱ�����ݣ��Լ����д��������ݡ�
 * �ƻ�������tft.cģ�飬���ھ�̬���ݣ����Ҫ���ʵĻ��������ַ�ʽ��һ�������ƻ�����
 * �����Ͳ������������ַ��һ�������ַ���Է��ʵ����еĶ������ԣ�������Ҫ֪�������
 * ַ�����ݵķֲ��������Ҫ��������ݡ����Ǹ���ÿ��Ԫ�ص�����ͨ����ѯ�õ��䵥����ֵ��
 */

#include <stdint.h>

#include "gpio.h"

#include "include/tft_plan_internal.h"

#include "include/plan_handle.h"
#include "include/tft.h"
#include "include/config.h"
#include "include/pm_flash.h"
#include "include/orient.h"
#include "include/key.h"

typedef struct plan_output_
{
    uint8_t is_reach;
    uint8_t cnt;
} plan_output;


static void indata_to_outdata(plan_input *ind, plan_output *outd);
static void plan_ctr_exe(uint8_t activity);
static void plan_inpu_to_tft(void);
static void plan_key_func(void);
static void manul_key_func(void);
static void manul_ctr(void);
static void plan_ctr(void);

static plan_output plan_out[PLAN_DATA_NUM] = { 0 };
plan_input plan_in[PLAN_DATA_NUM] = { 0 };



void plan_handle_init(void)
{
    gpio_Interrupt_init(LGRED_PINX, GPO, GPI_DISAB);
    gpio_Interrupt_init(LGBLUE_PINX, GPO, GPI_DISAB);
    gpio_Interrupt_init(LGUVB_PINX, GPO, GPI_DISAB);
    gpio_Interrupt_init(WATER_PINX, GPO, GPI_DISAB);

    gpio_Interrupt_init(LG1_PINX, GPO, GPI_DISAB);
    gpio_Interrupt_init(LG2_PINX, GPO, GPI_DISAB);
    gpio_Interrupt_init(LG3_PINX, GPO, GPI_DISAB);
    // �������÷���ĳ�ʼ��

    /*
     * ���ݻָ������ݴӴ�flash��plan_handleģ�飬�ٴ�plan_handleģ�鵽tftģ��
     */
    flash_read((uint8_t *)&plan_in, sizeof(plan_in));
    plan_inpu_to_tft();

    return;
}



static void indata_to_outdata(plan_input *ind, plan_output *outd)
{
    calendar_info st = get_system_time();
    uint32_t sys_sec = calendar_to_sec(&st);

    ind->pd_t.year = START_YEAR;
    ind->pd_t.month = 1;
    ind->pd_t.mday = 1;
    uint32_t pd_sec = calendar_to_sec(&ind->pd_t);

    uint32_t ctr_bg_sec = calendar_to_sec(&ind->bg_t) + pd_sec * outd->cnt;
    uint32_t ctr_ed_sec = calendar_to_sec(&ind->ed_t) + pd_sec * outd->cnt;

    if (ctr_bg_sec < sys_sec && ctr_ed_sec > sys_sec)
    {
        outd->is_reach = 1;
    }
    else
    {
        if (outd->is_reach == 1)
        {
            outd->cnt++;
        }
        outd->is_reach = 0;
    }

    return;
}

static void plan_ctr_exe(uint8_t activity)
{
    if (activity >= PLAN_DATA_NUM)
    {
        gpio_set(LGRED_PINX, 0);
        gpio_set(LGBLUE_PINX, 0);
        gpio_set(LGUVB_PINX, 0);
        gpio_set(WATER_PINX, 0);
    }
    else
    {
        orient_presetop(0, PRESET_CALL, activity + 1);
        orient_presetop(1, PRESET_CALL, activity + 1);
        gpio_set(LGRED_PINX, plan_in[activity].lg_r);
        gpio_set(LGBLUE_PINX, plan_in[activity].lg_b);
        gpio_set(LGUVB_PINX, plan_in[activity].lg_uvb);
        gpio_set(WATER_PINX, plan_in[activity].water);
    }
    return;
}


void plan_ctr(void)
{
    uint8_t activity = PLAN_DATA_NUM;

    for (uint8_t i = 0; i < PLAN_DATA_NUM; i++)
    {
        indata_to_outdata(&plan_in[i], &plan_out[i]);
        if (plan_in[i].sw == 1)
        {
            if (plan_out[i].is_reach == 1)
            {
                activity = i;
                break;
            }
        }
    }
    plan_ctr_exe(activity);
    return;
}


void plan_handle(void)
{
    if (gpio_get(AMS_KEY_PINX))
    {
        plan_ctr();
    }
    else
    {
        manul_ctr();
    }
    return;
}

void key_func(void)
{
    if (gpio_get(AMS_KEY_PINX))
    {
        plan_key_func();
    }
    else
    {
        manul_key_func();
    }
    return;
}


void manul_ctr(void)
{
    return;
}

/*
 * tft_to_plan_input() - ��tft��ʾ��������ȡ���ƻ�������������ݽṹ��
 * @objn: ��ȡ�Ķ���ţ�0~7
 *
 * �������Ӧ����tft�����иı��Ǳ����á�
 */
void tft_to_plan_input(uint8_t objn)
{
    plan_in[objn].bg_t.year = *get_value_of_kvp("bg_y", objn);
    plan_in[objn].bg_t.month = *get_value_of_kvp("bg_mo", objn);
    plan_in[objn].bg_t.mday = *get_value_of_kvp("bg_d", objn);
    plan_in[objn].bg_t.hour = *get_value_of_kvp("bg_h", objn);
    plan_in[objn].bg_t.min = *get_value_of_kvp("bg_mi", objn);

    /*
     * ����ʱ����tft��Ŀû���ꡢ�¡��գ���ֵΪ�ο�ʼʱ����ͬ
     */
    plan_in[objn].ed_t.year = plan_in[objn].bg_t.year;
    plan_in[objn].ed_t.month = plan_in[objn].bg_t.month;
    plan_in[objn].ed_t.mday = plan_in[objn].bg_t.mday;
    plan_in[objn].ed_t.hour = *get_value_of_kvp("ed_h", objn);
    plan_in[objn].ed_t.min = *get_value_of_kvp("ed_mi", objn);

    plan_in[objn].pd_t.mday = *get_value_of_kvp("pd_d", objn);
    plan_in[objn].pd_t.hour = *get_value_of_kvp("pd_h", objn);
    plan_in[objn].pd_t.min = *get_value_of_kvp("pd_mi", objn);

    plan_in[objn].lg_r = *get_value_of_kvp("lg_r", objn);
    plan_in[objn].lg_b = *get_value_of_kvp("lg_b)", objn);
    plan_in[objn].lg_uvb = *get_value_of_kvp("lg_uvb", objn);
    plan_in[objn].water = *get_value_of_kvp("water", objn);

    plan_in[objn].sw = *get_value_of_kvp("sw", objn);

    return;
}


/*
 * plan_inpu_to_tft() - �ƻ����ݵ�tft
 *
 * ���������Ϊ���ڳ�ʼ��ʱ����flash�лָ��ļƻ���������װ��tft
 */
static void plan_inpu_to_tft(void)
{
    for(uint8_t objn = 0; objn < 8; objn++)
    {
        *get_value_of_kvp("bg_y", objn) = plan_in[objn].bg_t.year;
        *get_value_of_kvp("bg_mo", objn) = plan_in[objn].bg_t.month;
        *get_value_of_kvp("bg_d", objn) = plan_in[objn].bg_t.mday;
        *get_value_of_kvp("bg_h", objn) = plan_in[objn].bg_t.hour;
        *get_value_of_kvp("bg_mi", objn) = plan_in[objn].bg_t.min;

        /*
         * ����ʱ����tft��Ŀû���ꡢ�¡���
         * *get_value_of_kvp("ed_y", objn) = plan_in[objn].ed_t.year;
         * *get_value_of_kvp("ed_mo", objn) = plan_in[objn].ed_t.month;
         * *get_value_of_kvp("ed_d", objn) = plan_in[objn].ed_t.mday;
         */
        *get_value_of_kvp("ed_h", objn) = plan_in[objn].ed_t.hour;
        *get_value_of_kvp("ed_mi", objn) = plan_in[objn].ed_t.min;

        *get_value_of_kvp("pd_d", objn) = plan_in[objn].pd_t.mday;
        *get_value_of_kvp("pd_h", objn) = plan_in[objn].pd_t.hour;
        *get_value_of_kvp("pd_mi", objn) = plan_in[objn].pd_t.min;

        *get_value_of_kvp("lg_r", objn) = plan_in[objn].lg_r;
        *get_value_of_kvp("lg_b", objn) = plan_in[objn].lg_b;
        *get_value_of_kvp("lg_uvb", objn) = plan_in[objn].lg_uvb;
        *get_value_of_kvp("water", objn) = plan_in[objn].water;

        *get_value_of_kvp("sw", objn) = plan_in[objn].sw;
    }
    return;
}


static void plan_key_func(void)
{
    /*
     * ��������ע�Ჿ��
     */
    switch (get_key_mean(UP_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("UP_KEY ����\n");
        tft_left();
        break;
    case D_KEY:
        printf("UP_KEY ˫��\n");
        tft_up();
        break;
    case L_KEY:
        printf("UP_KEY ����\n");
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
        printf("DOWN_KEY ����\n");
        tft_right();
        break;
    case D_KEY:
        printf("DOWN_KEY ˫��\n");
        tft_down();
        break;
    case L_KEY:
        printf("DOWN_KEY ����\n");
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
        printf("OK_KEY ����\n");
        tft_ok();
        break;
    case D_KEY:
        printf("OK_KEY ˫��\n");
        break;
    case L_KEY:
        printf("OK_KEY ����\n");
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
        printf("RET_KEY ����\n");
        tft_ret();
        break;
    case D_KEY:
        printf("RET_KEY ˫��\n");
        break;
    case L_KEY:
        printf("RET_KEY ����\n");
        break;
    default:
        break;
    }
    switch (get_key_mean(MR_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("MR_KEY ����\n");
        break;
    case D_KEY:
        printf("MR_KEY  ˫��\n");
        break;
    case L_KEY:
        printf("MR_KEY ����\n");
        break;
    default:
        break;
    }
    switch (get_key_mean(MB_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("MB_KEY ����\n");
        break;
    case D_KEY:
        printf("MB_KEY ˫��\n");
        break;
    case L_KEY:
        printf("MB_KEY ����\n");
        break;
    default:
        break;
    }
    switch (get_key_mean(MUVB_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("MUVB_KEY ����\n");
        break;
    case D_KEY:
        printf("MUVB_KEY ˫��\n");
        break;
    case L_KEY:
        printf("MUVB_KEY ����\n");
        break;
    default:
        break;
    }
    return;
}


static void manul_key_func(void)
{
    /*
     * ��������ע�Ჿ��
     */
    static uint8_t mlgr, mlgb, mlguvb;

    switch (get_key_mean(UP_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("UP_KEY ����\n");
        orient_setspeed(0, ORIENT_LEFT, 100);
        break;
    case D_KEY:
        printf("UP_KEY ˫��\n");
        orient_setspeed(0, ORIENT_RIGHT, 100);
        break;
    case L_KEY:
        printf("UP_KEY ����\n");
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
        printf("DOWN_KEY ����\n");
        orient_setspeed(1, ORIENT_LEFT, 100);
        break;
    case D_KEY:
        printf("DOWN_KEY ˫��\n");
        orient_setspeed(1, ORIENT_RIGHT, 100);
        break;
    case L_KEY:
        printf("DOWN_KEY ����\n");
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
        printf("OK_KEY ����\n");
        break;
    case D_KEY:
        printf("OK_KEY ˫��\n");
        break;
    case L_KEY:
        printf("OK_KEY ����\n");
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
        printf("RET_KEY ����\n");
        orient_setmode(0, MODE_MANUL);
        orient_setmode(1, MODE_MANUL);
        break;
    case D_KEY:
        printf("RET_KEY ˫��\n");
        break;
    case L_KEY:
        printf("RET_KEY ����\n");
        break;
    default:
        break;
    }
    switch (get_key_mean(MR_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("MR_KEY ����\n");
        mlgr = !mlgr;
        gpio_set(LGRED_PINX, mlgr);
        break;
    case D_KEY:
        printf("MR_KEY  ˫��\n");
        break;
    case L_KEY:
        printf("MR_KEY ����\n");
        break;
    default:
        break;
    }
    switch (get_key_mean(MB_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("MB_KEY ����\n");
        mlgb = !mlgb;
        gpio_set(LGBLUE_PINX, mlgb);
        break;
    case D_KEY:
        printf("MB_KEY ˫��\n");
        break;
    case L_KEY:
        printf("MB_KEY ����\n");
        break;
    default:
        break;
    }
    switch (get_key_mean(MUVB_KEY))
    {
    case N_KEY:
        //printf("�޼�\n");
        break;
    case S_KEY:
        printf("MUVB_KEY ����\n");
        mlguvb = !mlguvb;
        gpio_set(LGUVB_PINX, mlguvb);
        break;
    case D_KEY:
        printf("MUVB_KEY ˫��\n");
        break;
    case L_KEY:
        printf("MUVB_KEY ����\n");
        break;
    default:
        break;
    }
    return;
}
