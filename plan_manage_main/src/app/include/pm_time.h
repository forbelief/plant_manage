/*
 * time.h - 时间处理
 */

#include <stdint.h>


typedef struct calendar_info_
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t mday;
    uint8_t month;
    uint16_t year;
    uint8_t wday;
    uint16_t yday;
} calendar_info;


uint8_t is_leapyear(uint16_t year);

uint8_t get_month_days(uint16_t year, uint8_t month);

void ds1302_init(void);

void ds1302_set_time(calendar_info *cal);

void ds1302_read_time(calendar_info *cal);

void maintain_system_time(void);

calendar_info get_system_time(void);

uint32_t calendar_to_sec(calendar_info *cal);

calendar_info sec_to_calendar(uint32_t sec);