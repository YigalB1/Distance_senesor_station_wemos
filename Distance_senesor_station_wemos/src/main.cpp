#include <Servo.h>
//#include <Arduino.h>

int servo_pin = 12; // D6

int echoPin = 7; // Echo Pin
int trigPin = 6; // Trigger Pin
int pos = 0;    // variable to store the servo position
 
Servo servo_base;  // create servo object to control a servo

int ultrasinic_measure();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  servo_base.attach(servo_pin);  // attaches the servo on pin 9 
  Serial.begin(9600);
  
} // of setup

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
  int dist = ultrasinic_measure();
  Serial.print("Distance is: ");
  Serial.println(dist);

} // of loop

int ultrasinic_measure() {
  long duration, distance; // Duration used to calculate distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  Serial.println(distance);
  //Delay 50ms before next reading.
  delay(50);
  return distance;
}

