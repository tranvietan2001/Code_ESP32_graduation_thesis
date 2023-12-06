#include "BluetoothSerial.h"
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini player;

const char *pin = "1234";
String device_name = "Robot_ESP32";

BluetoothSerial SerialBT;

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
}

void loop() {

  if (SerialBT.available()) {
    String val = SerialBT.readStringUntil('\b');
    Serial.println(val);

    if (val == "S")
      player.play(1);
    else if (val == "E")
      player.play(2);
    else if (val == "G")
      player.play(3);
    else if (val == "C")
      player.play(4);
  }
  delay(20);
}
