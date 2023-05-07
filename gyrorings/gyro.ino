#include<Wire.h>
#include "settings.h"
#include "gyro.h"


const int MPU=0x68;  // I2C address of the MPU-6050

float EMA_a_low = 0.05;
float EMA_a_high = 0.4;

int16_t EMA_S_low = 0;
int16_t EMA_S_high = 0;

int8_t detect=1;

void setupMPU(){
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);  
}

void readMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)  
}

int16_t abs(int16_t x) {
  if (x < 0) {
    return -x;
  }
  return x;
}

int16_t calcGMov() {
  return abs(GyX)/4+abs(GyY)/4+abs(GyZ)/4;
}

int16_t calcAMov() {
  return abs(AcX)/4+abs(AcY)/4+abs(AcZ)/4;
}

void initEMA() {
  readMPU();
  EMA_S_low = calcAMov();
  EMA_S_high = EMA_S_low;
}

int16_t calcEMA() {
  readMPU();
  int16_t S = calcAMov();
  EMA_S_low = (EMA_a_low*S)+((1-EMA_a_low)*EMA_S_low);
  EMA_S_high = (EMA_a_high*S)+((1-EMA_a_high)*EMA_S_high);
  int16_t v = (EMA_S_high - EMA_S_low);
  if (detect) {
    if (v > 800) {
      detect = 0;
      return v/800;
    }
    return 0;
  } else {
    if (v < 800) {
      detect = 1;
    }
    return 0;
  }
}
