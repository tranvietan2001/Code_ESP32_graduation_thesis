#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini player;
void setup() {
  Serial.begin(9600);
  //  Serial1.begin(9600); //rx9, tx10
  //  Serial2.begin(9600); //rx16, tx17
  Serial1.begin(9600, SERIAL_8N1, 4, 5);

  if (!player.begin(Serial1)) {
    Serial.println("Unable to begin");
    Serial.println("reconnect || insert SD card!");
    while (true);
  }

  Serial.println("connect ok!");

  player.setTimeOut(500);
  player.volume(30);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);
  player.randomAll();

}

void loop() {
  //  player.play(1);
  //  delay(5000);
  //  player.play(2);
  //  delay(5000);
  //  player.play(3);
  //  delay(5000);
  //  player.play(4);
  //  delay(5000);
  //  player.play(5);
  //  delay(5000);

  //    player.randomAll();
  // put your main code here, to run repeatedly:
}
