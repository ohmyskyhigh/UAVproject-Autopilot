#include <Wire.h>

String all_data_csv[9] = {};

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
  String data_val_string = "";
  String data_id_string = "";
  
  while (1 < Wire.available()) { // loop through all but the last
    char char_val = Wire.read(); // receive byte as a character
    data_val_string += char_val;
  }
  
  char char_id = Wire.read();
  data_id_string += char_id;

  int data_id = (int)data_id_string[0]-48;
 
  all_data_csv[data_id] = data_val_string;
  
  Serial.println(all_data_csv[0]);
  Serial.println(all_data_csv[1]);
  Serial.println(all_data_csv[2]);
  Serial.println(all_data_csv[3]);
  Serial.println(all_data_csv[4]);
  Serial.println(all_data_csv[5]);
  Serial.println(" ");
}
