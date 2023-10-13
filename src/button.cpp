#include "button.h"

void btn_init(BTN_t *btn,int pin,int external_pullup)
{
    btn->pin = pin;
    if (external_pullup != -1)
    {
      pinMode(btn->pin, INPUT_PULLUP);
    }
    else
    {
      pinMode(btn->pin, INPUT);
    }
    btn->keyup = 1;
    btn->current_time = 0;
    btn->debouce_time = 20;
    btn->long_time_release = 0;
    btn->pin_read = 1;
}


_weak_ void fn_key_handler()
{
  // Serial.println("weak");
}

void btn_scan(BTN_t* p)
{

  for (int i = 0 ; i < btn_sample_rate ; i ++ )
  {
    if (i < btn_sample_rate - 1)
    {
      p->pin_read = digitalRead(p->pin);
    }
    else if (i == btn_sample_rate - 1 )
    {
      
      if (p->pin_read == 0 && p->keyup == 1 && p->pin_read == digitalRead(p->pin))//如果keyup == 1 管脚状态处于断开状态  p == 0
      {
        p->long_time_release = millis();
        if (millis() - p->current_time > p->debouce_time) //消抖（非必要）
        {

          p->current_time = millis();
          p->keyup = 0;

          //*****你的程序在这下面*********
          // Serial.println("pressed");
          //*****************************

        }
      } else if (p->pin_read == 1 && p->keyup == 0)
      {
        if (millis() - p->long_time_release > 1000)
        {
          
          p->keyup = 1;
          // Serial.println("long release");
        } else
        {
          p->keyup = 1;
          fn_key_handler();
          // Serial.println("short release");
        }

      }
    }
  }

}