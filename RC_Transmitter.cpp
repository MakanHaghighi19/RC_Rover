#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX

struct PacketData {
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
};

PacketData data;

void setup() {
  btSerial.begin(38400); 
  Serial.begin(9600);  
}

int mapAndAdjustJoystickDeadBandValues(int value, bool reverse) {
  if (value >= 550) {
    value = map(value, 550, 1023, 127, 254);
  } else if (value <= 480) {
    value = map(value, 480, 0, 127, 0);  
  } else {
    value = 127;
  }

  if (reverse) {
    value = 254 - value;
  }
  return value;
}

void loop() {
  data.lxAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(A1), false);
  data.lyAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(A0), false);
  data.rxAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(A2), false);
  data.ryAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(A3), false);

  String dataString = String(data.lxAxisValue) + "," + String(data.lyAxisValue) + "," +
                      String(data.rxAxisValue) + "," + String(data.ryAxisValue) + "\n";
  
  btSerial.print(dataString);
  delay(10);
}
