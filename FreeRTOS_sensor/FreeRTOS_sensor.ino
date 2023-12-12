#include "func.h"

static void taskTestLed(void *par);
static void setDataSensor(void *par);
static void getDataSensor(void *par);

QueueHandle_t queueData;


void setup() {
  Serial.begin(115200);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  Serial.println(F("Shutdown pins inited..."));
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  Serial.println(F("Both in reset mode...(pins are low)"));
  Serial.println(F("Starting..."));



  queueData = xQueueCreate(10, sizeof(String));
  //  xTaskCreate(taskTestLed, "led", 1024 * 2,  NULL, 3, NULL);
  //  xTaskCreate(setDataSensor, "setData", 1024 * 2,  NULL, 3, NULL);
  //  xTaskCreate(getDataSensor, "setData", 1024 * 2,  NULL, 3, NULL);
  xTaskCreatePinnedToCore(taskTestLed, "taskTestLed", 1024 * 1, NULL, 1, NULL, 0 );
  xTaskCreatePinnedToCore(setDataSensor, "Read Data From Sensors", 1024 * 3, NULL, 3, NULL, 1 );
  xTaskCreatePinnedToCore(getDataSensor, "Get data sensors from Queue", 1024 * 3, NULL, 3, NULL, 1 );

}

void loop() {
  vTaskDelete(NULL);
}


void taskTestLed(void *par) {
  pinMode(2, OUTPUT);
  for (;;) {
    digitalWrite(2, 1);
    vTaskDelay(200);
    digitalWrite(2, 0);
    vTaskDelay(200);
  }
}

void setDataSensor(void *par) {
  int i = 0;
  String s = "Data: ";
  String tof1, tof2, tof3;
  setID();
  for (;;) {
    i++;

    lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
    lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
    lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!

    if (measure1.RangeStatus != 4) tof1 = String(measure1.RangeMilliMeter);
    else tof1 = "10000";
    if (measure2.RangeStatus != 4) tof2 = String(measure2.RangeMilliMeter);
    else tof2 = "10000";
    if (measure1.RangeStatus != 4) tof3 = String(measure3.RangeMilliMeter);
    else tof3 = "10000";
    s = tof1 + "," + tof2 + "," + tof3;
    xQueueOverwrite(queueData, &s);
    vTaskDelay(10);
  }
}

void getDataSensor(void *par) {
  String dr;
  String v1, v2, v3;
  int tof1, tof2, tof3;
  for (;;) {
    xQueueReceive(queueData, &dr, 10);
//    Serial.println("Data in Queue");
    tof1 = getValue(dr, ',', 0).toInt();
    tof2 = getValue(dr, ',', 1).toInt();
    tof3 = getValue(dr, ',', 2).toInt();
    Serial.print("ToF1: "); Serial.print(tof1);
    Serial.print("\tToF2: "); Serial.print(tof2);
    Serial.print("\tToF3: "); Serial.println(tof3);
//    Serial.println("=================");
  }
}

  //QueueHandle_t queueData;
  //static void setData(void *params);
  //static void getData(void *params);
  //
  //void setup() {
  //  queueData = xQueueCreate(1, sizeof(String));
  //  xTaskCreate(setData, "setData", 1024 * 2,  NULL, 3, NULL);
  //  xTaskCreate(getData, "getData", 1024 * 2, NULL, 3, NULL );
  //
  //  Serial.begin(115200);
  //  Serial.println("------------Hello------------");
  //}
  //
  //void loop() {
  //  vTaskDelete(NULL);
  //}
  //
