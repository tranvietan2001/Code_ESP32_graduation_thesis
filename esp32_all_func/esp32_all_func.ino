#include "BluetoothSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Adafruit_VL53L0X.h"
#include <MPU6050_tockn.h>
#include <Wire.h>
#include "config.h"

String data_Uart_Rec = "", serBT = "", data_Uart_Rec_Pre = "";
float tempX = 0;
int dis1 = 0, dis2 = 0, dis3 = 0;
uint8_t disStatus = 0;
int threshold = 500;
uint8_t countErr = 0;

BluetoothSerial SerialBT;
DFRobotDFPlayerMini player;
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
MPU6050 mpu6050(Wire);
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;

static void taskControlHand(void *par);
void setup() {
  pinMode(IN_L1, OUTPUT);
  pinMode(IN_L2, OUTPUT);
  pinMode(IN_L3, OUTPUT);
  pinMode(IN_L4, OUTPUT);
  pinMode(IN_R1, OUTPUT);
  pinMode(IN_R2, OUTPUT);
  pinMode(IN_R3, OUTPUT);
  pinMode(IN_R4, OUTPUT);

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  Serial.println(F("Shutdown pins inited..."));
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  Serial.println(F("Both in reset mode...(pins are low)"));
  Serial.println(F("Starting..."));

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  setIDSensor();

  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
  tempX = mpu6050.getAngleX();

  Serial.begin(9600);
  Serial.println("Serial0 OK");
  Serial1.begin(9600, SERIAL_8N1, 4, 5);
  Serial.println("Serial1 OK");
  Serial2.begin(9600);
  Serial.println("Serial2 OK");

  SerialBT.begin(device_name);
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  if (!player.begin(Serial1)) {
    Serial.println("Unable to begin");
    Serial.println("reconnect || insert SD card!");
    while (true);
  }

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

  player.setTimeOut(500);
  player.volume(30);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);
  player.play(2);
  delay(20000);
  Serial.println("Setup SUSS");

  xTaskCreatePinnedToCore(taskControlHand, "taskControlHand", 1024 * 4, NULL, 1, NULL, 0);
}

void loop() {
  uint8_t spl1 = 180, spl2 = 180, spl3 = 180, spl4 = 180;
  if (Serial2.available() > 0) {
    data_Uart_Rec = Serial2.readStringUntil('\b');
  }
  Serial.println(data_Uart_Rec);
  if (measure1.RangeStatus != 4) {    // if not out of range
    //    Serial.print(measure1.RangeMilliMeter);
    dis1 = measure1.RangeMilliMeter;
  }
  else dis1 = 10000;

  if (measure2.RangeStatus != 4) {
    dis2 = measure2.RangeMilliMeter;
    //    Serial.print(measure2.RangeMilliMeter);
  }
  else dis2 = 10000;
  if (measure3.RangeStatus != 4) {
    dis3 = measure3.RangeMilliMeter;
    //    Serial.print(measure3.RangeMilliMeter);
  }
  else dis3 = 10000;

  mpu6050.update();
  if (mpu6050.getAngleX() > (tempX + 7) || mpu6050.getAngleX() < (tempX - 7)) {
    countErr ++;
    if (countErr >= 5) {
      if (player.available()) {
        if (player.readType() == DFPlayerPlayFinished) {
          player.play(8);
        }
      }
      countErr = 0;
    }
  }

  if (data_Uart_Rec == "F" && dis2 > threshold )
    moveForward(spl1, spl2, spl3, spl4);
  else if (data_Uart_Rec == "F" && dis2 <= threshold ) {
    if (dis1 <= threshold && dis3 <= threshold)
      stop();
    //warning sound
    else if (dis1 > threshold && dis3 <= threshold)
      //      moveRight(spl1, spl2, spl3, spl4);
      moveFarwardRight(spl1, spl2, spl3, spl4);
    else if (dis1 <= threshold && dis3 > threshold)
      moveFarwardLeft(spl1, spl2, spl3, spl4);
  }
  else if (data_Uart_Rec == "L")
    moveLeft(spl1, spl2, spl3, spl4);
  else if (data_Uart_Rec == "R")
    moveRight(spl1, spl2, spl3, spl4);
  else if (data_Uart_Rec == "FL")
    moveForwardLeft(spl1, spl2, spl3, spl4);
  else if (data_Uart_Rec == "FR")
    moveForwardRight(spl1, spl2, spl3, spl4);
  else if (data_Uart_Rec == "ST" || data_Uart_Rec == "NT")
    stop();

  if (data_Uart_Rec == "NT" && (data_Uart_Rec != data_Uart_Rec_Pre)) {
    player.play(7);
    data_Uart_Rec_Pre = data_Uart_Rec;
  }
}

