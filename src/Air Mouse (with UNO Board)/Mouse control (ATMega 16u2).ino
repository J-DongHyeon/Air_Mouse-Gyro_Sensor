#include <Mouse.h>
#include <HID.h>

String data;
int gyX, gyZ;
int8_t mouseX, mouseY;

void setup() {
  Serial1.begin(9600);
  Mouse.begin();
}

void loop() {
 if (Serial1.available() > 0) // ATMega 328로부터 하드웨어 시리얼로 각속도 데이터를 받는 경우
 {
  get_data();
  mouse_cont();
 }

}

// 하드웨어 시리얼로 받은 각속도 데이터를 파싱하는 함수이다.
void get_data() {
  data = Serial1.readStringUntil('\n');

  int idx = data.indexOf(",");
  String gyX_str = data.substring(0, idx);
  String gyZ_str = data.substring(idx+1);
  
  // 여러 번 테스트 해보니, 손목 각속도 데이터를 그대로 사용하면 마우스 움직임 감도가 너무 크다.
  // 따라서 각속도 데이터를 1.5 정도로 나눠서 사용하였다.
  gyX = - (int) (gyX_str.toDouble() / 1.5);
  gyZ = - (int) (gyZ_str.toDouble() / 1.5);

  // 아주 미세한 손목의 움직임 또는 미세한 각속도 데이터 잡음에 대해 무시하기 위한 부분이다.
  if (-3 < gyZ && 3 > gyZ) {
    gyZ = 0;
  }
  if (-3 < gyX && 3 > gyX) {
    gyX = 0;
  }
}

// 가공된 손목 각속도 데이터에 따라 마우스 움직임을 제어하는 함수이다.
void mouse_cont() {
  mouseX = constrain(gyZ, -128, 127);
  mouseY = constrain(gyX, -128, 127);

  Mouse.move(mouseX, mouseY, 0);
}
