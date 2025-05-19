#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BOOT 0

#define SHIN_V 26
#define SHIN_CW 18
#define SHIN_CCW 4

#define THIGH_V 25
#define THIGH_CW 19
#define THIGH_CCW 5

Adafruit_BNO055 bno_thigh = Adafruit_BNO055(55, 0x28, &Wire);
Adafruit_BNO055 bno_shin = Adafruit_BNO055(55, 0x29, &Wire);

String inString = "";
//int state = 0;

double thigh_controller_output, thigh_current_angle, thigh_prev_error, thigh_curr_error, thigh_error_diff, thigh_set_point = 70;
double shin_controller_output, shin_current_angle, shin_prev_error, shin_curr_error, shin_error_diff, shin_set_point = 100;

double Kp_thigh = 3;
double Kd_thigh = 0;

double Kp_shin = 2;
double Kd_shin = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  pinMode(SHIN_CW, OUTPUT);
  pinMode(SHIN_CCW, OUTPUT);

  pinMode(THIGH_CW, OUTPUT);
  pinMode(THIGH_CCW, OUTPUT);

//  pinMode(BOOT, INPUT);
  
    if (!bno_thigh.begin())
  {
    Serial.print("Problem with thigh BNO, rectify.");
    while (1);
  }

    if (!bno_shin.begin())
  {
    Serial.print("Problem with shin BNO, rectify.");
    while (1);
  }

  delay(1000);
}

void loop() {  
  if (digitalRead(BOOT) == LOW) {
    if (state < 2) {
      state = state + 1;  
    }
    else {
      state = 0;
    }
  }

  switch (state) {
    case 0:
      thigh_set_point = 88;
      shin_set_point = 92;
      break;
      
    case 1:
      thigh_set_point = 50;
      shin_set_point = 110;
      break;
      
    case 2:
      thigh_set_point = 110;
      shin_set_point = 135;
      break;
  
  }

  feedback();
  
  controller();

  actuation();

  delay(50);
}


void feedback() {
  sensors_event_t theta_thigh, theta_shin;
  
  bno_thigh.getEvent(&theta_thigh, Adafruit_BNO055::VECTOR_GRAVITY);
  bno_shin.getEvent(&theta_shin, Adafruit_BNO055::VECTOR_GRAVITY);
  
  thigh_current_angle = get_angle(&theta_thigh);
  Serial.print("Thigh: ");
  Serial.print(thigh_current_angle);
  shin_current_angle = get_angle(&theta_shin);
  Serial.print(", Knee: ");
  Serial.println(shin_current_angle);
}


double get_angle(sensors_event_t* event) {
  double temp_angle, x, y, z;
  event->type == SENSOR_TYPE_GRAVITY;
  x = event->acceleration.x;
  y = event->acceleration.y;
  z = event->acceleration.z;
    
  if (abs(z) > 3) {
    Serial.println("Please hold the mount vertical.");
  }
  else {
    
    if ((x < 0) && (y < 0)) {
      temp_angle = (atan(y/x) * 57.2957795131);
    }
    else if ((x > 0) && (y < 0)) {
      temp_angle = 180 - (atan(-y/x) * 57.2957795131);
    }
    else if ((x > 0) && (y > 0)) {
      temp_angle = (atan(y/x) * 57.2957795131);
    }
    else {
      temp_angle = 180 - (atan(-y/x) * 57.2957795131);
    }
 }
   
     return temp_angle;
}


void controller() {
  thigh_curr_error = thigh_set_point - thigh_current_angle;
  thigh_error_diff = thigh_prev_error - thigh_curr_error;

  shin_curr_error = shin_set_point - shin_current_angle;
  shin_error_diff = shin_prev_error - shin_curr_error;

  if (thigh_curr_error <= 0) {
    digitalWrite(THIGH_CW, HIGH);
    digitalWrite(THIGH_CCW, LOW);
  }
  else {
    digitalWrite(THIGH_CW, LOW);
    digitalWrite(THIGH_CCW, HIGH);
  }
  
  if (shin_curr_error >= 0) {
    digitalWrite(SHIN_CW, LOW);
    digitalWrite(SHIN_CCW, HIGH);
  }
  else {
    digitalWrite(SHIN_CW, HIGH);
    digitalWrite(SHIN_CCW, LOW);
  }
  
  thigh_controller_output = abs((Kp_thigh*thigh_curr_error) + (Kd_thigh*thigh_error_diff));
  shin_controller_output = abs((Kp_shin*shin_curr_error) + (Kd_shin*shin_error_diff));
}


void actuation() {
  int thigh_voltage = int(map(thigh_controller_output, 0, 600, 0, 255));
  int shin_voltage = int(map(shin_controller_output, 0, 720, 0, 255));

  dacWrite(SHIN_V, shin_voltage);
  dacWrite(THIGH_V, thigh_voltage);
  
  shin_prev_error = shin_curr_error;
  thigh_prev_error = thigh_curr_error;
}
