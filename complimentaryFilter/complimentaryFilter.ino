#include <Wire.h>

const int MPU_ADDR = 0x68;
const double raw_to_gyro = 32767 / 250;
const double raw_to_acc = 16384 / 9.8;
const double rad_to_deg = 180 / 3.141592;

int16_t acX, acY, acZ, gyX, gyY, gyZ;
double acX_cal, acY_cal, acZ_cal, gyX_cal, gyY_cal, gyZ_cal;
double ave_gyX = 0, ave_gyY = 0, ave_gyZ = 0;

double phi_acc, theta_acc, phi_gyro, theta_gyro, psi_gyro;
double phi, theta, psi;

unsigned long now = 0;
unsigned long past = 0;
double dt = 0;

void setup() {
  setting();
  ave_cal();
  Serial.begin(115200);
  past = millis();

}

void loop() {
  getData();

  gyX -= ave_gyX; gyY -= ave_gyY; gyZ -= ave_gyZ;

  acX_cal = acX / raw_to_acc;
  acY_cal = acY / raw_to_acc;
  acZ_cal = acZ / raw_to_acc;

  gyX_cal = gyX / raw_to_gyro;
  gyY_cal = gyY / raw_to_gyro;
  gyZ_cal = gyZ / raw_to_gyro;

  getDT();

  phi_acc = atan(acY / sqrt(pow(acX, 2) + pow(acZ, 2))) * rad_to_deg;
  theta_acc = atan(-acX / sqrt(pow(acY, 2) + pow(acZ, 2))) * rad_to_deg;

  phi_gyro += gyX_cal * dt;
  theta_gyro += gyY_cal * dt;
  psi_gyro += gyZ_cal * dt;

  phi = 0.96 * (phi + phi_gyro * dt) + 0.04 * phi_acc;
  theta = 0.96 * (theta + theta_gyro * dt) + 0.04 * theta_acc;
  psi = psi_gyro;
  
//  Serial.print("X축 가속도 : "); Serial.print(acX_cal); Serial.print("\t");
//  Serial.print("Y축 가속도 : "); Serial.print(acY_cal); Serial.print("\t");
//  Serial.print("Z축 가속도 : "); Serial.println(acZ_cal);

//  Serial.print("X축 각속도 : "); Serial.print(gyX_cal); Serial.print("\t");
//  Serial.print("Y축 각속도 : "); Serial.print(gyY_cal); Serial.print("\t");
//  Serial.print("Z축 각속도 : "); Serial.println(gyZ_cal);

//  Serial.print("phi : "); Serial.print(phi); Serial.print("\t");
//  Serial.print("theta : "); Serial.print(theta); Serial.print("\t");
//  Serial.print("psi : "); Serial.println(psi);

  Serial.print(phi); Serial.print(","); 
  Serial.print(theta); Serial.print(",");
  Serial.println(psi);


}

void setting() {
   Wire.begin();
   Wire.beginTransmission(MPU_ADDR);
   Wire.write(0x6B);
   Wire.write(0);
   Wire.endTransmission();
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14);
  acX = Wire.read() << 8 | Wire.read();
  acY = Wire.read() << 8 | Wire.read();
  acZ = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read();
  gyX = Wire.read() << 8 | Wire.read();
  gyY = Wire.read() << 8 | Wire.read();
  gyZ = Wire.read() << 8 | Wire.read();
  Wire.endTransmission();

}

void ave_cal() {
  for (int i=0; i<10; i++) {
    getData();
    ave_gyX += gyX; ave_gyY += gyY; ave_gyZ += gyZ;
    delay(50);
  }

  ave_gyX /= 10; ave_gyY /= 10; ave_gyZ /= 10;
}

void getDT() {
  now = millis();
  dt = (now - past) / 1000.0;
  past = now;
}
