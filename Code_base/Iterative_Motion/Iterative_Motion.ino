#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define SHIN_VOLTAGE 25
#define SHIN_CW 19
#define SHIN_CCW 23

#define THIGH_VOLTAGE 26
#define THIGH_CW 18
#define THIGH_CCW 4

unsigned long cycle_start = 0;
const unsigned long gait_time = 22000;

// Percentage of the gait cycle for various limb positions
const float toe_off_time_thigh = 0.25;
//const float full_contraction_thigh = 0.55;
const float mid_swing_thigh = 0.7;
const float full_extension_thigh = 0.75;

const float toe_off_time_shin = 0.245;
//const float full_contraction_shin = 0.43;
const float mid_swing_shin = 0.49;
const float full_extension_shin = 0.745;
const float neutral = 1;

// PWM channel parameters
const int freq = 5000;
const int shinChannel = 0;
const int thighChannel = 1;
const int debugChannel = 2;
const int resolution = 10;

bool safe = 1;

// Pre-defined way points
double thigh_points[4] = { 10, -8, -10, 0};
double shin_points[4] = { 165, 168, 175, 170};
int thigh_iteration = 0;
int shin_iteration = 0;

Adafruit_BNO055 bno_thigh = Adafruit_BNO055(55, 0x28, &Wire);
Adafruit_BNO055 bno_shin = Adafruit_BNO055(55, 0x29, &Wire);

double Kp_thigh = 2, thigh_controller_output, thigh_angle, thigh_error;
double Kp_shin = 2, shin_controller_output, shin_angle, shin_error;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("Setup initialized.");

  pinMode(SHIN_CW, OUTPUT);
  pinMode(SHIN_CCW, OUTPUT);

  pinMode(THIGH_CW, OUTPUT);
  pinMode(THIGH_CCW, OUTPUT);

  ledcSetup(thighChannel, freq, resolution);
  ledcSetup(shinChannel, freq, resolution);

  ledcAttachPin(THIGH_VOLTAGE, thighChannel);
  // ledcAttachPin(2, thighChannel);
  ledcAttachPin(SHIN_VOLTAGE, shinChannel);

  delay(10);

  Serial.println("Configured output and PWM pins.");

  if (!bno_thigh.begin()) {
    Serial.print("Problem with thigh BNO, rectify.");
    while (1)
      ;
  }

  if (!bno_shin.begin()) {
    Serial.print("Problem with shin BNO, rectify.");
    while (1)
      ;
  }

  delay(10);

  int setup_gap = 3;
  while (setup_gap > 0) {
    Serial.print("Setup complete. Executing loop in ");
    Serial.print(setup_gap);
    Serial.println(" seconds");
    delay(1000);
    setup_gap = setup_gap - 1;
  }

  Serial.println("Starting Gait.");

  unsigned long cycle_start = millis();
}

void loop() {

  controller();

  delay(20);
}


void update_values() {

  unsigned long current_time = millis();
  if (safe) {
    if (current_time - cycle_start < (long)(toe_off_time_thigh * gait_time)) {
      thigh_iteration = 0;
      if (current_time - cycle_start < (long)(toe_off_time_shin * gait_time)) {
        shin_iteration = 0;
      } else if (current_time - cycle_start < (long)(mid_swing_shin * gait_time)) {
        shin_iteration = 1;
      }
    } else if (current_time - cycle_start < (long)(mid_swing_thigh * gait_time)) {
      thigh_iteration = 1;
      if (current_time - cycle_start < (long)(mid_swing_shin * gait_time)) {
        shin_iteration = 1;
      } else if (current_time - cycle_start < (long)(full_extension_shin * gait_time)) {
        shin_iteration = 2;
      }
    } else if (current_time - cycle_start < (long)(full_extension_thigh * gait_time)) {
      thigh_iteration = 2;
      if (current_time - cycle_start < (long)(full_extension_shin * gait_time)) {
        shin_iteration = 2;
      } else if (current_time - cycle_start < (long)(neutral * gait_time)) {
        shin_iteration = 3;
      }
    } else if (current_time - cycle_start < (long)(neutral * gait_time)) {
      thigh_iteration = 3;
      shin_iteration = 3;
    } else if (current_time - cycle_start >= (long)(neutral * gait_time)) {
      cycle_start = current_time;
    }
  } else {
    thigh_iteration = 3;
    shin_iteration = 3;
  }
}

