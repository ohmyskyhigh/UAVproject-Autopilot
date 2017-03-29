#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20.h>

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




void setup() {
  Wire.begin(); // join i2c bus (address optional for master)   

  Serial.begin(9600);

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

}
void loop() {
  float temperature;
  float seaLevelPressure;

//Accelerometer part
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  char mi[100];
  int Count = 0;
  int a = 100*event.acceleration.x;
  int b = 100*event.acceleration.y;
  int c = 100*event.acceleration.z;

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");





//Pressure sensor part
  bmp.getEvent(&event);
  
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
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,event.pressure)); 
    Serial.println(" m");
  }
  else
  {
    Serial.println("Sensor error");
  }
  int d = event.pressure;
  int e = temperature;
  int f = bmp.pressureToAltitude(seaLevelPressure,event.pressure);

//Gyro part
  gyro.read();
  Serial.print("X: "); Serial.print(gyro.data.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print(gyro.data.y);   Serial.print(" ");
  Serial.print("Z: "); Serial.println(gyro.data.z); Serial.print(" ");
  int g = 100*gyro.data.x;
  int h = 100*gyro.data.y;
  int i = 100*gyro.data.z;

//Transmission
  String o = String(a);
  String p = String(b);
  String q = String(c);
  String r = String(d);
  String s = String(e);
  String t = String(f);
  String u = String(g);
  String v = String(h);
  String w = String(i);
  String m = String(o + "," + p + "," + q + "," + r + "," + s + "," + t + "," + u + "," + v + "," + w);
  m.toCharArray(mi, 100);
  Serial.println(a);
  Serial.println(mi);
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(mi);
  Wire.endTransmission();    // stop transmitting
  Count = Count + 1;
  delay(500);
}

  
