#include "BluetoothSerial.h"
#include "DFRobotDFPlayerMini.h"


const char *pin = "1234";
String device_name = "Robot_ESP";

BluetoothSerial SerialBT;
String data_Uart_Rec = "", data_Uart_Rec_Pre = "";
DFRobotDFPlayerMini player;

void printDetail(uint8_t type, int value);

void setup() {
  Serial.begin(9600);
  Serial.println("Serial CH340");
  Serial2.begin(9600);
  Serial.println("Serial2 Raspberry");
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  Serial1.begin(9600, SERIAL_8N1, 4, 5);
  if (!player.begin(Serial1)) {
    Serial.println("Unable to begin");
    Serial.println("reconnect || insert SD card!");
    while (true);
  }
  player.setTimeOut(500);
  player.volume(30);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);


#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

  //  player.play(2);

  Serial.println("Setup SUSS");
}

String serBT;
bool pl = false;
void loop() {
  if (pl == false) {
    player.play(2);
    pl = true;
  }

  if (SerialBT.available()) {
    serBT = SerialBT.readStringUntil('\b');
    Serial.println(serBT);
    Serial2.println(serBT);
  }
  if (serBT == "C") {
    player.play(3);
    Serial2.println(serBT);
    serBT = "";
  }
  else if (serBT == "D") {
    player.play(9);
    Serial2.println(serBT);
    serBT = "";
  }
  else if (serBT == "s") {
    player.play(4);
    Serial2.println(serBT);
    serBT = "";

    delay(15000);
    player.play(5);
  }
  else if (serBT == "e") {
    player.play(6);
    Serial2.println(serBT);
    serBT = "";
    data_Uart_Rec_Pre = ""; 
  }
  else if (serBT == "r") {
   ESP.restart();
  }

  if (Serial2.available() > 0) {
    data_Uart_Rec = Serial2.readStringUntil('\b');
    Serial.print("Rec: ");
    Serial.println(data_Uart_Rec);
    //        if (data_Uart_Rec == "ST")
    //          Serial.println(F("STOP"));
    //        else if (data_Uart_Rec == "F")
    //          Serial.println(F("FORWARD"));
    //        else if (data_Uart_Rec == "R")
    //          Serial.println(F("RIGHT"));
    //        else if (data_Uart_Rec == "L")
    //          Serial.println(F("LEFT"));
    //        else if (data_Uart_Rec == "FL")
    //          Serial.println(F("FORWARD-LEFT"));
    //        else if (data_Uart_Rec == "FR")
    //          Serial.println(F("FORWARD-RIGHT"));
    //        else if (data_Uart_Rec == "ND")
    //          Serial.println(F("NO DETECT OBJ"));
    //        else Serial.println(F("STOP"));
  }


  if (data_Uart_Rec == "NT" && (data_Uart_Rec != data_Uart_Rec_Pre)) {
    //    Serial.print("Rec: ");
    //    Serial.println(data_Uart_Rec);
    player.play(7);
    data_Uart_Rec_Pre = data_Uart_Rec;
    Serial.println(data_Uart_Rec_Pre);
  }
}


void printDetail(uint8_t type, int value) {
  switch (type)  {
    case TimeOut:
      Serial.println("Time Out");
      break;
    case WrongStack:
      Serial.println("Stack Wrong");
      break;
    case DFPlayerCardInserted:
      Serial.println("Card Inserted");
      break;
    case DFPlayerCardRemoved:
      Serial.println("Card Removed");
      break;
    case DFPlayerCardOnline:
      Serial.println("Card Online");
      break;
    case DFPlayerPlayFinished:
      Serial.print("Number:");
      Serial.print(value);
      Serial.println(" Play Finished");
      break;
    case DFPlayerError:
      Serial.print("DFPlayerError:");
      switch (value)
      {
        case Busy:
          Serial.println("Card not found");
          break;
        case Sleeping:
          Serial.println("Sleeping");
          break;
        case SerialWrongStack:
          Serial.println("Get Wrong Stack");
          break;
        case CheckSumNotMatch:
          Serial.println("Check Sum Not Match");
          break;
        case FileIndexOut:
          Serial.println("File Index Out");
          break;
        case FileMismatch:
          Serial.println("Cannot Find File");
          break;
        case Advertise:
          Serial.println("In Advertise");
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
