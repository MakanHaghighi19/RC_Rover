#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX

struct PacketData {
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
};

PacketData data;

// Assign ports for right motor
int enb = 7;
int rmotor_pin1 = 10;
int rmotor_pin2 = 9;

// Assign ports for left motor
int ena = 8;
int lmotor_pin1 = 12;
int lmotor_pin2 = 11;

// Assign ports for Ultrasonic Sensor
int trig_pin = 4;
int echo_pin = 5;

unsigned long lastRecvTime = 0;

long DistanceMeasurement() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);
  long distance = (duration / 2) / 29.1; // Convert duration to distance
  return distance; // Return the calculated distance
}

void setup() {
  pinMode(enb, OUTPUT);
  pinMode(rmotor_pin1, OUTPUT);
  pinMode(rmotor_pin2, OUTPUT);

  pinMode(ena, OUTPUT);
  pinMode(lmotor_pin1, OUTPUT);
  pinMode(lmotor_pin2, OUTPUT);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT); // Set echo pin as INPUT

  Serial.begin(9600);
  btSerial.begin(38400);
}

void loop() {
  String dataString;
  long distance = DistanceMeasurement();

  if (distance < 20) { // Threshold distance for crash detection (20 cm)
    analogWrite(enb, 0);
    analogWrite(ena, 0);
  } else {
    if (btSerial.available()) {
      dataString = btSerial.readStringUntil('\n');
      sscanf(dataString.c_str(), "%d,%d,%d,%d", &data.lxAxisValue, &data.lyAxisValue, &data.rxAxisValue, &data.ryAxisValue);

      int throttle = map(data.lyAxisValue, 254, 0, -255, 255); // Left stick - y axis - forward/backward car movement
      int steering = map(data.rxAxisValue, 0, 254, -255, 255); // Right stick - x axis - left/right car movement

      int rightMotorSpeed = 0, leftMotorSpeed = 0;

      if (throttle >= 0) {
        rightMotorSpeed = throttle - steering;
        leftMotorSpeed = throttle + steering;
      } else {
        rightMotorSpeed = throttle + steering;
        leftMotorSpeed = throttle - steering;
      }

      rightMotorSpeed = constrain(rightMotorSpeed, -250, 250);
      leftMotorSpeed = constrain(leftMotorSpeed, -250, 250);

      int rightMotorDirection = (rightMotorSpeed >= 0) ? 1 : -1;
      int leftMotorDirection = (leftMotorSpeed >= 0) ? 1 : -1;

      // Control right motor
      if (rightMotorSpeed > 0) {
        digitalWrite(rmotor_pin1, HIGH);
        digitalWrite(rmotor_pin2, LOW);
      } else if (rightMotorSpeed < 0) {
        digitalWrite(rmotor_pin1, LOW);
        digitalWrite(rmotor_pin2, HIGH);
      } else {
        digitalWrite(rmotor_pin1, LOW);
        digitalWrite(rmotor_pin2, LOW);
      }

      // Control left motor
      if (leftMotorSpeed > 0) {
        digitalWrite(lmotor_pin1, HIGH);
        digitalWrite(lmotor_pin2, LOW);
      } else if (leftMotorSpeed < 0) {
        digitalWrite(lmotor_pin1, LOW);
        digitalWrite(lmotor_pin2, HIGH);
      } else {
        digitalWrite(lmotor_pin1, LOW);
        digitalWrite(lmotor_pin2, LOW);
      }

      analogWrite(enb, abs(rightMotorSpeed));
      analogWrite(ena, abs(leftMotorSpeed));

      lastRecvTime = millis();
    } else {
      unsigned long now = millis();
      if (now - lastRecvTime > 1000) {
        // Signal lost after 1 second. Stop the motors
        analogWrite(enb, 0);
        analogWrite(ena, 0);
      }
    }
  }
}
