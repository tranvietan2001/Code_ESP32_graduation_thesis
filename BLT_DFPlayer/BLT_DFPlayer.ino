#include "BluetoothSerial.h"
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini player;

const char *pin = "1234";
String device_name = "Robot_ESP32";

BluetoothSerial SerialBT;

void printDetail(uint8_t type, int value);
void setup() {
  Serial.begin(9600);
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
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif
  player.play(4 );
  Serial.println("Da phat");
}

int count = 1;
int numb = 1;
void loop() {

  //  if (SerialBT.available()) {
  //    String val = SerialBT.readStringUntil('\b');
  //    Serial.println(val);
  //
  //    if (val == "E") {
  //      count++;
  //      if (count >= 7)
  //        count = 7;
  //      numb = count;
  //    }
  //  }
  //  Serial.println(numb);
  //
  //  if (player.available()) {
  //    printDetail(player.readType(), player.read()); // In thông tin bài hát hiện tại
  //
  //    if (player.readType() == TimeOut) {
  //      Serial.print("Dang phat: ");
  //      Serial.println(player.read());
  //
  //    }
  //    else if (player.readType() == DFPlayerPlayFinished) {
  //      Serial.print("Phat bai: ");
  //      //      if (numb == 0) {
  //      ////        player.pause();
  //      //      }
  //      //      else {
  //      Serial.println(numb);
  //      player.start();
  //      player.play(numb);
  //      numb = 10;
  //      //      }
  //    }
  //    //    if (player.read() == DFPlayerStatusFinished) // Kiểm tra xem bài hát đã kết thúc chưa
  //    //    {
  //    //      Serial.println("Phay Bai Moi");
  //    //      delay(500);             // Đợi 1 giây trước khi chuyển sang bài hát tiếp theo
  //    //      player.play(count);       // Chuyển sang bài hát tiếp theo
  //    //      delay(500);              // Đợi 0,5 giây để đảm bảo bài hát mới được phát đúng
  //    //    }
  //  }




  //    else if (val == "E")
  //      player.play(2);
  //    else if (val == "G")
  //      player.play(3);
  //    else if (val == "C")
  //      player.play(4);
  //  Serial.println(count);
  //  player.play(count);
  //  printDetail(player.readType(), player.read());

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
