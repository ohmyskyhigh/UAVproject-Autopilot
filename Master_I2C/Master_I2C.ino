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
#else;
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
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }

}
void loop() {
  float temperature;
  float seaLevelPressure;
  char mi[100];
  int Time = millis()/1000;
  int previous_time = -1;

//Accelerometer part
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);


  float accel_X = event.acceleration.x;
  float accel_Y = event.acceleration.y;
  float accel_Z = event.acceleration.z;

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
  int pressure = event.pressure;
  int atitude = bmp.pressureToAltitude(seaLevelPressure,event.pressure);

//Gyro part
  gyro.read();
  Serial.print("X: "); Serial.print(gyro.data.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print(gyro.data.y);   Serial.print(" ");
  Serial.print("Z: "); Serial.println(gyro.data.z); Serial.print(" ");
  float gyro_X = gyro.data.x;
  float gyro_Y = gyro.data.y;
  float gyro_Z = gyro.data.z;

//Transmission
  String o = String(accel_X*100, 0);
  String p = String(accel_Y*100, 0);
  String q = String(accel_Z*100, 0);
  String r = String(pressure);
  String s = String(temperature, 0);
  String t = String(atitude);
  String u = String(gyro_X*100, 0);
  String v = String(gyro_Y*100, 0);
  String w = String(gyro_Z*100, 0);
  String Ttime = String(Time);
  String m = String(o + "," + p + "," + q + "," + r + "," + s + "," + t + "," + u + "," + v + "," + w + "," + Ttime);
  m.toCharArray(mi, 100);
  Serial.println(mi);
  Wire.beginTransmission(8); // transmit to device #8
  if(Time =! previous_time){
    Wire.write(mi);
    Wire.endTransmission();    // stop transmitting
    Serial.println("data sent");
    previous_time = Time;
  }
  delay(100);

}

  
