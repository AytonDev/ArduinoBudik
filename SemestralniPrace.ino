
#define BUZZER 9


void setup() {
  // put your setup code here, to run once:
  Serial.begin(96000);
   pinMode(BUZZER, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
 beep();
}

/**BEEP FUNCTION */

void beep() {
  tone(BUZZER, 3500);
  delay(1000);
  noTone(BUZZER);
  delay(1000);
}

