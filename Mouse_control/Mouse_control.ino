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

  if (data == "END") {
    exit(0);
  }
  
  int idx1 = data.indexOf(",");
  int idx2 = data.indexOf(",", idx1+1);
  String gyX_str = data.substring(0, idx1);
  String gyY_str = data.substring(idx1+1, idx2);
  String gyZ_str = data.substring(idx2+1);
  
  gyX = - ((int) (gyX_str.toDouble())) / 2;
  gyZ = - ((int) (gyZ_str.toDouble())) / 2;
}

void mouse_cont() {
  mouseX = constrain(gyZ, -128, 127);
  mouseY = constrain(gyX, -128, 127);

  Mouse.move(mouseX, mouseY, 0);
}
