#include <Wire.h>


void setup() {
  Wire.begin();
}

float x = 3.1415926;
float y = 8.31;
float c = 1.111;
float d = 1.04;
String stringA = String(x, 2);
String stringB = String(y, 2);
String stringC = String(c, 2);
String stringD = String(d, 2);

void loop() {
  Wire.beginTransmission(5);
  //Try to split the following string 
  //String stringOne =  String(5.698, 3);                                // using a float and the decimal 
  Wire.write(stringA);
  Wire.write("Z");
  Wire.write(stringB);
  Wire.write("Z");
  Wire.write(stringC);
  Wire.write("Z");
  Wire.write(stringD);
  Wire.endTransmission();
  delay(500);
  

}
