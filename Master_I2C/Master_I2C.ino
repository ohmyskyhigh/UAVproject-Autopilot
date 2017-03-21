#include <Wire.h>

int x = 3.1415926;
float y = 8.31;
float c = 998.111;
float f = 0.332;


void setup() {
  Wire.begin();
  Serial.begin(9600);

}


void loop() {
  char charA[10];
  char charB[10];
  char charC[10];
  char charD[10];
  dtostrf(x, 4, 3, charA);
  dtostrf(y, 4, 3, charB);
  dtostrf(c, 4, 3, charC);
  dtostrf(f, 4, 3, charD);
  Serial.print(charA);
  Serial.println(charB);
  Serial.println(charC);
  Serial.println(charD);
  Wire.beginTransmission(5);
  //Try to split the following string 
  //String stringOne =  String(5.698, 3);                                // using a float and the decimal 
  Wire.write(x);
  Wire.endTransmission();
  delay(500);
}
  
