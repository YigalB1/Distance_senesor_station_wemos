#include <Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Radar.h>


int servo_pin = 12; // D6

int echoPin = 13; // Echo Pin (D7)
int trigPin = 14; // Trigger Pin (D5) 
int pos = 0;    // variable to store the servo position

int start_angle = 0;
int end_angle = 180 ;
int angle_step = 20;

 
// Reset pin not used but needed for library
#define OLED_RESET -1  //4
Adafruit_SSD1306 display(OLED_RESET);

  Radar MyRadar; 
  


// *** Function headers *** 
Servo servo_base;  // create servo object to control a servo
int I2C_scanner(byte _dev); // scan I2C and return fist device found
int ultrasinic_measure();
int move_servo_and_read(int _pos);
int radar_search(int _min_pos, int _max_pos, int _step);
void display_scroll_text(char *msg);

// ** SETUP ***
void setup() {
  Serial.begin(9600);
  Wire.begin(); // for I2C
  Serial.println("Staring setup");

  MyRadar.start_angle = start_angle;
  MyRadar.end_angle   = end_angle;
  MyRadar.angle_step  = angle_step; 

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo_base.attach(servo_pin);  // attaches the servo on pin 9 
    
  delay(2000); // for debug, to allow time to open terminal
    
  byte oled_i2c_addr = 0x3c;
  int i2C_dev = I2C_scanner(oled_i2c_addr); // the 3C Hex is 60 Dec.
  Serial.print("Oled display detected address:");
  Serial.println(i2C_dev, HEX);

  if (i2C_dev==0) {
    Serial.println("ERROR: ***** no I2C devices ");
  }
  else {
    Serial.print("OK, I2C device found: ");
    Serial.println(i2C_dev, HEX);
  }
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  char to_disp[100];
  if (i2C_dev==0) {
    sprintf(to_disp,"No I2C");
  }
  else {
    sprintf(to_disp,"I2C: %x",i2C_dev);
  }
   
  
   display_scroll_text(to_disp);
 
  

  Serial.print("SSD1306_LCDHEIGHT: ");
  Serial.println(SSD1306_LCDHEIGHT);

  Serial.print("display.width() ");
   Serial.println(display.width());
   
   Serial.print("display.height() ");
   Serial.println(display.height());
  delay(4000);
 

} // of setup


void loop() {
  int start_angle = 0; // starting angle
  int end_angle = 180; // end angle
  int step = 20 ;  // strp in angles

  digitalWrite(LED_BUILTIN, HIGH); // led OFF
  int measured_dist = radar_search(start_angle , end_angle, step); // search between 0 and 180
  char to_disp[100];
  itoa(measured_dist, to_disp , 10);
 //sprintf(to_disp,measured_dist);
  display_scroll_text(to_disp);

  delay(2500);
  digitalWrite(LED_BUILTIN, LOW); // led ON
 } // of loop

// ************************************************
int move_servo_and_read(int _pos) {
  servo_base.write(_pos);
  delay(15); // waits 15ms for the servo to reach the position
  return ultrasinic_measure();  
} // of move_servo_and_read

// ************************************************
int radar_search(int _min_pos, int _max_pos, int _step) {
  // int step = 10; // amount pf steps each move of servo (out of 180)
  int min_dist = 9999;
  int min_dist_pos = 9999;

   for (pos = _min_pos; pos <= _max_pos; pos += _step) { // in degrees, 1-180
    int dist = move_servo_and_read(pos);
    if (dist < min_dist) {
      min_dist = dist;
      min_dist_pos = pos;
      } // of if
    Serial.print("Distance/POs is: ");
    Serial.print(dist);
    Serial.print(" / ");
    Serial.println(pos);
    delay(250);   
    } // of for pos loop
  
  Serial.print(" Minimal distance/Pos: ");
  Serial.print(min_dist);
  Serial.print(" / ");
  Serial.println(min_dist_pos);

  return(min_dist);

} // radar_search

// ************************************************
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
} // ultrasinic_measure

// ************************************************
int I2C_scanner(byte _dev) {
  // returns with first found device
  byte count = 0;
  int i2c_found = 0;
  Serial.print("Searching for : ");
  Serial.println(_dev);

   if (_dev != 0 ) {
     // search for a specific I2C device
     Wire.beginTransmission(_dev);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C Device: ");
      Serial.print(" (0x");
      Serial.print(_dev, HEX);
      Serial.println(")");
      i2c_found = _dev;
      return i2c_found;
      }     
   }
   // if _dev ==0 - search all I2C bus
   
  // Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C Device: ");
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      i2c_found = i;
      return i2c_found;
      count++;
      }
    else {
      //Serial.print(".. ");
      //Serial.print(i, HEX);
      //Serial.print(".. ");
      }
      delay(100);
  } // of for loop
  //Serial.print("\r\n");
  //Serial.print("Finish I2C scanner, found ");
  //Serial.print(count, HEX);
  //Serial.println(" Device(s).");
  return i2c_found;
} // of I2C_scanner

// ************************************************
void display_scroll_text(char *msg) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(BLACK, WHITE);
  display.setCursor(10, 0);
  display.println(msg);
  display.display();      // Show initial text
  delay(200);

} // of display_scroll_text