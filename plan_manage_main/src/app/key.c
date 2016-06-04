/*
 * key.c - ��Ⲣ�ɼ�����������
 *
 * �����������룬�޼���������˫����������
 * ÿ10ms��Ҫ�ɼ�һ��io״̬��
 * ÿ����һ����������Ҫ���õ���:
 *
 *      key_typeö�������ж�Ӧ��ö��Ԫ��
 *
 *      ��Ӧ��key_infomation����
 *
 *      key_init()�����ж�Ӧ�ĳ�ʼ����Ŀ
 *      key_detect()�����ж�Ӧ�����Ŀ
 *      get_key_mean()�����ж�Ӧ��ȡ��Ŀ
 */


#include <stdint.h>

#include "include.h"

#include "include/key.h"
#include "include/config.h"


/*
 * key_state - ��ʾ������״̬
 * key_state_0   ��ʼ״̬�����߱�ʾ��һ�ε���״̬
 * key_state_1   ������ȷ��״̬�����ʾ�ڶ��ε���״̬
 * key_state_2   ������ʱ״̬
 * key_state_3   �����ȴ��ͷ�״̬
 */
typedef enum key_state_
{
    key_state_0 = 0, key_state_1, key_state_2, key_state_3
} key_state;

/*
 * IO״̬���Ŷ���
 */
typedef enum state_sign_
{
    INVALID = 0, VALID = 1
} state_sign;


/*
 * key_infomation - ��ʾһ����������Ϣ
 * state_base��time_base��key_read_base������ʹ�õ�״̬����
 * state��time��key_read������ʹ�õ�״̬����
 * key_m���浱ǰ������ȡ�Ľ��
 * get_state_io����ָ���ȡ������Ӧ����io״̬�ĺ���
 */
typedef struct key_infomation_
{
    key_state state_base;
    uint8_t time_base;

    key_state state;
    uint8_t time;

    key_mean key_m;

    state_sign (*get_state_io)(void);
} key_infomation;



/*
 * �±��⼸������������ǻ�ȡ����io״̬�ĺ���
 */
