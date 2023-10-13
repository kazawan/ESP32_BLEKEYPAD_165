#ifndef POWER_MANAGER_H_
#define POWER_MANAGER_H_
#include <Arduino.h>
/**
 * @brief 基本ARDUINO ESP32 
*/

#define _weak_ __attribute__((weak))
#define LOG_TAG 0
#define FULL_POWER 80
#define MID_POWER 80
#define LOW_POWER 40

enum
{
  fullpower = 0,
  midpower = 1,
  lowpower = 2,
} POWER_MODE;

enum
{
  check_enable = 0,
  check_disable = 1,
}CHECK_EN_TIGER;

#define CHECK_EN_ENABLE(X) digitalWrite(X, check_enable)
#define CHECK_EN_DISABLE(X) digitalWrite(X, check_disable)


struct power_manager_t
{
  int mode;
  int sleep_timer;
  uint8_t sleep_flag;
  int cpu_freq;
  int sleep_time;
};

struct lipo_t
{
  double voltage;
  float MaxVoltage;
  float MinVoltage;
  uint8_t percent;
  int check_en;//0:检测 1:不检测
  int power_check_pin;
  bool is_low_power ;
  int check_timer;
};


void power_manager_init(power_manager_t *power_manager, int mode,int sleep_time);
static void power_mode_switch(int mode);
void power_check(power_manager_t *power_manager);
int get_sleep_flag(power_manager_t *power_manager); //获取睡眠标志位
void reset_sleep_timer(power_manager_t *power_manager); 

void battery_init(lipo_t *lipo,int check_pin,int en_pin = -1);

double get_battery_voltage(lipo_t *lipo);
uint8_t get_battery_percent(lipo_t *lipo);
bool is_battery_low(lipo_t *lipo);
void check_battery_loop(lipo_t *lipo,int time_loop);


#endif // POWER_MANAGER_H_