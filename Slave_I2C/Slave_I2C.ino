#include <Wire.h>
#include <SD.h>
#include <SPI.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

File logFile;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");

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
  logFile.print("accel x(m/s^2)*100");
  logFile.print(",");
  logFile.print("accel y(m/s^2)*100");
  logFile.print(",");
  logFile.print("accel z(m/s^2)*100");
  logFile.print(",");
  logFile.print("press (hPa)");
  logFile.print(",");
  logFile.print("tempe (C))");
  logFile.print(",");
  logFile.print("altit (m)");  
  logFile.print(",");
  logFile.print("gyro x(m/s^2)*100");
  logFile.print(",");
  logFile.print("gyro y(m/s^2)*100");
  logFile.print(",");
  logFile.print("gyro z(m/s^2)*100");
  logFile.print(",");
  logFile.println("time s");
}
void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last 
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    logFile.print(c);
  }
  //It will print some wired shit if I don't do this
  Serial.println("");

  logFile.println("");
  logFile.flush();
  Serial.println("...");
  logFile.close();
  
}
