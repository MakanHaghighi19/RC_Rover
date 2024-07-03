#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);

//initialize the data received by the transmitted
struct PacketData {
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
};

PacketData data;

//assign ports for right motor
int enb = 7;
int rmotor_pin1 = 10;
int rmotor_pin2 = 9;

//assign ports for left motor
int ena = 8;
int lmotor_pin1 = 12;
int lmotor_pin2 = 11;

unsigned long lastRecvTime = 0;


void setup() {
  pinMode(enb, OUTPUT);
  pinMode(rmotor_pin1, OUTPUT);
  pinMode(rmotor_pin2, OUTPUT);

  pinMode(ena, OUTPUT);
  pinMode(lmotor_pin1, OUTPUT);
  pinMode(lmotor_pin2, OUTPUT);

  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop() {
string dataString;
if (btSerial.available()) 
}
#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);

//initialize the data received by the transmitted
struct PacketData {
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
};

PacketData data;

//assign ports for right motor
int enb = 7;
int rmotor_pin1 = 10;
int rmotor_pin2 = 9;

//assign ports for left motor
int ena = 8;
int lmotor_pin1 = 12;
int lmotor_pin2 = 11;

unsigned long lastRecvTime = 0;


void setup() {
  pinMode(enb, OUTPUT);
  pinMode(rmotor_pin1, OUTPUT);
  pinMode(rmotor_pin2, OUTPUT);

  pinMode(ena, OUTPUT);
  pinMode(lmotor_pin1, OUTPUT);
  pinMode(lmotor_pin2, OUTPUT);

  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop() {
string dataString;
if (btSerial.available()) 
}
