/*
 * tft.c - tft ��ʾ���Ľӿ�
 *
 * uart hmi��ʽ
 * ���ģ��ĺ�����һ��״̬����ܣ�����
 *    switch (tft_stt.pgn)
 *    {
 *    case ORIGINAL_PG:
 *        switch (kvp_menu[etn].attr)
 *        {
 *        case R_NUM:
 *            break;
 *        case RW_NUM:
 *            break;
 *        case RW_PIC:
 *            break;
 *        case SW_PAGE:
 *            break;
 *        case R_TXT:
 *            break;
 *        default:
 *            break;
 *        }
 *        break;
 *    case MENU_PG:
 *        switch (kvp_menu[etn].attr)
 *        {
 *        case R_NUM:
 *            break;
 *        case RW_NUM:
 *            break;
 *        case RW_PIC:
 *            break;
 *        case SW_PAGE:
 *            break;
 *        case R_TXT:
 *            break;
 *        default:
 *            break;
 *        }
 *        break;
 *    case OBJ_SET_PG:
 *        switch (kvp_obj_set[tft_stt.objn][etn].attr)
 *        {
 *        case R_NUM:
 *            break;
 *        case RW_NUM:
 *            break;
 *        case RW_PIC:
 *            break;
 *        case SW_PAGE:
 *            break;
 *        case R_TXT:
 *            break;
 *        default:
 *            break;
 *        }
 *        break;
 *    default:
 *        break;
 *    }
 * ������ʾ�����У����ն����������������Ҳ������ν��״̬����߼��򵥣�ϸ�ڷ���
 *
 * ���ͼ����ؼ�������
 *      tft_stt - ����״̬����ǰ��״̬��Ϣ
 *      kvp_* - ���飬ÿһ��Ԫ�ؾ���һ��ҳ���е���Ԫ�ص�������Ϣ�����ļ����������ҳ
 *      ���������Ϣ������Ԫ��˳��Ӧ�÷���ҳ���е�ʵ��˳��Ҳ������Ԫ��˳���ʾ��ҳ
 *      ��Ԫ�ص�˳����Ϣ
 *      *_lyt - ���飬���kvp_*����һ��ҳ��Ĳ��֣���ÿһ��Ԫ�ر�ʾҳ���ж�Ӧ���е�һ
 *      ��Ԫ����kvp_*�����е��±�
 *      ÿһ��*_lyt��kvp_*�����ı�ʾ��һ��ҳ�棬Ҳ����������Ϣ�Ͳ�����Ϣ
 *      ( * ����ָͨ�����ҳ���ǰ��зָ��)
 *
 * �������ϣ���������
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"

#include "include/tft.h"
#include "include/knob.h"
#include "include/key.h"
#include "include/config.h"
#include "include/plan_handle.h"
#include "include/tft_plan_internal.h"
#include "include/pm_flash.h"
#include "include/orient.h"






/*
 * tft_state - ��tft�����У���ʾ��ǰ״̬����״̬������
 * pgn��ʾҳ���ţ�etn��ln�ֱ�һ��ҳ���е�Ԫ�غš��кţ�objn��ʾ����š�
 */
typedef struct tft_state_
{
    uint8_t pgn;
    uint8_t etn;
    uint8_t ln;
    uint8_t objn;
} tft_state;

/*
 * ҳ���־
 * ORIGINAL_PG��ʾ�������棬MENU_PG��ʾ�����棬OBJ_SET_PG��ʾֲ���������ý���
 */
typedef enum page_name_
{
    ORIGINAL_PG = 0, MENU_PG, OBJ_SET_PG
} page_name;

/*
 * ҳ�����Ա�־
 * ö�ٵ�ÿ����Ա�ĺ���ͬ���ʶ��
 */
typedef enum entry_attr_
{
    R_NUM = 0, RW_NUM, RW_PIC, SW_PAGE, R_TXT, RW_TXT
} entry_attr;

/*
 * tft������ɫʱ��Ҫ��ֵ
 */
typedef enum tft_colour_
{
    TFT_BACK = 65535, TFT_RED = 63488, TFT_PURPLE = 31
} tft_colour;





