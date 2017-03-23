#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20.h>
#include <SPI.h>
#include <SD.h>


const int chipSelect = 4;
//assign a variable for creating file
File logFile;

/* Assign a unique ID to this sensor at the same time */ 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

//Gyro primary settings, defining whether to use I²C or SPI
#define USE_I2C

#ifdef USE_I2C
  // The default constructor uses I2C
  Adafruit_L3GD20 gyro;
#else
  // To use SPI, you have to define the pins
  //#define GYRO_CS 4 // labeled CS
  //#define GYRO_DO 5 // labeled SA0
  //#define GYRO_DI 6  // labeled SDA
  //#define GYRO_CLK 7 // labeled SCL
  //Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif


void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

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

  //Accelerometer part
  #ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif
    Serial.begin(9600);
    Serial.println("Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }


//Pressure sensor part
  Serial.begin(9600);
  Serial.println("Pressure Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

//Gyro part
  // Try to initialise and warn if we couldn't detect the chip
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
  {
    //print this massage if gyro is not responding
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }

//create header for each collumn
  logFile.print("accel x(m/s^2)");
  logFile.print(",");
  logFile.print("accel y(m/s^2)");
  logFile.print(",");
  logFile.print("accel z(m/s^2)");
  logFile.print(",");
  logFile.print("press (hPa)");
  logFile.print(",");
  logFile.print("tempe (C))");
  logFile.print(",");
  logFile.print("altit (m)");  
  logFile.print(",");
  logFile.print("gyro x(m/s^2)");
  logFile.print(",");
  logFile.print("gyro y(m/s^2)");
  logFile.print(",");
  logFile.println("gyro z(m/s^2)");
  delay(500);
}

void loop()
{

//millis() time counter
  float Time = millis()/1000;
  Serial.println(Time);
  
//Accelerometer part
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  //log accelerometer data into SD card
  logFile.print(event.acceleration.x);
  logFile.print(",");
  logFile.print(event.acceleration.y);
  logFile.print(",");
  logFile.print(event.acceleration.z);
  logFile.print(",");
  

  /* Delay before the next sample */
  delay(500);


//Pressure sensor part
  bmp.getEvent(&event);
  float temperature;
  float seaLevelPressure;
  
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");

    
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    Serial.println(" m");
    //log pressure sensor data into sd card
    logFile.print(event.pressure);
    logFile.print(",");
    logFile.print(temperature);
    logFile.print(",");
    logFile.print(bmp.pressureToAltitude(seaLevelPressure, event.pressure));
    logFile.print(",");
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(500);


//Gyro part
  gyro.read();
  Serial.print("X: "); Serial.print((int)gyro.data.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)gyro.data.y);   Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)gyro.data.z); Serial.print(" ");
  //log gyro sensor data into sd card
  logFile.print((int)gyro.data.x);
  logFile.print(",");
  logFile.print((int)gyro.data.y);
  logFile.print(",");
  logFile.print((int)gyro.data.z);
  logFile.print(",");
  delay(100);


  
  //log in time, and end logging
  logFile.println(Time);
  logFile.flush();
  Serial.println("...");
  delay(500);
}









