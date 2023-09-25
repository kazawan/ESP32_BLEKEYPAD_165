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
    // power_mode_switch(power_manager->mode);
    #if LOG_TAG
    Serial.println("power_manager_init");
    Serial.print(power_manager->cpu_freq);
    Serial.println("MHz");
    #endif
};


/**
 * @brief 电池初始化
*/
void battery_init(lipo_t *lipo,int en_pin,int check_pin)
{
    
    lipo->check_en = en_pin;
    pinMode(lipo->check_en, OUTPUT);
    CHECK_EN_DISABLE(lipo->check_en);
    lipo->voltage = get_battery_voltage(lipo);
    lipo->percent = get_battery_percent(lipo);
    lipo->is_low_power = is_battery_low(lipo);
    lipo->power_check_pin = check_pin;
    lipo->MaxVoltage = 4.7;
    lipo->MinVoltage = 3.3;
    
    //todo add check

};


/**
 * @brief 电池检测
*/
double get_battery_voltage(lipo_t *lipo)
{
    CHECK_EN_ENABLE(lipo->check_en);
    delay(1);
    double voltage = analogRead(lipo->power_check_pin) * 2 * 3.3 / 4095;
    CHECK_EN_DISABLE(lipo->check_en);
    return voltage;
};

/**
 * @brief 返回电池百分比
 *
*/
uint8_t get_battery_percent(lipo_t *lipo)
{
    lipo->voltage = get_battery_voltage(lipo);
    lipo->percent = (lipo->voltage - lipo->MinVoltage) / (lipo->MaxVoltage - lipo->MinVoltage) * 100;
    if (lipo->percent > 100)
    {
        lipo->percent = 100;
    }
    else if (lipo->percent < 0)
    {
        lipo->percent = 0;
    }
    return lipo->percent;
};

/**
 * @@brief 电池电量低
 * 
*/
bool is_battery_low(lipo_t *lipo)
{
    if (get_battery_percent(lipo) < 10)
    {
        return true;
    }
    else
    {
        return false;
    }
};


_weak_ void battery_check_loop_handle(){
    // 电池检测循环 回调函数
};

/**
 * @brief 电池检测循环
*/
void check_battery_loop(lipo_t *lipo,int time_loop)
{
    if (millis() - lipo->check_timer > time_loop)
    {
        lipo->check_timer = millis();
        lipo->is_low_power = is_battery_low(lipo);
        battery_check_loop_handle();
        if(lipo->is_low_power){
            #if LOG_TAG
            Serial.println("low power");
            #endif
            power_mode_switch(lowpower);
        }
    }

};


/**
 * @brief 电源模式切换
 * ! 电源模式切换时会调用相应的回调函数
*/
_weak_ void full_power_task_handle(){
    //   Serial.println("full_power_task_handle");
};

_weak_ void mid_power_task_handle(){
    //   Serial.println("mid_power_task_handle");
};

_weak_ void low_power_task_handle(){
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