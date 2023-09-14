#include <Arduino.h>
#include <BleKeyboard.h>
#include "BluetoothSerial.h"
#include "hc165.h"
#include "power_manager.h"

//POWER
power_manager_t power_manager;
#define SLEEP_OVER_TIME 1000*10 //睡眠时间

//LED
#define LED 2       // led引脚

//165
#define DATA 25     // 数据
#define CLOCK 26    // 时钟
#define LATCH 27    // latch引脚
#define key_nums 16 // 按键数量

#define enter 0xB0

BleKeyboard ble;
BTN165_typeDef btn165[key_nums];

HC165_typeDef hc165 = {
    .data_pin = DATA,
    .clock_pin = CLOCK,
    .latch_pin = LATCH,
    .key_num = key_nums,
};

//data
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



// weak定义函数
void press_handler(int i)
{
  
  ble.press(key_map[i].key_value);
  reset_sleep_timer(&power_manager);
}
// weak定义函数
void release_handler(int i)
{
  ble.release(key_map[i].key_value);
  
};

void setup()
{
  // bt.begin("ESP32");
  power_manager_init(&power_manager, fullpower, SLEEP_OVER_TIME);
  ble.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  hc165_init(&hc165, btn165, key_nums);

  
  
}

void loop()
{
  power_check(&power_manager);
  hc165_scan(&hc165, key_map, btn165);
}
