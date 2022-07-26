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
 if (Serial1.available() > 0) {
  get_data();
  mouse_cont();
  
 }

}

void get_data() {
  data = Serial1.readStringUntil('\n');

  int idx = data.indexOf(",");
  String gyX_str = data.substring(0, idx);
  String gyZ_str = data.substring(idx+1);
  
  gyX = - (int) (gyX_str.toDouble() / 1.5);
  gyZ = - (int) (gyZ_str.toDouble() / 1.5);

  if (-3 < gyZ && 3 > gyZ) {
    gyZ = 0;
  }

  if (-3 < gyX && 3 > gyX) {
    gyX = 0;
  }
}

void mouse_cont() {
  mouseX = constrain(gyZ, -128, 127);
  mouseY = constrain(gyX, -128, 127);

  Mouse.move(mouseX, mouseY, 0);
}
