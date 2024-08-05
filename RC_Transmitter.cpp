#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);

struct PacketData {
  byte forwardBackwardValue;
  byte turnValue;
};

PacketData data;

void setup() {
  btSerial.begin(38400); 
  Serial.begin(9600);
  Serial.println("Transmitter setup complete. Sending data...");
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
  data.forwardBackwardValue = mapAndAdjustJoystickDeadBandValues(analogRead(A0), false);
  data.turnValue = mapAndAdjustJoystickDeadBandValues(analogRead(A2), false);

  String dataString = String(data.forwardBackwardValue) + "," + String(data.turnValue) + "\n";
  
  Serial.print("Sending: ");
  Serial.println(dataString);

  btSerial.print(dataString);
  delay(10);
}
