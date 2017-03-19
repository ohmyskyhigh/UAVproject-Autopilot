#include <Wire.h>


void setup() {
  Wire.begin(5);
  Wire.onReceive(dataIN);
  Serial.begin(9600);

}
void dataIN(int numBytes){
  while (1 < Wire.available()){
    char c = Wire.read();
    Serial.print(c);    
  }
  int x = Wire.read();
}
      
void loop() {
  delay(100);

}