void feedback() {
  update_values();

  sensors_event_t theta_thigh, theta_shin;

  bno_thigh.getEvent(&theta_thigh, Adafruit_BNO055::VECTOR_GRAVITY);
  bno_shin.getEvent(&theta_shin, Adafruit_BNO055::VECTOR_GRAVITY);

  thigh_angle = get_angle(&theta_thigh);
  Serial.println("thigh_angle: ");
  Serial.println(thigh_angle);
  shin_angle = 180 + get_angle(&theta_thigh) - get_angle(&theta_shin);
  Serial.println("shin_angle: ");
  Serial.println(shin_angle);

  if (abs(thigh_angle) >= 30) {
    safe = 0;
  }
  if (!safe) {
    thigh_iteration = 3;
    shin_iteration = 3;
    if ((abs(thigh_error) <= 2) && (abs(shin_error) <= 2)) {
      Serial.println("Reached safe state, restarting gait cycle.");
      cycle_start = millis();
      safe = 1;
    }
  }

  thigh_error = thigh_points[thigh_iteration] - thigh_angle;
  shin_error = shin_points[shin_iteration] - shin_angle;
}

double get_angle(sensors_event_t* event) {
  double temp_angle, x, y, z;
  event->type == SENSOR_TYPE_GRAVITY;
  x = event->acceleration.x;
  y = event->acceleration.y;
  z = event->acceleration.z;
  if ((x < 0) && (y < 0)) {
    temp_angle = (atan(x / y) * 57.2957795131);
  } else if ((x > 0) && (y < 0)) {
    temp_angle = (atan(x / y) * 57.2957795131);
  } else if ((x > 0) && (y > 0)) {
    temp_angle = (atan(x / y) * 57.2957795131);
  } else {
    temp_angle = (atan(x / y) * 57.2957795131);
  }
  if (temp_angle > 180) {
    temp_angle = temp_angle - 270;
  }
  return temp_angle;
}


void controller() {
  feedback();

  if (thigh_error >= 0) {
    digitalWrite(THIGH_CW, HIGH);
    digitalWrite(THIGH_CCW, LOW);
  } else {
    digitalWrite(THIGH_CW, LOW);
    digitalWrite(THIGH_CCW, HIGH);
  }

  if (shin_error >= 0) {
    digitalWrite(SHIN_CW, LOW);
    digitalWrite(SHIN_CCW, HIGH);
  } else {
    digitalWrite(SHIN_CW, HIGH);
    digitalWrite(SHIN_CCW, LOW);
  }

  if (safe) {
    shin_controller_output = abs(Kp_shin * shin_error);
    thigh_controller_output = abs(Kp_thigh * thigh_error);
  }

  else {
    shin_controller_output = 10;
    thigh_controller_output = 10;
  }

  actuation();
}

void actuation() {
  int shin_duty, thigh_duty;
  if (abs(shin_error) > 3) {
    shin_duty = int(map(shin_controller_output, 0, 240, 102, 922));
  } else if (abs(shin_error) <= 3) {
    shin_duty = 102;
  }

  if (abs(thigh_error) > 3) {
    thigh_duty = int(map(thigh_controller_output, 0, 240, 102, 922));
  } else if (abs(thigh_error) <= 3) {
    thigh_duty = 102;
  }
  ledcWrite(shinChannel, shin_duty);
  ledcWrite(thighChannel, thigh_duty);
}
