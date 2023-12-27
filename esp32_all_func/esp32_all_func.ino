#include "BluetoothSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "config.h"


String data_Uart_Rec = "", serBT = "", data_Uart_Rec_Pre = "";

BluetoothSerial SerialBT;
DFRobotDFPlayerMini player;

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
  Serial.println("Setup SUSS");

  xTaskCreatePinnedToCore(taskControlHand, "taskControlHand", 1024 * 4, NULL, 1, NULL, 0);
}

void loop() {

  if (Serial2.available() > 0) {
    data_Uart_Rec = Serial2.readStringUntil('\b');
    //    Serial.print("Rec: ");
    //    Serial.println(data_Uart_Rec);
  } else data_Uart_Rec = "";

  //  Serial.print("BLT: "); Serial.println(serBT);
  //  Serial.print("\tSr2: "); Serial.println(data_Uart_Rec);


  //  if (serBT == "F" || data_Uart_Rec == "F")
  //    moveForward(sp1, sp2, sp3, sp4);
  //  else if (serBT == "B")
  //    moveBackward(sp1, sp2, sp3, sp4);
  //  else if (serBT == "L" || data_Uart_Rec == "L")
  //    moveLeft(sp1, sp2, sp3, sp4);
  //  else if (serBT == "R" || data_Uart_Rec == "R")
  //    moveRight(sp1, sp2, sp3, sp4);
  //  else if (serBT == "G" || data_Uart_Rec == "FL")
  //    moveForwardLeft(sp1, sp2, sp3, sp4);
  //  else if (serBT == "I" || data_Uart_Rec == "FR")
  //    moveForwardRight(sp1, sp2, sp3, sp4);
  //  else if (serBT == "H")
  //    moveBackwardLeft(sp1, sp2, sp3, sp4);
  //  else if (serBT == "J")
  //    moveBackwardRight(sp1, sp2, sp3, sp4);
  //  else if (serBT == "S" || data_Uart_Rec == "ST" || data_Uart_Rec == "NT")
  //    stop();
  //
  //  if (data_Uart_Rec == "NT" && (data_Uart_Rec != data_Uart_Rec_Pre)) {
  //
  //    player.play(7);
  //    data_Uart_Rec_Pre = data_Uart_Rec;
  //
  //  }
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
      v4 = getValue(task_BLT, ',', 3);
      sp1 = v1.toInt();
      sp2 = v2.toInt();
      sp3 = v3.toInt();
      sp4 = v4.toInt();

      Serial.print("Speed1: "); Serial.println(sp1);
      Serial.print("Speed2: "); Serial.println(sp2);
      Serial.print("Speed3: "); Serial.println(sp3);
      Serial.print("Speed4: "); Serial.println(sp4);
    }
    //    else {
    //      sp1 = 180;
    //      sp2 = 180;
    //      sp3 = 180;
    //      sp4 = 180;
    //    }

    if (task_BLT == "F")
      moveForward(sp1, sp2, sp3, sp4);
    else if (task_BLT == "B")
      moveBackward(sp1, sp2, sp3, sp4);
    else if (task_BLT == "L")
      moveLeft(sp1, sp2, sp3, sp4);
    else if (task_BLT == "R")
      moveRight(sp1, sp2, sp3, sp4);
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

    //    if (data_Uart_Rec == "NT" && (data_Uart_Rec != data_Uart_Rec_Pre)) {
    //
    //      player.play(7);
    //      data_Uart_Rec_Pre = data_Uart_Rec;
    //
    //    }
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

//String sensorToF();