state_sign get_state_up_key_io(void)
{ return !gpio_get(UP_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_down_key_io(void)
{ return !gpio_get(DOWN_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_ok_key_io(void)
{ return !gpio_get(OK_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_ret_key_io(void)
{ return !gpio_get(RET_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_ams_key_io(void)
{ return !gpio_get(AMS_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_wifi_key_io(void)
{ return !gpio_get(WIFI_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_mr_key_io(void)
{ return !gpio_get(MR_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_mb_key_io(void)
{ return !gpio_get(MB_KEY_PINX) ? VALID : INVALID; }

state_sign get_state_muvb_key_io(void)
{ return !gpio_get(MUVB_KEY_PINX) ? VALID : INVALID; }


// ���尴����������ʼ��
static volatile key_infomation
up_key_info =   { key_state_0, 0, key_state_0, 0, N_KEY, get_state_up_key_io },
down_key_info = { key_state_0, 0, key_state_0, 0, N_KEY, get_state_down_key_io },
ok_key_info =   { key_state_0, 0, key_state_0, 0, N_KEY, get_state_ok_key_io },
ret_key_info =  { key_state_0, 0, key_state_0, 0, N_KEY, get_state_ret_key_io },
ams_key_info =  { key_state_0, 0, key_state_0, 0, N_KEY, get_state_ams_key_io },
wifi_key_info = { key_state_0, 0, key_state_0, 0, N_KEY, get_state_wifi_key_io },
mr_key_info =   { key_state_0, 0, key_state_0, 0, N_KEY, get_state_mr_key_io },
mb_key_info =   { key_state_0, 0, key_state_0, 0, N_KEY, get_state_mb_key_io },
muvb_key_info = { key_state_0, 0, key_state_0, 0, N_KEY, get_state_muvb_key_io };



/**
 * key_init() - ����ģ���ʼ��
 *
 */
void key_init(void)
{
    gpio_Interrupt_init(UP_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(DOWN_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(OK_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(RET_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(AMS_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(WIFI_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(MR_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(MB_KEY_PINX, GPI_UP_PF, GPI_DISAB);
    gpio_Interrupt_init(MUVB_KEY_PINX, GPI_UP_PF, GPI_DISAB);

    return;
}


/**
 * key_read_base - ����������ȡ������
 * @key_info: Ҫ��ⰴ����������Ϣ,��������ȫ�ֱ������д洢��
 *
 * ����������� N_KEY,S_KEY,L_KEY�������
 */
key_mean key_read_base(volatile key_infomation *key_info)
{
    state_sign key_state_io;
    key_mean key_m = N_KEY;

    // ��ȡio״̬
    key_state_io = (*(key_info->get_state_io))();
    switch(key_info->state_base)
    {
    case key_state_0:
        if (key_state_io == VALID)
        {
            // �������º����Ƚ��������Ͱ���ȷ��״̬
            key_info->state_base = key_state_1;
        }
        break;
    case key_state_1:
        if (key_state_io == VALID)
        {
            // ���������Ȼ���£����������
            // ����״̬ת��,���ҷ����ް����¼�
            key_info->time_base = 0;
            key_info->state_base = key_state_2;
        }
        else
        {
            key_info->state_base = key_state_0;
        }
        break;
    case key_state_2:
        if (key_state_io == INVALID)
        {
            // ��ʱ�����ͷ�˵��Ϊһ�ζ̲���
            key_m = S_KEY;
            key_info->state_base = key_state_0;
        }
        // �������£���ʱ��10ms������������������
        else if (++key_info->time_base > 100)
        {
            // ����ʱ�����1s�����س�����������ת��Ϊ�ȴ������ͷ�״̬
            key_m = L_KEY;
            key_info->state_base = key_state_3;
        }
        break;
    case key_state_3:
        if (key_state_io == INVALID)
        {
            // ������ʱ���ͷţ�ת��Ϊ��ʼ״̬
            key_info->state_base = key_state_0;
        }
        break;
    default:
        break;
    }
    return key_m;
}

/**
 * key_read - ������ȡ����
 * @key_info: Ҫ���İ����������Ϣ����������ȫ�ֱ������д洢��
 *
 * ��������᷵��N_KEY,S_KEY,D_KEY��L_KEY�����
 */
key_mean key_read(volatile key_infomation *key_info)
{
    key_mean key_m = N_KEY,
            key_m_temp;

    key_m_temp = key_read_base(key_info);
    switch(key_info->state)
    {
    case key_state_0:
        if (key_m_temp == S_KEY)
        {
            // ��һ�ε���״̬����Ȼ�����޼������¸������ж��Ƿ����˫��
            key_info->time = 0;
            key_info->state = key_state_1;
        }
        else
        {
            // ���ڷǵ���������ԭ�¼�
            key_m = key_m_temp;
        }
        break;
    case key_state_1:
        // ��һ�ε�������� < 300ms
        if (key_m_temp == S_KEY)
        {
            key_m = D_KEY;
            key_info->state = key_state_0;
        }
        else
        {
            if (++key_info->time > 30)
            {
                // 300ms��û���ٳ��ֵ����¼����򷵻���һ�ε����¼�
                key_m = S_KEY;
                key_info->state = key_state_0;
            }
        }
        break;
    default:
        break;
    }
    return key_m;
}


/**
 * key_detect() - ��ⰴ��
 *
 * �����������N_KEY״̬ʱ��¼������Ϣ��N_KEY����Ӧ�ó����ȡ������Ϣ��ʱ��ͬʱ
 * �����õģ���������Ŀ���Ǳ�֤�ɼ����İ���һ���ᱻִ��
 * Ҫ��ÿ��10ms����һ��
 */
void key_detect(void)
{
    key_mean key_m;

    if ((key_m = key_read(&up_key_info)) != N_KEY)
    {
        up_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&down_key_info)) != N_KEY)
    {
        down_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&ok_key_info)) != N_KEY)
    {
        ok_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&ret_key_info)) != N_KEY)
    {
        ret_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&ams_key_info)) != N_KEY)
    {
        ams_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&wifi_key_info)) != N_KEY)
    {
        wifi_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&mr_key_info)) != N_KEY)
    {
        mr_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&mb_key_info)) != N_KEY)
    {
        mb_key_info.key_m = key_m;
    }
    if ((key_m = key_read(&muvb_key_info)) != N_KEY)
    {
        muvb_key_info.key_m = key_m;
    }
    return;
}


/**
 * get_key_mean() - �õ�ָ���İ�����Ϣ
 *
 * �����������ȡ����Ϣ��Ὣ��Ӧ�������±��ΪN_KEY
 */
key_mean get_key_mean(key_type key_t)
{
    key_mean key_m;

    switch (key_t)
    {
    case UP_KEY:
        key_m = up_key_info.key_m;
        up_key_info.key_m = N_KEY;
        break;
    case DOWN_KEY:
        key_m = down_key_info.key_m;
        down_key_info.key_m = N_KEY;
        break;
    case OK_KEY:
        key_m = ok_key_info.key_m;
        ok_key_info.key_m = N_KEY;
        break;
    case RET_KEY:
        key_m = ret_key_info.key_m;
        ret_key_info.key_m = N_KEY;
        break;
    case AMS_KEY:
        key_m = ams_key_info.key_m;
        ams_key_info.key_m = N_KEY;
        break;
    case WIFI_KEY:
        key_m = wifi_key_info.key_m;
        wifi_key_info.key_m = N_KEY;
        break;
    case MR_KEY:
        key_m = mr_key_info.key_m;
        mr_key_info.key_m = N_KEY;
        break;
    case MB_KEY:
        key_m = mb_key_info.key_m;
        mb_key_info.key_m = N_KEY;
        break;
    case MUVB_KEY:
        key_m = muvb_key_info.key_m;
        muvb_key_info.key_m = N_KEY;
        break;
    default:
        break;
    }
    return key_m;
}


void clear_key_m(void)
{
    down_key_info.key_m = N_KEY;
    up_key_info.key_m = N_KEY;
    ok_key_info.key_m = N_KEY;
    ret_key_info.key_m = N_KEY;
    ams_key_info.key_m = N_KEY;
    wifi_key_info.key_m = N_KEY;
    mr_key_info.key_m = N_KEY;
    mb_key_info.key_m = N_KEY;
    muvb_key_info.key_m = N_KEY;
}
