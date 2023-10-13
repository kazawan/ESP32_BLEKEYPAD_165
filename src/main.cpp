#include <Arduino.h>
#include <BleKeyboard.h>
#include "BluetoothSerial.h"
#include "hc165.h"
#include "power_manager.h"
#include "button.h"
// ttl
//  BluetoothSerial BT;
// button
#define FN 19
BTN_t fn;

// 添加电源检测
#define check_en 14
#define check_pin 35
lipo_t battey;
// #define battery_check_loop 1000 * 60 // 电池检测时间
#define battery_check_loop 1000 * 5 // 电池检测时间

// POWER
power_manager_t power_manager;
#define SLEEP_OVER_TIME 1000 * 60 * 10// 睡眠时间


// LED
#define LED 2 // led引脚
int led_timer = 0;

// 165
#define DATA 25     // 数据
#define CLOCK 26    // 时钟
#define LATCH 27    // latch引脚
#define key_nums 16 // 按键数量

#define enter 0xB0
#define ESC 0xB1

BleKeyboard ble;
BTN165_typeDef btn165[key_nums];

HC165_typeDef hc165 = {
    .data_pin = DATA,
    .clock_pin = CLOCK,
    .latch_pin = LATCH,
    .key_num = key_nums,
};

// data
KEYMAP_typeDef key_map[] =
    {
        {'4'},
        {'+'},
        {'9'},
        {'8'},
        {'7'},
        {'-'},
        {'/'},
        {'*'},
        {'.'},
        {'0'},
        {enter},
        {'3'},
        {'2'},
        {'1'},
        {'6'},
        {'5'},
};

void battery_check_loop_handle()
{
  ble.setBatteryLevel(battey.percent);
 
}



// weak定义函数
void press_handler(int i)
{

  ble.press(key_map[i].key_value);
  reset_sleep_timer(&power_manager);
  // reset_sleep_timer(&power_manager);
}
// weak定义函数
void release_handler(int i)
{
  ble.release(key_map[i].key_value);
};

void full_power_task_handle()
{
  if(digitalRead(LED) == 0){
    digitalWrite(LED, 1);
  }
}

void low_power_task_handle()

{
  if(digitalRead(LED) == 1){
    digitalWrite(LED, 0);
  }
}

void fn_key_handler()
{
  ble.press(KEY_ESC);
  ble.release(KEY_ESC);
};

void setup()
{
  // Serial.begin(115200);
  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_19, 0); // 1 = High, 0 = Low0
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  battery_init(&battey, check_pin );
  power_manager_init(&power_manager, fullpower, SLEEP_OVER_TIME);
  hc165_init(&hc165, btn165, key_nums);
  btn_init(&fn, FN,1);
  ble.begin();
  ble.setBatteryLevel(get_battery_percent(&battey));
}

void loop()
{
  check_battery_loop(&battey, battery_check_loop);
  power_check(&power_manager);
  hc165_scan(&hc165, key_map, btn165);
  btn_scan(&fn);


}
