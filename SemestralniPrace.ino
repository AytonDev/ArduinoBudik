
/*LIBRARIES*/

/*--I2C*/
#include <Wire.h>
/*--RTCmodule*/
#include "RTClib.h"
/*--DISPLAY*/
#include <TM1637.h>
#include <TimerOne.h>




/* PINS DEFINITION*/
/*--BUZZER*/

#define BUZZER 9
/*--DISPLAY*/
#define CLK_DISPLAY 2
#define DIO_DISPLAY 3



struct T_Time {
  unsigned int minutes;
  unsigned int hours;
};

struct T_Alarm {
  unsigned int minutes;
  unsigned int hours;
  bool active;
};

T_Time m_Time;

RTC_DS1307 RTC;


int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
TM1637 tm1637(CLK_DISPLAY,DIO_DISPLAY);




void setup() {
  // put your setup code here, to run once:

  /**SETUP PINS **/
  pinMode(BUZZER, OUTPUT);
  /**SETUP SERIAL**/
  Serial.begin(9600);

  /**SETUP RTC MODULE VIA I2C BUS **/
  InitRTC();
  /**SETUP DISPLAY**/
  InitDisplay();
 

}

void loop() {

  m_Time = GetTime();

  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(CheckAlarms);//declare the interrupt serve routine:TimingISR
 
  // put your main code here, to run repeatedly:
}


void SaveAlarm() {
  
}

void LoadAlarms() {
  
}

void CheckAlarms() {
  
}

/*DISPLAY----------------------------------------------*/

void InitDisplay(){

  tm1637.set();
  tm1637.init();
  
}


void PrintDisplay() {
  
  TimeDisp[0] = m_Time.hours / 10;
  TimeDisp[1] = m_Time.hours % 10;
  TimeDisp[2] = m_Time.minutes / 10;
  TimeDisp[3] = m_Time.minutes % 10;
  tm1637.display(TimeDisp);
  
}

/*-----------------------------------------------------*/


/*RTC MODULE-------------------------------------------*/
void InitRTC() {
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

/**GET TIME from RTC module
 * returns T_Time struct of actual time
**/
T_Time GetTime() {
  DateTime now = RTC.now(); 
  return T_Time{ now.minute(),now.hour()};
}
/*-----------------------------------------------------*/


/*BEEEP------------------------------------------------*/
/**BEEP FUNCTION */
void Beep() {
  tone(BUZZER, 3500);
  delay(1000);
  noTone(BUZZER);
  delay(1000);
}

