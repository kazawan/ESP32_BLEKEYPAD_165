#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>
#define _weak_ __attribute__((weak)) // 弱引用


#define btn_sample_rate 300 // 按键扫描频率


struct BTN_t
{
  byte pin;//管脚
  int keyup;//按键状态
  int current_time;//时间
  int long_time_release;
  unsigned int debouce_time;//消抖时间
  byte pin_read; //管脚状态  上拉 默认 1  下拉 默认0
   
};

void btn_init(BTN_t *btn,int pin,int external_pullup = -1);
void btn_scan(BTN_t *p);


#endif