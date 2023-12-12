#ifndef CONFIG_H
#define CONFIG_H

#include "Adafruit_VL53L0X.h"
#include <MPU6050_tockn.h>
#include <Wire.h>

#endif

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

#define SHT_LOX1 14
#define SHT_LOX2 15
#define SHT_LOX3 13

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
MPU6050 mpu6050(Wire);

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
