#include <Wire.h>


void setup() {
  Wire.begin(5);
  Wire.onReceive(dataIN);
  Serial.begin(9600);

}
void dataIN(int numBytes){
  while (1 < Wire.available()){
    char c = Wire.read();
    //String stringOne =  String('a');                                          // converting a constant char 
    String x = String(c);
    
    Serial.print(c);    
  }
}
      
void loop() {
  delay(100);

}


