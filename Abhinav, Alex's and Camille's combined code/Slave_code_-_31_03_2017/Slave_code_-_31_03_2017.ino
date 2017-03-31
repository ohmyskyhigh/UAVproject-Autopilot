// Slave code

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
File logFile;
String received_data_string = "";

void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
  delay(500); // To ensure proper start

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
  logFile.print("accel x (m/s^2)");
  logFile.print(",");
  logFile.print("accel y (m/s^2)");
  logFile.print(",");
  logFile.print("accel z (m/s^2)");
  logFile.print(",");
  logFile.print("gyro x (m/s^2)");
  logFile.print(",");
  logFile.print("gyro y (m/s^2)");
  logFile.print(",");
  logFile.print("gyro z (m/s^2)");
  logFile.print(",");
  logFile.print("press (hPa)");
  logFile.print(",");
  logFile.print("temp (C))");
  logFile.print(",");
  logFile.print("altit (m)");
  logFile.print(",");
  logFile.println("Time (s)");
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all
    char c = Wire.read(); // receive byte as a character
    received_data_string += c;
  }
  char d = Wire.read();
  if(d != ','){
    logFile.println(received_data_string);
    logFile.flush();
    Serial.println(received_data_string);
    Serial.println(" ");
    received_data_string = "";
  }
  else{
    received_data_string += d;
  }
}
