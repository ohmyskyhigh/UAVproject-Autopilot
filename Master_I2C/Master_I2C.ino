
#include <Wire.h>
char mi[100];
int a = 0;
int b = 552.32;
int c = 98.110;


void setup() {
  Wire.begin(); // join i2c bus (address optional for master)   
  Serial.begin(9600);
}

void loop() {
  String x = String(a);
  String y = String(b);
  String g = String(c);
  String m = String(x + "," + y + "," + g);
  m.toCharArray(mi, 100);
  Serial.println(a);
  Serial.println(mi);
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(mi);
  Wire.endTransmission();    // stop transmitting
  a = a + 1;
  delay(500);
}

  
