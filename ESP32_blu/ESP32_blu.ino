#include "BluetoothSerial.h"
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini player;

const char *pin = "1234";
String device_name = "Robot_ESP32"; 

BluetoothSerial SerialBT;

void printDetail(uint8_t type, int value);
void setup() {
  Serial.begin(9600);
  Serial.println("Serial 0 OK");
  Serial1.begin(9600, SERIAL_8N1, 4, 5);
  Serial.println("Serial 1 OK");
  Serial2.begin(9600);
  Serial.println("Serial 2 OK");
  if (!player.begin(Serial1)) {
    Serial.println("Unable to begin");
    Serial.println("reconnect || insert SD card!");
    while (true);
  }
  else Serial.println("Player ok");
  player.setTimeOut(500);
  player.volume(30);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can  pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif
  Serial.println("Setup suss"); 
}


void loop() {

//  if(Serial2.available()){
//    String ser2 = Serial2.readStringUntil('\b');
//    Serial.print("SERIAL 2 READ: ");
//    Serial.println(ser2);
//  }

  if (SerialBT.available()) {
    String serBT = SerialBT.readStringUntil('\b');
    Serial.println(serBT);
    Serial2.println(serBT);
  }
                              
  delay(20);
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