// tft��ʾ����ǰ״̬��������
static tft_state tft_stt = { 0, 0, 0, 0 };

/*
 * original_lyt����洢���ǿ��������ÿһ�е�һ����ЧԪ�ص�Ԫ�ر��
 * menu_lyt����洢�����������ÿһ�е�һ����ЧԪ�ص�Ԫ�ر��
 * obj_set_lyt����洢����ֲ���������ý����ÿһ�е�һ����ЧԪ�ص�Ԫ�ر��
 */
static uint8_t original_lyt[] = { 0, 1 };
static uint8_t menu_lyt[] = { 0, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
static uint8_t obj_set_lyt[] = { 0, 2, 5, 9, 13, 16, 18, 19};


/*
 * kvp_original������ÿһ��Ԫ�ذ�˳�����ζ�Ӧ�ſ��������е�ÿһ����ЧԪ�ء�
 * kvp_menu�����Ӧ�����档
 * kvp_obj_set��Ӧ�˸�ֲ�����Խ��棬����������а˸���ͬ��ʽ�����飬��Ӧ�˸���
 * ͬ��ֲ�
 *
 * ��Ҫ˵���ĵط���������һ�������е�ÿһ��Ԫ�ض�������ģ��Ǻ���ʾҳ���е���Ч
 * Ԫ��˳���Ӧ�ģ����������е�ÿһ��Ԫ�ذ�������Ϣ��һ���ַ�����һ������ֵ��
 * һ��ö��ֵ���ַ�����Ӧ��ҳ��Ԫ�ص�ID������ֵ�ڲ�ͬ��������в�ͬ�ĺ��壬ö��
 * ֵ��ʾ��Ԫ�ص����͡�
 */
static kv_pair kvp_original[] = { {"ori_0", 0, R_NUM} };
static kv_pair kvp_menu[] = {
    { "st_y", 2016, RW_NUM }, { "st_mo", 4, RW_NUM }, { "st_d", 14, RW_NUM }, { "st_h", 0, RW_NUM }, { "st_mi", 0, RW_NUM }, { "st_s", 0, RW_NUM },
    { "obj0", 0, SW_PAGE },
    { "obj1", 1, SW_PAGE },
    { "obj2", 2, SW_PAGE },
    { "obj3", 3, SW_PAGE },
    { "obj4", 4, SW_PAGE },
    { "obj5", 5, SW_PAGE },
    { "obj6", 6, SW_PAGE },
    { "obj7", 7, SW_PAGE },
    { "note", 0, R_TXT }
};
kv_pair kvp_obj_set[][PLAN_DATA_NUM] =
{
    {
        { "ob", 1, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    },  // 0
    {
        { "ob", 2, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    }, // 1
    {
        { "ob", 3, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    }, // 2
    {
        { "ob", 4, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    }, // 3
    {
        { "ob", 5, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    }, // 4
    {
        { "ob", 6, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    }, // 5
    {
        { "ob", 7, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    }, // 6
    {
        { "ob", 8, R_NUM}, { "sw", 1, RW_PIC },// 0~1
        { "bg_y", 2016, RW_NUM }, { "bg_mo", 4, RW_NUM }, { "bg_d", 14, RW_NUM }, // 2~4
        { "bg_h", 14, RW_NUM }, { "bg_mi", 0, RW_NUM }, { "ed_h", 14, RW_NUM }, { "ed_mi", 0, RW_NUM }, // 5~8
        { "lg_r", 0, RW_PIC}, { "lg_b", 0, RW_PIC }, { "lg_uvb", 0, RW_PIC }, { "water", 0, RW_PIC }, // 9~12
        { "pd_d", 3, RW_NUM }, { "pd_h", 3, RW_NUM }, { "pd_mi", 3, RW_NUM }, // 13~15
        { "x", 0, RW_TXT }, { "y", 1, RW_TXT }, // 16~17
        { "cnt", 0, RW_NUM } //18
    }, // 7
};

static char tft_cmd_str[20];

/*
 * ��̬��������
 */
static void tft_send_cmd(const char *cmd);
static void tft_set_color(uint8_t etn, tft_colour tft_col);
static void tft_input(void);
static void sw_to_obj(void);




/**
 * tft_init() - tftģ���ʼ��
 *
 * �������ڳ�ʼ���ͳ�ʼ����ʾ��Ϊ��������
 */
void tft_init(void)
{
    uart_init(TFT_UARTX, 9600);
    tft_send_cmd("page original");
    return;
}

/**
 * tft_send_cmd() - �򴮿�����������
 */
static void tft_send_cmd(const char *cmd)
{
    uart_sendStr(TFT_UARTX, (const uint8_t *)cmd);

    uart_putchar(TFT_UARTX, 0xff);
    uart_putchar(TFT_UARTX, 0xff);
    uart_putchar(TFT_UARTX, 0xff);
}

/**
 * tft_set_color() - ����tft��ʾ��ҳ����һ��Ԫ���������ɫ
 * @etn: Ҫ������ɫ�������Ӧ��Ԫ�غ�
 * @tft_col: Ҫ���õ���ɫ
 *
 * ������ɫʱ�����ȵõ���Ӧ��Ԫ��ID�����Ų���һ���Դ�ID������ɫ���Ե��ַ�����ʽ
 * �����Ȼ��ͨ�����ڽ�����ַ������͸������������Ҫ����ˢ�´�ID��������ʹ
 * ��ɫ�仯��ʾ����
 */
static void tft_set_color(uint8_t etn, tft_colour tft_col)
{
    //���ڲ�ͬ��ҳ���в�ͬ�Ĵ���
    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        break;
    case MENU_PG:
        // ��ͬһҳ���в�ͬ���Ե�Ԫ������ͬ�Ĵ���
        switch (kvp_menu[etn].attr)
        {
        case R_NUM:
        case RW_NUM:
        case R_TXT:
        case RW_TXT:
            // ���Ĵ�Ԫ�ص���ɫ����
            sprintf(tft_cmd_str, "%s.bco=%u", kvp_menu[etn].key, tft_col);
            tft_send_cmd(tft_cmd_str);
            // ˢ�´�Ԫ��
            sprintf(tft_cmd_str, "ref %s", kvp_menu[etn].key);
            tft_send_cmd(tft_cmd_str);
            break;
        case RW_PIC:
        case SW_PAGE:
            sprintf(tft_cmd_str, "%s_sg.bco=%u", kvp_menu[etn].key, tft_col);
            tft_send_cmd(tft_cmd_str);
            sprintf(tft_cmd_str, "ref %s_sg", kvp_menu[etn].key);
            tft_send_cmd(tft_cmd_str);
            break;
        default:
            break;
        }
        break;
    case OBJ_SET_PG:
        switch (kvp_obj_set[tft_stt.objn][etn].attr)
        {
        case R_NUM:
        case RW_NUM:
        case R_TXT:
        case RW_TXT:
            sprintf(tft_cmd_str, "%s.bco=%u",
                    kvp_obj_set[tft_stt.objn][etn].key, tft_col);
            tft_send_cmd(tft_cmd_str);
            sprintf(tft_cmd_str, "ref %s", kvp_obj_set[tft_stt.objn][etn].key);
            tft_send_cmd(tft_cmd_str);
            break;
        case RW_PIC:
        case SW_PAGE:
            sprintf(tft_cmd_str, "%s_sg.bco=%u",
                    kvp_obj_set[tft_stt.objn][etn].key, tft_col);
            tft_send_cmd(tft_cmd_str);
            sprintf(tft_cmd_str, "ref %s_sg",
                    kvp_obj_set[tft_stt.objn][etn].key);
            tft_send_cmd(tft_cmd_str);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return;
}

/**
 * tft_up() - ѡ��tft��ʾ����ǰҳ�浱ǰ�е���һ�еĵ�һ��Ԫ��
 *
 * ��ǰ��ѡ�е�Ԫ�ؽ�����������ɫ��ʾ����
 */
void tft_up(void)
{
    // ��¼��ǰԪ�غ�
    uint8_t etn_log = tft_stt.etn;

    // ��ͬһҳ������ͬ�Ĵ���
    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        // ��ǰ�кŴ�����ͼ�һ��ʾ��һ��
        if (tft_stt.ln > 0)
        {
            tft_stt.ln--;
        }
        // ����Ԫ�غ�Ϊ��ʱ�кŶ�Ӧ�еĵ�һ��Ԫ�ص�Ԫ�غ�
        tft_stt.etn  = original_lyt[tft_stt.ln];

        // ȡ��֮ǰ��Ԫ�ص���ɫ��ǣ�����ǵ�ǰԪ��
        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case MENU_PG:
        if (tft_stt.ln > 0)
        {
            tft_stt.ln--;
        }
        tft_stt.etn  = menu_lyt[tft_stt.ln];

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case OBJ_SET_PG:
        if (tft_stt.ln > 0)
        {
            tft_stt.ln--;
        }
        tft_stt.etn  = obj_set_lyt[tft_stt.ln];

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    default:
        break;
    }
    return;
}

/**
 * tft_down() - ѡ��tft��ʾ����ǰҳ�浱ǰ�е���һ�еĵ�һ��Ԫ��
 *
 * ��ǰ��ѡ�е�Ԫ�ؽ�����������ɫ��ʾ����
 */
void tft_down(void)
{
    uint8_t etn_log = tft_stt.etn;

    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        // �����ǰ�к�С������кŵĻ�����ǰ�кż�һ
        if (tft_stt.ln < sizeof(original_lyt) / sizeof(uint8_t) - 2)
        {
            tft_stt.ln++;
        }
        tft_stt.etn  = original_lyt[tft_stt.ln];

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case MENU_PG:
        if (tft_stt.ln < sizeof(menu_lyt) / sizeof(uint8_t) - 2)
        {
            tft_stt.ln++;
        }
        tft_stt.etn  = menu_lyt[tft_stt.ln];

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case OBJ_SET_PG:
        if (tft_stt.ln < sizeof(obj_set_lyt) / sizeof(uint8_t) - 2)
        {
            tft_stt.ln++;
        }
        tft_stt.etn  = obj_set_lyt[tft_stt.ln];

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    default:
        break;
    }
    return;
}

/**
 * tft_left() - ѡ��tft��ʾ����ǰҳ���е�ǰԪ�ص���һ��Ԫ��
 *
 * ��ǰ��ѡ�е�Ԫ�ؽ�����������ɫ��ʾ����
 */
void tft_left(void)
{
    uint8_t etn_log = tft_stt.etn;

    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        if (tft_stt.etn > 0)
        {
            if (--tft_stt.etn < original_lyt[tft_stt.ln])
            {
                tft_stt.ln--;
            }
        }

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case MENU_PG:
        if (tft_stt.etn > 0)
        {
            if (--tft_stt.etn < menu_lyt[tft_stt.ln])
            {
                tft_stt.ln--;
            }
        }

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case OBJ_SET_PG:
        if (tft_stt.etn > 0)
        {
            if (--tft_stt.etn < obj_set_lyt[tft_stt.ln])
            {
                tft_stt.ln--;
            }
        }

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    default:
        break;
    }
    return;
}

/**
 * tft_left() - ѡ��tft��ʾ����ǰҳ���е�ǰԪ�ص���һ��Ԫ��
 *
 * ��ǰ��ѡ�е�Ԫ�ؽ�����������ɫ��ʾ����
 */
void tft_right(void)
{
    uint8_t etn_log = tft_stt.etn;

    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        if (tft_stt.etn < original_lyt[sizeof(original_lyt)
                / sizeof(uint8_t) - 1] - 1)
        {
            if (++tft_stt.etn >= original_lyt[tft_stt.ln + 1])
            {
                tft_stt.ln++;
            }
        }

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case MENU_PG:
        if (tft_stt.etn < menu_lyt[sizeof(menu_lyt) / sizeof(uint8_t) - 1] - 1)
        {
            if (++tft_stt.etn >= menu_lyt[tft_stt.ln + 1])
            {
                tft_stt.ln++;
            }
        }

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case OBJ_SET_PG:
        if (tft_stt.etn < obj_set_lyt[sizeof(obj_set_lyt)
                / sizeof(uint8_t) - 1] - 1)
        {
            if (++tft_stt.etn >= obj_set_lyt[tft_stt.ln + 1])
            {
                tft_stt.ln++;
            }
        }

        tft_set_color(etn_log, TFT_BACK);
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    default:
        break;
    }
    return;
}



/**
 * tft_ret() - ������������һ��ҳ��
 */
void tft_ret(void)
{
    // ���ڲ�ͬ��ҳ������ͬ�Ĵ���
    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        break;
    case MENU_PG:
        /*
         * ����״̬
         * ������ʾ��һ��ҳ������
         * ���ô�ʱҳ��ѡ�б��Ϊ��ǰҳ���еĵ�һ��Ԫ��
         */
        tft_stt.pgn = ORIGINAL_PG;
        tft_stt.ln = 0;
        tft_stt.etn = 0;
        tft_send_cmd("page original");
        tft_page_refresh();
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    case OBJ_SET_PG:
        tft_stt.pgn = MENU_PG;
        tft_stt.ln = 0;
        tft_stt.etn = 0;
        tft_send_cmd("page menu");
        tft_page_refresh();

        sw_to_obj();
        tft_to_plan_input(tft_stt.objn);
        enter_critical();
        flash_write((uint8_t *)plan_in, sizeof(plan_in));
        exit_critical();
        tft_set_color(tft_stt.etn, TFT_PURPLE);
        break;
    default:
        break;
    }
    return;
}

/**
 * tft_ok() - ������������һ��ҳ����߱༭״̬
 */
void tft_ok(void)
{
    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        switch (SW_PAGE)
        {
        case R_NUM:
            break;
        case RW_NUM:
            break;
        case RW_PIC:
            break;
        case SW_PAGE:
            tft_stt.pgn = MENU_PG;
            tft_stt.ln = 0;
            tft_stt.etn = 0;
            tft_send_cmd("page menu");
            tft_page_refresh();
            sw_to_obj();
            tft_set_color(tft_stt.etn, TFT_PURPLE);
            break;
        default:
            break;
        }
        break;
    case MENU_PG:
        switch (kvp_menu[tft_stt.etn].attr)
        {
        case R_NUM:
            break;
        // ����RW_NUM����Ԫ�صı༭
        case RW_NUM:
            tft_input();
            break;
        // ����RW_TXT����Ԫ�صı༭
        case RW_TXT:
            tft_input();
            break;
        // ����RW_PIC����Ԫ�صı༭
        case RW_PIC:
            tft_input();
            break;
        // ������һ��ҳ��
        case SW_PAGE:
            tft_stt.pgn = OBJ_SET_PG;
            tft_stt.objn = kvp_menu[tft_stt.etn].value;
            tft_stt.ln = 0;
            tft_stt.etn = 0;
            tft_send_cmd("page obj_set");
            tft_page_refresh();
            tft_set_color(tft_stt.etn, TFT_PURPLE);
            break;
        default:
            break;
        }
        break;
    case OBJ_SET_PG:
        switch (kvp_obj_set[tft_stt.objn][tft_stt.etn].attr)
        {
        case R_NUM:
            break;
        case RW_TXT:
            tft_input();
            break;
        case RW_NUM:
            tft_input();
            break;
        case RW_PIC:
            tft_input();
            break;
        case SW_PAGE:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return;
}

/**
 * tft_input() - ʵ�ִ������ı༭���빦��
 *
 * ��ѡ��һ��Ԫ�ر༭ʱ��Ӧ�ı�����ɫ�Ա�ǽ���༭״̬
 * �༭����ʱ����ť����Ͱ��������������
 */
static void tft_input(void)
{
    input_limit in_lmt;
    int16_t in_v, bg_v;

    // ��ͬҳ��
    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        break;
    case MENU_PG:
        // ��ͬ����
        switch (kvp_menu[tft_stt.etn].attr)
        {
        case R_NUM:
            break;
        // ��ť����
        case RW_NUM:
            // ������ɫ
            tft_set_color(tft_stt.etn, TFT_RED);

            // �õ���ǰ�༭��Ԫ���������䷶Χ
            in_lmt = tft_input_limit(kvp_menu[tft_stt.etn].key);
            // ��¼��ǰԪ�صĳ�ʼֵ
            bg_v = kvp_menu[tft_stt.etn].value;
            knob_enable();
            // ��û�а�RET������ʱ��һֱ�����ڱ༭״̬
            while (get_key_mean(RET_KEY) == N_KEY)
            {
                in_v = bg_v + get_knob_val();
                if (in_v < in_lmt.min)
                {
                    kvp_menu[tft_stt.etn].value = in_lmt.min;
                    bg_v = in_lmt.min;
                    knob_clear();
                }
                else if (in_v > in_lmt.max)
                {
                    kvp_menu[tft_stt.etn].value = in_lmt.max;
                    bg_v = in_lmt.max;
                    knob_clear();
                }
                else
                {
                    kvp_menu[tft_stt.etn].value = in_v;
                }
                sprintf(tft_cmd_str, "%s.val=%u", kvp_menu[tft_stt.etn].key,
                        kvp_menu[tft_stt.etn].value);
                tft_send_cmd(tft_cmd_str);
            }
            knob_disable();
            tft_set_color(tft_stt.etn, TFT_PURPLE);
            // ����Ƕ�ϵͳʱ�����õĻ���Ҫͬʱ����ϵͳʱ��
            calendar_info cal;
            cal.year = *get_value_of_kvp("st_y", 0);
            cal.month = *get_value_of_kvp("st_mo", 0);
            cal.mday = *get_value_of_kvp("st_d", 0);
            cal.hour = *get_value_of_kvp("st_h", 0);
            cal.min = *get_value_of_kvp("st_mi", 0);
            cal.sec= *get_value_of_kvp("st_s", 0);
            ds1302_set_time(&cal);
            clear_key_m();
            break;
        // ������������
        case RW_PIC:
            tft_set_color(tft_stt.etn, TFT_RED);
            while (get_key_mean(RET_KEY) == N_KEY)
            {
                if (get_key_mean(UP_KEY) == S_KEY
                        || get_key_mean(DOWN_KEY) == S_KEY)
                {
                    kvp_menu[tft_stt.etn].value = !kvp_menu[tft_stt.etn].value;
                    sprintf(tft_cmd_str, "vis %s,%u", kvp_menu[tft_stt.etn].key,
                            kvp_menu[tft_stt.etn].value);
                    tft_send_cmd(tft_cmd_str);
                }
            }
            tft_set_color(tft_stt.etn, TFT_PURPLE);
            clear_key_m();
            break;
        case SW_PAGE:
            break;
        default:
            break;
        }
        break;
    case OBJ_SET_PG:
        switch (kvp_obj_set[tft_stt.objn][tft_stt.etn].attr)
        {
        case R_NUM:
            break;
        case RW_NUM:
            tft_set_color(tft_stt.etn, TFT_RED);
            in_lmt =
                tft_input_limit(kvp_obj_set[tft_stt.objn][tft_stt.etn].key);
            bg_v = kvp_obj_set[tft_stt.objn][tft_stt.etn].value;
            knob_enable();
            while (get_key_mean(RET_KEY) == N_KEY)
            {
                in_v = bg_v + get_knob_val();
                if (in_v < in_lmt.min)
                {
                    kvp_obj_set[tft_stt.objn][tft_stt.etn].value = in_lmt.min;
                    bg_v = in_lmt.min;
                    knob_clear();
                }
                else if (in_v > in_lmt.max)
                {
                    kvp_obj_set[tft_stt.objn][tft_stt.etn].value = in_lmt.max;
                    bg_v = in_lmt.max;
                    knob_clear();
                }
                else
                {
                    kvp_obj_set[tft_stt.objn][tft_stt.etn].value = in_v;
                }
                sprintf(tft_cmd_str, "%s.val=%u",
                        kvp_obj_set[tft_stt.objn][tft_stt.etn].key,
                        kvp_obj_set[tft_stt.objn][tft_stt.etn].value);
                tft_send_cmd(tft_cmd_str);
            }
            knob_disable();
            tft_set_color(tft_stt.etn, TFT_PURPLE);
            clear_key_m();
            break;
        case RW_PIC:
            tft_set_color(tft_stt.etn, TFT_RED);

            while (get_key_mean(RET_KEY) == N_KEY)
            {
                if (get_key_mean(UP_KEY) == S_KEY || get_key_mean(DOWN_KEY))
                {
                    kvp_obj_set[tft_stt.objn][tft_stt.etn].value =
                        !kvp_obj_set[tft_stt.objn][tft_stt.etn].value;
                    sprintf(tft_cmd_str, "vis %s,%u",
                            kvp_obj_set[tft_stt.objn][tft_stt.etn].key,
                            kvp_obj_set[tft_stt.objn][tft_stt.etn].value);
                    tft_send_cmd(tft_cmd_str);
                }
            }
            tft_set_color(tft_stt.etn, TFT_PURPLE);
            clear_key_m();
            break;
        // ���÷���
        case RW_TXT:
            tft_set_color(tft_stt.etn, TFT_RED);
            while (get_key_mean(RET_KEY) == N_KEY)
            {
                if (get_key_mean(UP_KEY) == S_KEY)
                {
                    orient_setspeed(
                            kvp_obj_set[tft_stt.objn][tft_stt.etn].value,
                            ORIENT_LEFT, 100);
                    sprintf(tft_cmd_str, "%s.txt=\"%s\"",
                            kvp_obj_set[tft_stt.objn][tft_stt.etn].key, "��ת");
                    tft_send_cmd(tft_cmd_str);
                }
                else if (get_key_mean(DOWN_KEY))
                {
                    orient_setspeed(
                            kvp_obj_set[tft_stt.objn][tft_stt.etn].value,
                            ORIENT_RIGHT, 100);
                    sprintf(tft_cmd_str, "%s.txt=\"%s\"",
                            kvp_obj_set[tft_stt.objn][tft_stt.etn].key, "��ת");
                    tft_send_cmd(tft_cmd_str);
                }
                else
                {
                    // ������һ��ѭ��
                }
            }
            orient_setmode(kvp_obj_set[tft_stt.objn][tft_stt.etn].value,
                    MODE_MANUL);
            orient_presetop(kvp_obj_set[tft_stt.objn][tft_stt.etn].value,
                    PRESET_SET, tft_stt.objn + 1);
            tft_set_color(tft_stt.etn, TFT_PURPLE);
            clear_key_m();
            break;
        case SW_PAGE:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}


/*
 * tft_page_refresh() - ��ҳ����Ϣ�иı�ʱ������ˢ��ҳ��
 *
 * ʵ�ֵ���Ҫ�����Ǵ������пؼ���ˢ������
 */
void tft_page_refresh(void)
{
    switch (tft_stt.pgn)
    {
    case ORIGINAL_PG:
        for (uint8_t etn = 0; etn < sizeof(kvp_original) / sizeof(kv_pair);
                etn++)
        {
            switch (SW_PAGE)
            {
            case R_NUM:
                break;
            case RW_NUM:
                break;
            case RW_PIC:
                break;
            case SW_PAGE:
                break;
            default:
                break;
            }
        }
        break;
    case MENU_PG:
        for (int etn = 0; etn < sizeof(kvp_menu) / sizeof(kv_pair);
                etn++)
        {
            switch (kvp_menu[etn].attr)
            {
            case R_NUM:
            case RW_NUM:
                sprintf(tft_cmd_str, "%s.val=%u", kvp_menu[etn].key,
                        kvp_menu[etn].value);
                tft_send_cmd(tft_cmd_str);
                sprintf(tft_cmd_str, "ref %s", kvp_menu[etn].key);
                tft_send_cmd(tft_cmd_str);
                break;
            case RW_PIC:
                sprintf(tft_cmd_str, "vis %s,%u", kvp_menu[etn].key,
                        kvp_menu[etn].value);
                tft_send_cmd(tft_cmd_str);
                break;
            case SW_PAGE:
                break;
            default:
                break;
            }
        }
        break;
    case OBJ_SET_PG:
        for (int etn = 0; etn < sizeof(kvp_obj_set[0]) / sizeof(kv_pair);
                etn++)
        {
            switch (kvp_obj_set[tft_stt.objn][etn].attr)
            {
            case R_NUM:
            case RW_NUM:
                sprintf(tft_cmd_str, "%s.val=%u",
                        kvp_obj_set[tft_stt.objn][etn].key,
                        kvp_obj_set[tft_stt.objn][etn].value);
                tft_send_cmd(tft_cmd_str);
                sprintf(tft_cmd_str, "ref %s",
                        kvp_obj_set[tft_stt.objn][etn].key);
                tft_send_cmd(tft_cmd_str);
                break;
            case RW_PIC:
                sprintf(tft_cmd_str, "vis %s,%u",
                        kvp_obj_set[tft_stt.objn][etn].key,
                        kvp_obj_set[tft_stt.objn][etn].value);
                tft_send_cmd(tft_cmd_str);
                break;
            case SW_PAGE:
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }
    return;
}

/**
 * sw_to_obj() - ��ֲ���������ý����е��Ƿ����ƻ���һ���Զ�Ӧ��������������
 *
 * �ڴ�ֲ���������ý��淵��������ʱ����
 */
static void sw_to_obj(void)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        sprintf(tft_cmd_str, "vis obj%u,%u", i,
                        *get_value_of_kvp("sw", i));
        tft_send_cmd(tft_cmd_str);
    }
    return;
}




/**
 * get_value_of_kvp() - �õ�һ��ҳ��Ԫ�ص�ID��Ӧ��vlaue�ĵ�ַ
 */
int16_t *get_value_of_kvp(char *name, uint8_t objn)
{
    // ����������
    for (int i = 0; i < sizeof(kvp_menu) / sizeof(kv_pair); i++)
    {
        if (strcmp(name, kvp_menu[i].key) == 0)
        {
            return &kvp_menu[i].value;
        }
    }
    // ����ֲ���������ý���
    for (int i = 0; i < sizeof(kvp_obj_set[objn]) / sizeof(kv_pair); i++)
    {
        if (strcmp(name, kvp_obj_set[objn][i].key) == 0)
        {
            return &kvp_obj_set[objn][i].value;
        }
    }
    return 0;
}

/**
 * tft_input_limit() - ȷ��һ��Ԫ�����������
 * @name: Ԫ��ID
 */
input_limit tft_input_limit(char *name)
{
    input_limit in_lmt;
    if (strcmp(name, "st_y") == 0)
    {
        in_lmt.min = 2000;
        in_lmt.max = 2100;
    }
    else if (strcmp(name, "st_mo") == 0)
    {
        in_lmt.min = 1;
        in_lmt.max = 12;
    }
    else if (strcmp(name, "st_d") == 0)
    {
        in_lmt.min = 1;
        in_lmt.max = get_month_days(*get_value_of_kvp("st_y", 0),
                *get_value_of_kvp("st_mo", 0));
    }
    else if (strcmp(name, "st_h") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 23;
    }
    else if (strcmp(name, "st_mi") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 59;
    }
    else if (strcmp(name, "st_s") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 59;
    }
    else if (strcmp(name, "bg_y") == 0)
    {
        in_lmt.min = 2000;
        in_lmt.max = 2100;
    }
    else if (strcmp(name, "bg_mo") == 0)
    {
        in_lmt.min = 1;
        in_lmt.max = 12;
    }
    else if (strcmp(name, "bg_d") == 0)
    {
        in_lmt.min = 1;
        in_lmt.max = get_month_days(*get_value_of_kvp("bg_y", 0),
                *get_value_of_kvp("bg_mo", 0));
    }
    else if (strcmp(name, "bg_h") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 23;
    }
    else if (strcmp(name, "bg_mi") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 59;
    }
    else if (strcmp(name, "ed_h") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 23;
    }
    else if (strcmp(name, "ed_mi") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 59;
    }
    else if (strcmp(name, "pd_d") == 0)
    {
        in_lmt.min = 1;
        in_lmt.max = 30;
    }
    else if (strcmp(name, "pd_h") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 23;
    }
    else if (strcmp(name, "pd_mi") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 59;
    }
    else if (strcmp(name, "x") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 359;
    }
    else if (strcmp(name, "y") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 359;
    }
    else if (strcmp(name, "cnt") == 0)
    {
        in_lmt.min = 0;
        in_lmt.max = 1000;
    }
    else
    {
        in_lmt.min = 0;
        in_lmt.max = 0;
    }
    return in_lmt;
}











