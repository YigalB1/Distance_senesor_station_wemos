#include <Servo.h>
//#include <Arduino.h>

int servo_pin = 12; // D6

;
Servo servo_base;  // create servo object to control a servo


int pos = 0;    // variable to store the servo position

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  servo_base.attach(servo_pin);  // attaches the servo on pin 9 
}

void loop() {
   digitalWrite(LED_BUILTIN, HIGH);
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo_base.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
   digitalWrite(LED_BUILTIN, LOW);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo_base.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }



}

