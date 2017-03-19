#include <Wire.h>


void setup() {
  Wire.begin();
}

byte x = 0; 

void loop() {
  Wire.beginTransmission(5);
  Wire.write("i am a whore");
  Wire.write(x);
  Wire.endTransmission();

  x++;
  delay(500);
  

}
