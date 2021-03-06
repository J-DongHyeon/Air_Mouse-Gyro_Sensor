#include <Wire.h>
#include <Mouse.h>
#include <HID.h>

const int MPU_ADDR = 0x68;
const double raw_to_gyro = 32767 / 250;

int16_t gyX, gyY, gyZ;
double gyX_cal, gyY_cal, gyZ_cal;
double ave_gyX = 0, ave_gyY = 0, ave_gyZ = 0;

boolean bt_Lnow, bt_Dnow, bt_Rnow, bt_Lpast, bt_Dpast, bt_Rpast, bt_Lstate, bt_Dstate, bt_Rstate;
unsigned bt_Lpush_time, bt_Dpush_time, bt_Rpush_time;

const int L_BUTTON = 13; // 1번 버튼 (좌 클릭)
const int D_BUTTON = 12; // 2번 버튼 (더블 클릭)
const int R_BUTTON = 11; // 3번 버튼 (우 클릭)

// Potentiometer로 가변 저항을 조절함으로써 'wheel_div' 값이 조절된다.
// 손목 각속도 값을 'wheel_div' 값으로 나눔으로써 마우스 움직임 감도를 적절하게 조정할 것이다.
float wheel_div = 6;

int8_t mouseX, mouseY;

void setup() {
  setting();
  ave_cal();
  Serial.begin(9600);

  Mouse.begin();
  
  pinMode(L_BUTTON, INPUT);
  pinMode(D_BUTTON, INPUT);
  pinMode(R_BUTTON, INPUT);
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

  gyX_cal = - (gyX_cal / wheel_div);
  gyZ_cal = - (gyZ_cal / wheel_div);

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

void mouse_cont() {
  mouseX = constrain(gyZ_cal, -128, 127);
  mouseY = constrain(gyX_cal, -128, 127);

  Mouse.move(mouseX, mouseY, 0);

  // Leonardo 보드의 ADC 해상도는 10 bits 이다. 따라서 출력 디지털 값은 0 ~ 1023 이다.
  // 0 ~ 1023 범위를 0 ~ 12 범위로 변환하여 'wheel_div' 에 넣는다.
  wheel_div = (analogRead(A0) * 12 / 1023) + 0.1;
}

void button_cont() {

  // 버튼들은 풀다운 저항으로 연결되어 있다.
  
  bt_Lnow = digitalRead(L_BUTTON);
  bt_Dnow = digitalRead(D_BUTTON);
  bt_Rnow = digitalRead(R_BUTTON);

  if (bt_Lnow != bt_Lpast) {
    bt_Lpush_time = millis();
  }

  if (bt_Dnow != bt_Dpast) {
    bt_Dpush_time = millis();
  }

  if (bt_Rnow != bt_Rpast) {
    bt_Rpush_time = millis();
  }

  if (millis() - bt_Lpush_time > 50) {
    if (bt_Lstate != bt_Lnow) {
      bt_Lstate = bt_Lnow;
      if (bt_Lstate == HIGH) {
        Mouse.press(MOUSE_LEFT);
      } else if (bt_Lstate == LOW) {
        Mouse.release(MOUSE_LEFT);
      }
    }
  }

  if (millis() - bt_Dpush_time > 50) {
    if (bt_Dstate != bt_Dnow) {
      bt_Dstate = bt_Dnow;
      if (bt_Dstate == HIGH) {
        Mouse.click(MOUSE_LEFT);
        Mouse.click(MOUSE_LEFT);
      }
    }
  }

  if (millis() - bt_Rpush_time > 50) {
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
  bt_Dpast = bt_Dnow;
  bt_Rpast = bt_Rnow;
}
