#include <Servo.h>
//#include <Arduino.h>

int servo_pin = 12; // D6

int echoPin = 13; // Echo Pin (D7)
int trigPin = 14; // Trigger Pin (D5) 
int pos = 0;    // variable to store the servo position
 
Servo servo_base;  // create servo object to control a servo

int ultrasinic_measure();
int move_servo_and_read(int _pos);
void radar_search(int min_dist, int _max_dist);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo_base.attach(servo_pin);  // attaches the servo on pin 9 
  Serial.begin(9600);
  
} // of setup

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // led OFF
  radar_search(20, 40); // TBD - use min & max to define a range of detection
  delay(500);
  digitalWrite(LED_BUILTIN, LOW); // led ON
} // of loop


int move_servo_and_read(int _pos) {
  servo_base.write(_pos);
  delay(15); // waits 15ms for the servo to reach the position
  return ultrasinic_measure();  
}

void radar_search(int min_dist, int _max_dist) {
   for (pos = 0; pos <= 180; pos += 1) { // in degrees, 1-180
    int dist = move_servo_and_read(pos);
    Serial.print("Distance is: ");
    Serial.println(dist);
    delay(250);   
    }
  
    for (pos = 180; pos >=0 ; pos -= 1) { // in degrees, 1-180
    int dist = move_servo_and_read(pos);
    Serial.print("Distance is: ");
    Serial.println(dist);
    delay(250);   
    }


}

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
  //Serial.println(distance);
  //Delay 50ms before next reading.
  delay(50);
  return distance;
}
