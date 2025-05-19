#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BOOT 0
#define VOLTAGE 25
#define CW 19
#define CCW 5

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

double controller_output, current_angle, prev_error, curr_error, error_diff, set_point = 45;
double Kp = 2;
double Kd = 0;
int state = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  pinMode(CW, OUTPUT);
  pinMode(CCW, OUTPUT);
  pinMode(BOOT, INPUT);
  if (!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);
}

void loop() {
  sensors_event_t theta;
  bno.getEvent(&theta, Adafruit_BNO055::VECTOR_GRAVITY);
  get_angle(&theta);

  //  if (digitalRead(BOOT) == LOW) {
  //    if (state < 2) {
  //      state = state + 1;
  //    }
  //    else {
  //      state = 0;
  //    }
  //  }

  //  switch (state) {
  //    case 0:
  //      set_point = 45;
  //      break;
  //    case 1:
  //      set_point = 135;
  //      break;
  //    case 2:
  //      set_point = 60;
  //      break;
  //  }
  controller();

  int control_voltage = int(map(controller_output, 0, 720, 0, 255));
  //  Serial.println(control_voltage);
  dacWrite(VOLTAGE, control_voltage);

  prev_error = curr_error;
  delay(10);
}

void controller() {
  curr_error = set_point - current_angle;
  error_diff = prev_error - curr_error;

  if (curr_error >= 0) {
    digitalWrite(CW, HIGH);
    digitalWrite(CCW, LOW);
  }
  else {
    digitalWrite(CW, LOW);
    digitalWrite(CCW, HIGH);
  }

  controller_output = abs((Kp * curr_error) + (Kd * error_diff));

}

void get_angle(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  event->type == SENSOR_TYPE_GRAVITY;
  x = event->acceleration.x;
  y = event->acceleration.y;
  z = event->acceleration.z;
  if (abs(z) > 3) {
    Serial.println("Please hold the mount vertical.");
  }
  else {
    if ((x < 0) && (y < 0)) {
      current_angle = (atan(x / y) * 57.2957795131);
    }
    else if ((x < 0) && (y > 0)) {
      current_angle = 180 - (atan(-x / y) * 57.2957795131);
    }
    else if ((x > 0) && (y > 0)) {
      current_angle = 180 + (atan(x / y) * 57.2957795131);
    }
    else {
      current_angle = 360 - (atan(-x / y) * 57.2957795131);
    }
  }
}
