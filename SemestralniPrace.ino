
/*LIBRARIES*/
#include <EEPROM.h>
/*--I2C*/
#include <Wire.h>
/*--RTCmodule*/
#include "RTClib.h"
/*--DISPLAY*/
#include <TM1637.h>
#include <TimerOne.h>

///ROTATE ENCODER

#define BUTTON  3; //button
#define ROTATE1 = 6;
#define ROTATE2 = 5;

//rotation
int counter = 0; 
int aState;
int aLastState;

/* PINS DEFINITION*/
/*--BUZZER*/

#define BUZZER 9
/*--DISPLAY*/
#define CLK_DISPLAY 2
#define DIO_DISPLAY 3


#define ALARM_COUNT 5


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


int addresses[] ={0,3,6,9,12}; 
T_Alarm alarms[5];

int a = 0;
int value;

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

 //DefaultAlarms();

 //ENCODER PINS
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(ROTATE1, INPUT_PULLUP);
  pinMode(ROTATE2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON), menu, CHANGE);
 

}

void loop() {

  m_Time = GetTime();
  PrintDisplay();
  

  

 
  // put your main code here, to run repeatedly:
}



//Clears all data from EEPROM
void ClearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    if(EEPROM.read(i) != 0)                     //skip already "empty" addresses
    {
      EEPROM.write(i, 0);                       //write 0 to address i
    }
  }
  Serial.println("EEPROM erased");
                                 //reset address counter
}


//SET FIVE DEFAULT ALARMS
void DefaultAlarms() {
  for(int i =0; i< ALARM_COUNT; ++i) {
    alarms[i].minutes = 30+i;
    alarms[i].hours = 7+i;
    alarms[i].active = true;
  }

  for(int i = 0; i <ALARM_COUNT; i++) {
    EEPROM.write(addresses[i], alarms[i].hours);
    EEPROM.write(addresses[i]+1, alarms[i].minutes);
    EEPROM.write(addresses[i]+2, alarms[i].active);
   }
}

/*
 *Save Alarm to the EEPROM
 */
void SaveAlarm(int position) {
 
  EEPROM.write(addresses[position], alarms[position].hours);
  EEPROM.write(addresses[position]+1, alarms[position].minutes);
  EEPROM.write(addresses[position]+2, alarms[position].active);
}

/**
 * Load all alarms from EEPROM
 */
void LoadAlarms() {
  
  for(int i =0; i< ALARM_COUNT; ++i) {
    alarms[i].minutes =  EEPROM.read(addresses[i]);
    alarms[i].hours = EEPROM.read(addresses[i]+1);
    alarms[i].active = EEPROM.read(addresses[i]+2);
  }
}


/* 
 *  If current time is equal with one of the active alarm -> beep()
 */
void CheckAlarms() {

  for(int i =0; i< ALARM_COUNT; ++i) {
    if( alarms[i].active && (alarms[i].hours == m_Time.hours) && (alarms[i].minutes == m_Time.minutes) {
      Beep();
    }
  }
  
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


//ENCODER
void menu() {
 Serial.print("menu open");
 //readEncoderRotate();
  
}

void readEncoderRotate() {
 // put your main code here, to run repeatedly:
  aState = digitalRead(pin2);
  if (aState != aLastState){
    if (digitalRead(pin3) != aState) { 
       counter ++;
     } else {
       counter --;
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState; /
   //if(rotateEnable){
  //   readEncoderRotate();
  // }
}

void menu() {
 Serial.print("menu open");
 //readEncoderRotate();
  
}


