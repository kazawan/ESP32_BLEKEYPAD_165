#include "power_manager.h"


/**
 * @brief 初始化
 * @param power_manager 结构体
 * @param mode 电源模式
 * @param sleep_time 睡眠时间
*/
void power_manager_init(power_manager_t *power_manager, int mode, int sleep_time)
{
    power_manager->mode = mode;
    power_manager->sleep_timer = 0;
    power_manager->sleep_flag = 0;
    power_manager->sleep_time = sleep_time;
    power_manager->cpu_freq = getCpuFrequencyMhz();
    #if LOG_TAG
    Serial.println("power_manager_init");
    Serial.print(power_manager->cpu_freq);
    Serial.println("MHz");
    #endif
};


/**
 * @brief 电源模式切换
 * ! 电源模式切换时会调用相应的回调函数
*/
_weak_ void full_power_task_handle(void){
    //   Serial.println("full_power_task_handle");
};

_weak_ void mid_power_task_handle(void){
    //   Serial.println("mid_power_task_handle");
};

_weak_ void low_power_task_handle(void){
    //   Serial.println("low_power_task_handle");
};


/**
 * @brief 电源模式切换
 * @param mode 电源模式
 * ! 电源模式切换时会调用相应的回调函数
*/
static void power_mode_switch(int mode)
{
    int cpu_freq = getCpuFrequencyMhz();
    switch (mode)
    {
    case fullpower:
        if (cpu_freq != FULL_POWER)
        {
            setCpuFrequencyMhz(FULL_POWER);
            full_power_task_handle();
            #if LOG_TAG
            Serial.println("fullpower");
            Serial.print(getCpuFrequencyMhz());
            Serial.println("MHz");
            #endif
        }

        break;
    case midpower:
        if (cpu_freq != MID_POWER)
        {
            setCpuFrequencyMhz(MID_POWER);
            mid_power_task_handle();
        }
        break;
    case lowpower:
        if (cpu_freq != LOW_POWER)
        {
            setCpuFrequencyMhz(LOW_POWER);
            low_power_task_handle();
            #if LOG_TAG
            Serial.println("lowpower");
            Serial.print(getCpuFrequencyMhz());
            Serial.println("MHz");
            #endif
        }
        break;
    default:
        break;
    }
};



/**
 * @brief 电源检测
 * @param power_manager 结构体
*/
void power_check(power_manager_t *power_manager)
{

    if (millis() - power_manager->sleep_timer > power_manager->sleep_time)
    {
        power_manager->sleep_flag = 1;
    }
    else
    {
        power_manager->sleep_flag = 0;
    };

    if (power_manager->sleep_flag == 0) // fullpower
    {
        power_mode_switch(fullpower);
    }
    else
    {
        power_mode_switch(lowpower);
    }
};


/**
 * @brief 获取睡眠标志位
 * @param power_manager 结构体
 * @return 睡眠标志位
*/
int get_sleep_flag(power_manager_t *power_manager)
{
    return power_manager->sleep_flag;
};


/**
 * @brief 重置睡眠计时器
 * @param power_manager 结构体
*/
void reset_sleep_timer(power_manager_t *power_manager)
{
    power_manager->sleep_timer = millis();
};