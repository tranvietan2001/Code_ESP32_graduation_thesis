#include "BluetoothSerial.h"
#include "config.h"


uint8_t sp = 0;
BluetoothSerial SerialBT;

void moveForward(uint8_t speed) {
  analogWrite(IN_L1, speed);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, speed);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, speed);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, speed);
  analogWrite(IN_R4, 0);
  delay(10);
}
void moveBackward(uint8_t speed) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, speed);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, speed);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, speed);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, speed);
  delay(10);
}
void moveLeft(uint8_t speed) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, speed);
  analogWrite(IN_L3, speed);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, speed);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, speed);
  delay(10);
}

void moveRight(uint8_t speed) {
  analogWrite(IN_L1, speed);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, speed);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, speed);
  analogWrite(IN_R3, speed);
  analogWrite(IN_R4, 0);
  delay(10);
}

void moveForwardLeft(uint8_t speed) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, speed);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, speed);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, 0);
  delay(10);
}

void moveForwardRight(uint8_t speed) {
  analogWrite(IN_L1, speed);
  digitalWrite(IN_L2, LOW);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, speed);
  digitalWrite(IN_R4, LOW);
  delay(10);
}

void moveBackwardLeft(uint8_t speed) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, speed);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, 0);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, 0);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, speed);
  delay(10);
}

void moveBackwardRight(uint8_t speed) {
  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, speed);
  analogWrite(IN_R1, 0);
  analogWrite(IN_R2, speed);
  analogWrite(IN_R3, 0);
  analogWrite(IN_R4, 0);
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

void setup() {
  pinMode(IN_L1, OUTPUT);
  pinMode(IN_L2, OUTPUT);
  pinMode(IN_L3, OUTPUT);
  pinMode(IN_L4, OUTPUT);
  pinMode(IN_R1, OUTPUT);
  pinMode(IN_R2, OUTPUT);
  pinMode(IN_R3, OUTPUT);
  pinMode(IN_R4, OUTPUT);

  Serial.begin(115200);
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
//    String val = SerialBT.readStringUntil('\b');
    char val = SerialBT.read();
    Serial.println(val);
//    delay(10);

//    switch (val) {
//      case ('1'):
//        sp = 150; break;
//      case ('2'):
//        sp = 160; break;
//      case ('3'):
//        sp = 170; break;
//      case ('4'):
//        sp = 180; break;
//      case ('5'):
//        sp = 190; break;
//      case ('6'):
//        sp = 200; break;
//      case ('7'):
//        sp = 210; break;
//      case ('8'):
//        sp = 220; break;
//      case ('9'):
//        sp = 230; break;
//      case ('q'):
//        sp = 255; break;
//      case ('F'):
//        moveForward(sp);
//        break;
//      case ('B'):
//        moveBackward(sp);
//        break;
//      case ('L'):
//        moveLeft(sp);
//        break;
//      case ('R'):
//        moveRight(sp);
//        break;
//      case ('G'):
//        moveForwardLeft(sp);
//        break;
//      case ('I'):
//        moveForwardRight(sp);
//        break;
//      case ('H'):
//        moveBackwardLeft(sp);
//        break;
//      case ('J'):
//        moveBackwardRight(sp);
//        break;
//      case ('S'):
//        stop();
//        break;
//    }
  }
  //  moveForward(200);
  //  stop();
}
