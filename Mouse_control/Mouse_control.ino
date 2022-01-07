#include <Mouse.h>
#include <HID.h>

String data;
double gyX, gyY;
int8_t mouseX, mouseY;
const boolean END = HIGH;
const int END_PIN = 7;

void setup() {
  Serial1.begin(115200);
  pinMode(END_PIN, INPUT);
  Mouse.begin();
  
  
}

void loop() {
 if (Serial1.available() > 0) {
//  get_data();
//  mouse_cont();

  if (digitalRead(END_PIN) == END) {
    Mouse.end();
    exit(0);
  }
  
 }

 


}

void get_data() {
  data = Serial1.readStringUntil('\n');
  
  int idx1 = data.indexOf(",");
  int idx2 = data.indexOf(",", idx1+1);
  String gyX_str = data.substring(0, idx1);
  String gyY_str = data.substring(idx1+1, idx2);
  String gyZ_str = data.substring(idx2+1);
  
  gyX = gyX_str.toDouble();
  gyY = gyY_str.toDouble();
}

void mouse_cont() {
  mouseX = map(gyX, -250, 250, -127, 127);
  mouseY = map(gyY, -250, 250, -127, 127);

  Mouse.move(mouseX, mouseY, 0);
}
