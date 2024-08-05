#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);

struct PacketData {
  byte forwardBackwardValue;
  byte turnValue;
};

PacketData data;

int rightspeedPin = 7;
int dirPin3 = 10;
int dirPin4 = 9;

int leftspeedPin = 8;
int dirPin1 = 12;
int dirPin2 = 11;

int trigPin = 4;
int echoPin = 5;

void setup() {
  pinMode(rightspeedPin, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(dirPin4, OUTPUT);

  pinMode(leftspeedPin, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  btSerial.begin(38400);
}

void loop() {
  long distance = measureDistance();
  if (distance < 35) {
    setMotorSpeedAndDirection(rightspeedPin, dirPin3, dirPin4, 0, true);
    setMotorSpeedAndDirection(leftspeedPin, dirPin1, dirPin2, 0, true);
    return;
  }

  if (btSerial.available()) {
    String dataString = btSerial.readStringUntil('\n');
    sscanf(dataString.c_str(), "%hhu,%hhu", &data.forwardBackwardValue, &data.turnValue);
    controlMotors(data.turnValue, data.forwardBackwardValue);
  }
}

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1;
  return distance;
}

void controlMotors(int forwardBackwardValue, int turnValue) {
  int forwardSpeed = 0;
  int turnSpeed = 0;

  if (forwardBackwardValue < 127) {
    forwardSpeed = map(forwardBackwardValue, 0, 127, 255, 0);
  } else if (forwardBackwardValue > 127) {
    forwardSpeed = map(forwardBackwardValue, 127, 255, 0, 255);
  }

  if (turnValue > 127) {
    turnSpeed = map(turnValue, 0, 127, 255, 0);
  } else if (turnValue < 127) {
    turnSpeed = map(turnValue, 127, 255, 0, 255);
  }

  if (forwardBackwardValue < 127) {
    setMotorSpeedAndDirection(rightspeedPin, dirPin3, dirPin4, forwardSpeed, true);
    setMotorSpeedAndDirection(leftspeedPin, dirPin1, dirPin2, forwardSpeed, true);
  } else if (forwardBackwardValue > 127) {
    setMotorSpeedAndDirection(rightspeedPin, dirPin3, dirPin4, forwardSpeed, false);
    setMotorSpeedAndDirection(leftspeedPin, dirPin1, dirPin2, forwardSpeed, false);
  } else {
    setMotorSpeedAndDirection(rightspeedPin, dirPin3, dirPin4, 0, true);
    setMotorSpeedAndDirection(leftspeedPin, dirPin1, dirPin2, 0, true);
  }

  if (turnValue > 127) {
    setMotorSpeedAndDirection(rightspeedPin, dirPin3, dirPin4, turnSpeed, false);
    setMotorSpeedAndDirection(leftspeedPin, dirPin1, dirPin2, turnSpeed, true);
  } else if (turnValue < 127) {
    setMotorSpeedAndDirection(rightspeedPin, dirPin3, dirPin4, turnSpeed, true);
    setMotorSpeedAndDirection(leftspeedPin, dirPin1, dirPin2, turnSpeed, false);
  }
}

void setMotorSpeedAndDirection(int speedPin, int dirPin1, int dirPin2, int speed, bool forward) {
  if (speed != 0) {
    analogWrite(speedPin, abs(speed));
    digitalWrite(dirPin1, forward ? LOW : HIGH);
    digitalWrite(dirPin2, forward ? HIGH : LOW);
  } else {
    analogWrite(speedPin, 0);
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, LOW);
  }
}
