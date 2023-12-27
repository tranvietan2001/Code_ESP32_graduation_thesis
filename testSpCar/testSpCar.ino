#include "BluetoothSerial.h"
#include "config.h"


uint8_t sp = 0, sp1 = 0, sp2 = 0, sp3 = 0, sp4 = 0;
String val;

BluetoothSerial SerialBT;
  
void moveForward(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, speed1);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, speed2);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, speed3);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, speed4);
  analogWrite(IN_R4, 0);
  delay(10);
}
void moveBackward(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, speed1);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, speed2);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, speed3);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, speed4);
  delay(10);
}
void moveLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, speed1);
  analogWrite(IN_L3, speed2);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, speed3);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, speed4);
  delay(10);
}

void moveRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, speed1);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, speed2);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, speed3);
  analogWrite(IN_R3, speed4);
  analogWrite(IN_R4, 0);
  delay(10);
}

void moveForwardLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, speed2);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, speed3);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, 0);
  delay(10);
}

void moveForwardRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, speed1);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, speed4);
  analogWrite(IN_R4, 0);
  delay(10);
}

void moveBackwardLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, speed1);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, speed4);
  delay(10);
}

void moveBackwardRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, speed2);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, speed3);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, 0);
  delay(10);
}

void turnLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, speed1);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, speed2);
  analogWrite(IN_R1, speed3);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, speed4);
  analogWrite(IN_R4, 0);
  delay(10);
}

void turnRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) {
  analogWrite(IN_L1, speed1);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, speed2);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, speed3);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, speed4);
  delay(10);
}
void stop() {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, 0);
  delay(10);
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex [] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void setup() {
  pinMode(IN_L1, OUTPUT);
  pinMode(IN_L2, OUTPUT);
  pinMode(IN_L3, OUTPUT);
  pinMode(IN_L4, OUTPUT);
  pinMode(IN_R1, OUTPUT);
  pinMode(IN_R2, OUTPUT);
  pinMode(IN_R3, OUTPUT);
  pinMode(IN_R4, OUTPUT);

  Serial.begin(9600);
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif


}
void loop() {
  if (SerialBT.available()) {
    val = SerialBT.readStringUntil('\b');
  }
    //    char val = SerialBT.read();
    Serial.println(val);

    if (val.indexOf(',') != -1) {
      String v1, v2, v3, v4, v5;
      v1 = getValue(val, ',', 0);
      v2 = getValue(val, ',', 1);
      v3 = getValue(val, ',', 2);
      v4 = getValue(val, ',', 3);
      sp1 = v1.toInt();
      sp2 = v2.toInt();
      sp3 = v3.toInt();
      sp4 = v4.toInt();
//      Serial.print("Speed1: "); Serial.println(sp1);
//      Serial.print("Speed2: "); Serial.println(sp2);
//      Serial.print("Speed3: "); Serial.println(sp3);
//      Serial.print("Speed4: "); Serial.println(sp4);
    }

    if (val == "F")
      moveForward(sp1, sp2, sp3, sp4);
    else if (val == "B")
      moveBackward(sp1, sp2, sp3, sp4);
    else if (val == "L")
      moveLeft(sp1, sp2, sp3, sp4);
    else if (val == "R")
      moveRight(sp1, sp2, sp3, sp4);
    else if (val == "G")
      moveForwardLeft(sp1, sp2, sp3, sp4);
    else if (val == "I")
      moveForwardRight(sp1, sp2, sp3, sp4);
    else if (val == "H")
      moveBackwardLeft(sp1, sp2, sp3, sp4);
    else if (val == "J")
      moveBackwardRight(sp1, sp2, sp3, sp4);
    else if (val == "S")
      stop();

}
