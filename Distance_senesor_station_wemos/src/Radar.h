class Radar
{
 
  public:
    int servo_pin;    // WEMOS pin dedicated for servo
    int start_angle;  // starting angle
    int end_angle;    // end angle
    int angle_step;         // strp in angles


    int min_dist;
    int min_dist_pos;
    void dot(); // TBD - change name
    
  private:
    int _pin;



};