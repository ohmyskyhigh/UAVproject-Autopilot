#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20.h>
#include<SPI.h>
#include<SD.h>

//SD card login
File logFile;
const int chipSelect = 4;

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

void displaySensorDetails()
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);


  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);

  gero.read();
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println((String)gyro.name);
  Serial.print  ("Driver Ver:   "); Serial.println(gyro.version);
  Serial.print  ("Unique ID:    "); Serial.println(gyro.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(gyro.max_value);
  Serial.print  ("Min Value:    "); Serial.print(gyro.min_value); 
  Serial.print  ("Resolution:   "); Serial.print(gyro.resolution);  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
  
}

void setup(void){
  //Open serial communications and wait for port to open
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
    pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

//give a name to our file
  char fileName[13];
  strcpy(filename, "LOG00.CSV")
  for (uint8_t i = 0; i < 100; i++){
    filename[3] = "0" + i/10;
    filename[4] = "0" + i%10;
    if(! SD.exists(filename)){
      break;
    }
  }

//create logfile
  logFile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);


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
  //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
  //if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
    /* Display some basic information on this sensor */
  displaySensorDetails();

//create header for each collumn
  logFile.print("accel x(m/s^2)");
  logFile.print("accel y(m/s^2)");
  logFile.print("press (hPa)");
  logFile.print("tempe (C))");
  logFile.print("altit (m)");  
  logFile.print("gyro x(m/s^2)");
  logFile.print("gyro y(m/s^2)");
  logFile.println("gyro z(m/s^2)");
}

void loop()
{

//Accelerometer part
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  /* Note: You can also get the raw (non unified values) for */
  /* the last data sample as follows. The .getEvent call populates */
  /* the raw values used below. */
  //Serial.print("X Raw: "); Serial.print(accel.raw.x); Serial.print("  ");
  //Serial.print("Y Raw: "); Serial.print(accel.raw.y); Serial.print("  ");
  //Serial.print("Z Raw: "); Serial.print(accel.raw.z); Serial.println("");

  /* Delay before the next sample */
  delay(500);


//Pressure sensor part
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");

    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    Serial.println(" m");
    Serial.println("");
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
  delay(100);
}
