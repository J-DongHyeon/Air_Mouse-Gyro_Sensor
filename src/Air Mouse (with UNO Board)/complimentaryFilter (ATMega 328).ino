#include <Wire.h>

// MPU 6050의 I2C 기본 주소
const int MPU_ADDR = 0x68;

// MPU 6050에 저장되는 각속도 데이터는 unsigned 2Byte 데이터 이므로 0 ~ 65535 사이의 값으로 저장된다.
// MPU 6050의 기본적인 각속도 측정 범위는 -250 º/s ~ +250 º/s 이다.
const double raw_to_gyro = 32767 / 250;

int16_t gyX, gyY, gyZ;
double gyX_cal, gyY_cal, gyZ_cal;
double ave_gyX = 0, ave_gyY = 0, ave_gyZ = 0;


void setup() {
  setting();
  ave_cal();
  Serial.begin(9600);
}

void loop() {

  getData();

  gyX -= ave_gyX; gyY -= ave_gyY; gyZ -= ave_gyZ;

  gyX_cal = gyX / raw_to_gyro;
  gyY_cal = gyY / raw_to_gyro;
  gyZ_cal = gyZ / raw_to_gyro;
  

//  Serial.print("X축 각속도 : "); Serial.print(gyX_cal); Serial.print("\t");
//  Serial.print("Y축 각속도 : "); Serial.print(gyY_cal); Serial.print("\t");
//  Serial.print("Z축 각속도 : "); Serial.println(gyZ_cal);

  // 하드웨어 시리얼로 ATMega 16u2에 x축, z축 각속도 데이터를 전송한다.
  Serial.print(gyX_cal); Serial.print(",");
  Serial.println(gyZ_cal);
}

// MPU 6050과의 IC2 연결을 시작하고, 센서를 활성화 시키는 함수이다.
void setting() {
   Wire.begin();
   Wire.beginTransmission(MPU_ADDR);
   Wire.write(0x6B);
   Wire.write(0);
   Wire.endTransmission();
}

// MPU 6050으로부터 x, y, z축 각속도 데이터를 받아오는 함수이다.
void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);
  gyX = Wire.read() << 8 | Wire.read();
  gyY = Wire.read() << 8 | Wire.read();
  gyZ = Wire.read() << 8 | Wire.read();
  Wire.endTransmission();
}

// MPU 6050을 처음 활성화 시켰을 때 초기 잡음이 있을 수 있다.
// 10개 각속도 데이터의 평균을 구하여 초기 잡음으로 취급한다.
void ave_cal() {
  for (int i=0; i<10; i++) {
    getData();
    ave_gyX += gyX; ave_gyY += gyY; ave_gyZ += gyZ;
    delay(50);
  }

  ave_gyX /= 10; ave_gyY /= 10; ave_gyZ /= 10;
}

