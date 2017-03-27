#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int howMany) {
  String data_val = "";
  
  while (0 < Wire.available()) { // loop through all but the last
    char char_val = Wire.read(); // receive byte as a character
    //Serial.print(char_val);         // print the character
    data_val += char_val;
  }
  Serial.println(data_val);
}
