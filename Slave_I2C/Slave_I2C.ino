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
    char* string[] = stringSuckMyDick(x, "Z");
    String pi = string[0];
    String R = string[1];
    Serial.print(c);    
    Serial.print(pi);
    Serial.print(R);
  }
}
      
void loop() {
  delay(100);

}

array stringSuckMyDick(String s, String c){
  int m = s.lastIndexOf(c);
  int var = 0;
  char* myString[];
  while(var < m){
    int lastVar = var;
    int var = s.indexOf(c, var + x);
    mystring[x] = s.substring(lastVar, var);
  }
  return myString;
}