void taskControlHand(void *par) {
  Serial.println("Task Control Hand SUSS");
  String task_BLT;
  uint8_t sp = 0, sp1 = 0, sp2 = 0, sp3 = 0, sp4 = 0;
  for (;;) {
    if (SerialBT.available()) {
      task_BLT = SerialBT.readStringUntil('\b');
      Serial2.println(task_BLT);
    }
    Serial.println(task_BLT);
    if (task_BLT == "C") {
      player.play(3);
      task_BLT = "";
    }
    else if (task_BLT == "D") {
      player.play(9);
      task_BLT = "";
    }
    else if (task_BLT == "s") {
      player.play(4);
      Serial2.println(task_BLT);
      task_BLT = "";

      delay(15000);
      player.play(5);
    }
    else if (task_BLT == "e") {
      player.play(6);
      Serial2.println(task_BLT);
      task_BLT = "";
      data_Uart_Rec_Pre = "";
    }
    else if (task_BLT == "r") {
      ESP.restart();
      task_BLT = "";
    }
    else if (task_BLT.indexOf(',') != -1) {
      String v1, v2, v3, v4;
      v1 = getValue(task_BLT, ',', 0);
      v2 = getValue(task_BLT, ',', 1);
      v3 = getValue(task_BLT, ',', 2);
      v4 = getValue(task_BLT , ',', 3);
      sp1 = v1.toInt();
      sp2 = v2.toInt();
      sp3 = v3.toInt();
      sp4 = v4.toInt();

      //      Serial.print("Speed1: "); Serial.println(sp1);
      //      Serial.print("Speed2: "); Serial.println(sp2);
      //      Serial.print("Speed3: "); Serial.println(sp3);
      //      Serial.print("Speed4: "); Serial.println(sp4);
    }
    if (task_BLT == "F")
      moveForward(sp1, sp2, sp3, sp4);
    else if (task_BLT == "B")
      moveBackward(sp1, sp2, sp3, sp4);
    else if (task_BLT == "L")
      //      moveLeft(sp1, sp2, sp3, sp4);
      turnLeft(sp1, sp2, sp3, sp4);
    else if (task_BLT == "R")
      //      moveRight(sp1, sp2, sp3, sp4);
      turnRight(sp1, sp2, sp3, sp4);
    else if (task_BLT == "G")
      moveForwardLeft(sp1, sp2, sp3, sp4);
    else if (task_BLT == "I")
      moveForwardRight(sp1, sp2, sp3, sp4);
    else if (task_BLT == "H")
      moveBackwardLeft(sp1, sp2, sp3, sp4);
    else if (task_BLT == "J")
      moveBackwardRight(sp1, sp2, sp3, sp4);
    else if (task_BLT == "S")
      stop();
  }
}
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
  speed1 = speed1 + 60;
  speed2 = speed2 + 60;
  speed3 = speed3 + 60;
  speed4 = speed4 + 60;
  if (speed1 >= 255) speed1 = 255;
  if (speed2 >= 255) speed2 = 255;
  if (speed3 >= 255) speed3 = 255;
  if (speed4 >= 255) speed5 = 255;
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
  speed1 = speed1 + 60;
  speed2 = speed2 + 60;
  speed3 = speed3 + 60;
  speed4 = speed4 + 60;
  if (speed1 >= 255) speed1 = 255;
  if (speed2 >= 255) speed2 = 255;
  if (speed3 >= 255) speed3 = 255;
  if (speed4 >= 255) speed5 = 255;
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
  speed2 = speed2 + 60;
  speed3 = speed3 + 60;
  if (speed2 >= 255) speed2 = 255;
  if (speed3 >= 255) speed3 = 255;
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
  speed1 = speed1 + 60;
  speed4 = speed4 + 60;
  if (speed1 >= 255) speed1 = 255;
  if (speed4 >= 255) speed5 = 255;
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
  speed1 = speed1 + 60;
  speed4 = speed4 + 60;
  if (speed1 >= 255) speed1 = 255;
  if (speed4 >= 255) speed5 = 255;
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
  speed2 = speed2 + 60;
  speed3 = speed3 + 60;
  if (speed2 >= 255) speed2 = 255;
  if (speed3 >= 255) speed3 = 255;
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

void setIDSensor() {
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  delay(10);
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, LOW);
  delay(10);

  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }

  digitalWrite(SHT_LOX3, HIGH);
  if (!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }
  Serial.println("init VL53LX success");
}


void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
