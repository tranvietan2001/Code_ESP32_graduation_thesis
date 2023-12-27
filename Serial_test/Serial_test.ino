//String getValue(String data, char separator, int index){
//  int found = 0;
//  int strIndex[] = { 0, -1 };
//  int maxIndex = data.length() - 1;
//
//  for (int i = 0; i <= maxIndex && found <= index; i++) {
//      if (data.charAt(i) == separator || i == maxIndex) {
//          found++;
//          strIndex[0] = strIndex[1] + 1;
//          strIndex[1] = (i == maxIndex) ? i+1 : i;
//      }
//  }
//  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
//}
//
//
//void setup() {
//  Serial1.begin(9600, SERIAL_8N1, 4, 5); // Rx = 4, Tx = 5;p 8N1  (8 bit dữ liệu, không sử dụng bit kiểm tra chẵn lẻ, 1 bit kết thúc)
//  //  Serial2.begin(9600);
//  Serial1.println("Serial 1 ok");
//
//  Serial.begin(9600);
//  Serial.println("Serial 0 ok ");
//}
//
//void loop() {
//  //  Serial1.println(Serial1.read());
//  Serial1.print("abc,123,0.34,999");
//    delay(10);
//  //
////  String outMessage = "";
//
//  if (Serial.available()) {
//    String data_out = Serial.readString();
//    Serial.println("============================");
//    Serial.println (data_out);
//    String v1 = getValue(data_out, ',', 0);
//    String v2 = getValue(data_out, ',', 1);
//    String v3 = getValue(data_out, ',', 2);
//    String v4 = getValue(data_out, ',', 3);
//    Serial.print("v1: " + v1);
//    Serial.print("\tv2: " + v2);
//    Serial.print("\tv3: " + v3);
//    Serial.println("\tv4: " + v4);
//    Serial.println("============================");
//  }
//}

#include "BluetoothSerial.h"

const char *pin = "1234";
String device_name = "Robot_ESP";
BluetoothSerial SerialBT;

String data_Uart_Rec;
void setup() {
  Serial.begin(9600);
  Serial.println("Serial CH340");
  Serial2.begin(9600);
  Serial.println("Serial2 Raspberry");
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif
}

void loop() {
  //  Serial.println("----------------");
  //  Serial.print("SEND: ");
  //  Serial2.println("b");
  //  Serial.println("------------------");
  //  delay(100);

  //  if (SerialBT.available()) {
  //    char val = SerialBT.read();
  //    Serial.println(val);
  //
  //    Serial2.println(val);
  //  }
  //        Serial2.println("ESP32");
  //        Serial.println("Truyen UART\n-------------------------");
  if (Serial2.available() > 0) {
    data_Uart_Rec = Serial2.readStringUntil('\b');
    Serial.print("Rec: ");
    //          Serial.println(data_Uart_Rec);
    if (data_Uart_Rec == "0")
      Serial.println(F("STOP"));
    else if (data_Uart_Rec == "1")
      Serial.println(F("FORWARD"));
    else if (data_Uart_Rec == "2")
      Serial.println(F("RIGHT"));
    else if (data_Uart_Rec == "3")
      Serial.println(F("LEFT"));
    else if (data_Uart_Rec == "4")
      Serial.println(F("FORWARD-LEFT"));
    else if (data_Uart_Rec == "5")
      Serial.println(F("FORWARD-RIGHT"));
    else if (data_Uart_Rec == "6")
      Serial.println(F("NO DETECT OBJ"));
    else Serial.println(F("STOP"));
  }
  Serial2.println("ANNNNNNNNNNNNNNNNNNNNNN");
}
