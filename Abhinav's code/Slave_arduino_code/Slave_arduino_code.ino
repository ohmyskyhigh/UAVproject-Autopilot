// Slave code

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
File logFile;

String all_data_csv[10] = {};

void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Serial.begin(9600);
  delay(500);

  pinMode(SS, OUTPUT);
  
  // see if SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("SD card initialized.");

  //give a name to our file
  char filename[15];
  strcpy(filename, "SENLOG00.CSV");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  // Open up the file we're going to log to!
  logFile = SD.open(filename, FILE_WRITE);
  if (! logFile) {
    Serial.println("error opening");
    // Wait forever since we cant write data
    while (1) ;
  }

  //create header for each collumn
  logFile.print("accel x(m/s^2)");
  logFile.print(",");
  logFile.print("accel y(m/s^2)");
  logFile.print(",");
  logFile.print("accel z(m/s^2)");
  logFile.print(",");
  logFile.print("gyro x(m/s^2)");
  logFile.print(",");
  logFile.print("gyro y(m/s^2)");
  logFile.print(",");
  logFile.print("gyro z(m/s^2)");
  logFile.print(",");
  logFile.print("press (hPa)");
  logFile.print(",");
  logFile.print("tempe (C))");
  logFile.print(",");
  logFile.print("altit (m)");
  logFile.print(",");
  logFile.println("Time (s)");
  delay(100);
  
  logFile.close();
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
  
  Serial.println(all_data_csv[data_id]);

  if(data_id == 9){
    // Open up the file we're going to log to!
    if (! logFile) {
      Serial.println("error opening");
      // Wait forever since we cant write data
      while (1) ;
    }
    for(int i = 0; i < 9; i++){
      logFile.print(all_data_csv[i]);
      logFile.print(",");
    }
    logFile.println(all_data_csv[9]);
    logFile.close();
    Serial.println("Data recorded");
    Serial.println(" ");
  }
}
