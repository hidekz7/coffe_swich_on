#include <Sleep.h>
#include <Watchdog.h>
#include <Servo.h> 
#include <Wire.h>
#include "RTClib_2.h"
void Send(byte addr, byte reg, byte data); // declare fonction to send msg to I2C

// オブジェクト作成
WatchdogClass WD = WatchdogClass();
RTC_DS1307 RTC; //DS1307 64 x 8, Serial, I2C Real-Time Clock

Servo myservo;
int pos = 90;    // 水平
int power = 135; // 電源
int wipe = 40;   // 開始

int start_hour = 2;   // 開始
int start_minute = 18;   // 開始

void setup()
{
    Serial.begin(57600);
    myservo.attach(4);
    myservo.write(pos);

#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
    RTC.begin();

  if (! RTC.isrunning()) {
    RTC.begin();
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__)); // the compiler's time 
    //RTC.adjust(DateTime("Jan 07 2010", "00:00:00")); // Format : Mon Day Year, 00:00:00
    //RTC.adjust(DateTime(1325376000)); // UINX Time
    //RTC.adjust(DateTime(2000,12,01,00,00,00)); // Format : Year, Month, Day, Hour, Min, Sec
  } else {
    Serial.println("RTC is Timekeep!");
  }
  WD.systemResetEnable(false);          // スリープ復帰時にスケッチをリセットしない(setup関数を実行しない)ように設定
  WD.enable(WatchdogClass::TimeOut8s);  // スリープ間隔を8秒に設定
}

void loop() {                          // スリープで待機
    DateTime now = RTC.now();
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(' ');
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();
//    delay(1000);
    if  ((now.hour() == start_hour) && (now.minute() == start_minute)) {
      clean1();
//    delay(1000);
    }
//    delay(100000); テスト用ウェイト
    WatchdogClass::timerReset();  // Watchdogタイマーリセット
    SleepClass::powerDown();      // パワーダウン
}
void clean1() {
    interrupts();
    
    myservo.write(power); //電源ボタンを押す
    Serial.print("power on");
    delay(1000);
    myservo.write(pos);
    delay(1000);
    myservo.write(wipe); //開始ボタンを押す
    Serial.print("start");
    delay(1000);
    myservo.write(pos);
        delay(100000);
  }

