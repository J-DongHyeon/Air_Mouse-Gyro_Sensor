#include <Wire.h>
#include <Mouse.h>
#include <HID.h>

const int MPU_ADDR = 0x68;
const double raw_to_gyro = 32767 / 250;

int16_t gyX, gyY, gyZ;
double gyX_cal, gyY_cal, gyZ_cal;
double ave_gyX = 0, ave_gyY = 0, ave_gyZ = 0;

unsigned long now = 0;
unsigned long past = 0;
double dt = 0;

boolean bt_Lnow, bt_Rnow, bt_Lpast, bt_Rpast, bt_Lstate, bt_Rstate;
unsigned bt_Lpush, bt_Rpush;

const int L_BUTTON = 13;
const int R_BUTTON = 12;

float wheel_div = 6;

int8_t mouseX, mouseY;


void setup() {
  setting();
  ave_cal();
  Serial.begin(9600);
  past = millis();

  Mouse.begin();
  pinMode(L_BUTTON, INPUT);
  pinMode(R_BUTTON, INPUT);

}

void loop() {
  
  getData();

  gyX -= ave_gyX; gyY -= ave_gyY; gyZ -= ave_gyZ;

  gyX_cal = gyX / raw_to_gyro;
  gyY_cal = gyY / raw_to_gyro;
  gyZ_cal = gyZ / raw_to_gyro;

  getDT();
  

//  Serial.print("X축 각속도 : "); Serial.print(gyX_cal); Serial.print("\t");
//  Serial.print("Y축 각속도 : "); Serial.print(gyY_cal); Serial.print("\t");
//  Serial.print("Z축 각속도 : "); Serial.println(gyZ_cal);

  gyX_cal = - (int) (gyX_cal / wheel_div);
  gyZ_cal = - (int) (gyZ_cal / wheel_div);

  mouse_cont();
  button_cont();

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
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);
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

void mouse_cont() {
  mouseX = constrain(gyZ_cal, -128, 127);
  mouseY = constrain(gyX_cal, -128, 127);

  Mouse.move(mouseX, mouseY, 0);

  wheel_div = analogRead(A0) * 12 / 1023;
}

void button_cont() {

  bt_Lnow = digitalRead(L_BUTTON);
  bt_Rnow = digitalRead(R_BUTTON);

  if (bt_Lnow != bt_Lpast) {
    bt_Lpush = millis();
  }

  if (bt_Rnow != bt_Rpast) {
    bt_Rpush = millis();
  }

  if (millis() - bt_Lpush > 50) {
    if (bt_Lstate != bt_Lnow) {
      bt_Lstate = bt_Lnow;
      if (bt_Lstate == HIGH) {
        Mouse.press(MOUSE_LEFT);
      } else if (bt_Lstate == LOW) {
        Mouse.release(MOUSE_LEFT);
      }
    }
  }

  if (millis() - bt_Rpush > 50) {
    if (bt_Rstate != bt_Rnow) {
      bt_Rstate = bt_Rnow;
      if (bt_Rstate == HIGH) {
        Mouse.press(MOUSE_RIGHT);
      } else if (bt_Rstate == LOW) {
        Mouse.release(MOUSE_RIGHT);
      }
    }
  }

  bt_Lpast = bt_Lnow;
  bt_Rpast = bt_Rnow;

  
  
}
