#ifndef POWER_MANAGER_H_
#define POWER_MANAGER_H_
#include <Arduino.h>
/**
 * @brief 基本ARDUINO ESP32 
*/

#define _weak_ __attribute__((weak))
#define LOG_TAG 0
#define FULL_POWER 240
#define MID_POWER 80
#define LOW_POWER 10

enum
{
  fullpower = 0,
  midpower = 1,
  lowpower = 2,
} POWER_MODE;

struct power_manager_t
{
  int mode;
  int sleep_timer;
  uint8_t sleep_flag;
  int cpu_freq;
  int sleep_time;
};


void power_manager_init(power_manager_t *power_manager, int mode,int sleep_time);
static void power_mode_switch(int mode);
void power_check(power_manager_t *power_manager);
int get_sleep_flag(power_manager_t *power_manager); //获取睡眠标志位
void reset_sleep_timer(power_manager_t *power_manager); 



#endif // POWER_MANAGER_H_