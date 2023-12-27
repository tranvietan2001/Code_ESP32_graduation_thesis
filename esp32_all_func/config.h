const char *pin = "1234";
String device_name = "Robot_ESP32";

#define IN_L1 33
#define IN_L2 32
#define IN_L3 27
#define IN_L4 25
#define IN_R1 23
#define IN_R2 19
#define IN_R3 18
#define IN_R4 26


void moveForward(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) ;
void moveBackward(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) ;
void moveLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) ;
void moveRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) ;
void moveForwardLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) ;
void moveForwardRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4) ;
void moveBackwardLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4);
void moveBackwardRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4);
void turnLeft(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4);
void turnRight(uint8_t speed1, uint8_t speed2, uint8_t speed3, uint8_t speed4);
void stop();
String getValue(String data, char separator, int index);
String sensorToF();
