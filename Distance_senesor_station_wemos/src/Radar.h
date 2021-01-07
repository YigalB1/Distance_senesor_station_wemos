#include <Adafruit_SSD1306.h>

struct Radar_packet
{
  int pos;
  int min_dist;
  };


class Radar
{
  //#define OLED_RESET -1  //4
 
  public:
    int servo_pin;    // WEMOS pin dedicated for servo
    int start_angle;  // starting angle
    int end_angle;    // end angle
    int angle_step;         // servo step in angles (from 0 to 180)
    int us_trigPin;
    int us_echoPin;
    int min_dist = 9999;
    int min_dist_pos = 9999;

    Servo servo_base;  // create servo object to control a servo
    Radar_packet radar_packet; 

  void test_servo() {
    for (int i=0;i<180;i+=10) {
      Serial.print(".. pos is: ");
      Serial.print(i);
      servo_base.write(i);
      delay(100);
    } // end of for loop 
      
  } // end of test_servo


  int move_servo_and_read(int _pos) {
    int loc_dist = 9999;
    servo_base.write(_pos);
    delay(150); // waits 15ms for the servo to reach the position
    loc_dist = ultrasinic_measure();
    //Serial.print("in debug A, dist: ");
    //Serial.println(loc_dist);
    return(loc_dist);  
  } // of move_servo_and_read

  // **** Radar Search ***
  void radar_search(int _min_pos, int _max_pos, int _step) {
    // int step = 10; // amount pf steps each move of servo (out of 180)
    int loc_min_dist = 9999;
    int loc_min_dist_pos = 9999;
    //Radar_packet loc_packet;

    Serial.println();
    Serial.print("Distance/POs is: ");
    for (int pos = _min_pos; pos <= _max_pos; pos += _step) { // in degrees, 1-180
      int dist = move_servo_and_read(pos);
      if (dist < loc_min_dist) {
        loc_min_dist = dist;
        loc_min_dist_pos = pos;
        } // of if
      
      Serial.print(dist);
      //Serial.print(" / ");
      //Serial.print(pos);
      Serial.print(" ... ");
      delay(250);   
      } // of for pos loop
    
    Serial.println("");
    Serial.print(" Minimal distance/Position: ");
    Serial.print(loc_min_dist);
    Serial.print(" / ");
    Serial.println(loc_min_dist_pos);
    min_dist = loc_min_dist;
    min_dist_pos = loc_min_dist_pos;

} // radar_search


// ************************************************
int ultrasinic_measure() {
  long duration, distance; // Duration used to calculate distance
  digitalWrite(us_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(us_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(us_trigPin, LOW);
  duration = pulseIn(us_echoPin, HIGH);
  //Serial.println("dist debug trig/echo/duration");
  //Serial.println(us_trigPin);
  //Serial.println(us_echoPin);
  //Serial.println(duration);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  //Serial.println(distance);
  //Delay 50ms before next reading.
  delay(50);
  return distance;
} // ultrasinic_measure




}; // of Radar class