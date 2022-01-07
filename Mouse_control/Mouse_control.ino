#include <Mouse.h>
#include <HID.h>

String data;
double phi, theta, psi;

void setup() {
  Serial1.begin(9600);
  Mouse.begin();
  
}

void loop() {
 if (Serial1.available() > 0) {
  get_data();
 }


}

void get_data() {
  data = Serial1.readStringUntil('\n');
  String[] data_sub = data.split(',');
  phi = Double.parseDouble(data_sub[0]);
  theta = Double.parseDouble(data_sub[1]);
  psi = Double.parseDouble(data_sub[2]);
}

void mouse_cont() {
  
}
