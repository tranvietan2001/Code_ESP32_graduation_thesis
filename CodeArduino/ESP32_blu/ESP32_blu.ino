#include "config.h"

void rotate(uint8_t in1, uint8_t in2, uint8_t speed) {
  analogWrite(in1, speed);
  digitalWrite(in2, LOW);
}

void forward(uint8_t speed) {

}


void setup() {
  pinMode(IN_L1, OUTPUT);
  pinMode(IN_L2, OUTPUT);
  pinMode(IN_L3, OUTPUT);
  pinMode(IN_L4, OUTPUT);
}

void loop() {

  analogWrite(IN_L1, 160);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 160);
  analogWrite(IN_L4, 0);
  delay(3000);

  analogWrite(IN_L1, 0);
  analogWrite(IN_L2, 160);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, 160);

  delay(3000);

  analogWrite(IN_L1, 160);
  analogWrite(IN_L2, 0);
  analogWrite(IN_L3, 0);
  analogWrite(IN_L4, 160);
  delay(3000);


  //forward
  //  analogWrite(IN_L1, 150);
  //  digitalWrite(IN_L2, LOW);
  //  analogWrite(IN_L3, 200);
  //  digitalWrite(IN_L4, LOW);
  //  delay(3000);

  // stop
  //  analogWrite(IN_L1, 0);
  //  digitalWrite(IN_L2, LOW);
  //  analogWrite(IN_L3, 0);
  //  digitalWrite(IN_L4, LOW);
  //  digitalWrite(IN_L1, LOW);
  //  digitalWrite(IN_L2, LOW);
  //  digitalWrite(IN_L3, LOW);
  //  digitalWrite(IN_L4, LOW);
  //  delay(3000);

  // backword
  //  analogWrite(IN_L2, 150);
  //  digitalWrite(IN_L1, LOW);
  //  analogWrite(IN_L4, 200);
  //  digitalWrite(IN_L3, LOW);
  //  delay(3000);

  // stop
  //  analogWrite(IN_L2, 0);
  //  digitalWrite(IN_L1, LOW);
  //  analogWrite(IN_L4, 0);
  //  digitalWrite(IN_L3, LOW);
  //  delay(3000);




}
