// Master code by Abhinav, Kyle and Joe

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <PID_v1.h>

Servo ailerons;
Servo elevator;

int ailerons_pin = 11;
int elevator_pin = 10;
int gear_receiver_pin = 0;
int relay_switch_pin = 12;

// For Roll PID stabilization 
//Define Variables we'll be connecting to
double Setpoint_roll, Input_roll, Output_roll;

//Specify the links and initial tuning parameters for Roll PID
double Kp_roll=2, Ki_roll=5, Kd_roll=0;
PID Roll_PID(&Input_roll, &Output_roll, &Setpoint_roll, Kp_roll, Ki_roll, Kd_roll, DIRECT);

Adafruit_L3GD20 gyro; // Creating gyro element, by default it uses i2c no need for defining it
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321); // Unique ID for the accelerometer 

// Storage variables
unsigned long Time; // Remember unsigned long does not store negative numbers otherwise it is just a number, i think
float gyro_overall_angle_x;
float gyro_overall_angle_y;
float gyro_overall_angle_z;

void setup() {
  // put your setup code here, to run once:
  ailerons.attach(ailerons_pin);
  elevator.attach(elevator_pin);
  pinMode(relay_switch_pin, OUTPUT);
  digitalWrite(relay_switch_pin, LOW);
  
  Time = 0;
  
  Serial.begin(9600); // Begin serial
  Wire.begin(); // Required for slave arduino
  delay(1000); // Large delay for slave arduino to properly boot up
  
  // Initialise the accelerometer
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  Serial.println("Accelerometer initialized");
  
  // Initialise the gyro
  //if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
  //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
  if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
  Serial.println("Gyro initialized");

  Setpoint_roll = 0; // Setpoint for Roll PID control
  Roll_PID.SetMode(AUTOMATIC); // Mode for Roll PID control
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Store data values from the sensors first
  // Accelerometer
  sensors_event_t event;
  accel.getEvent(&event);
  float accel_x = event.acceleration.x;
  float accel_y = event.acceleration.y;
  float accel_z = event.acceleration.z;
  Serial.println("Accelerometer readings");
  Serial.println(accel_x);
  Serial.println(accel_y);
  Serial.println(accel_z);
  
  // Gyro
  gyro.read();
  float gyro_x = gyro.data.x;
  float gyro_y = gyro.data.y;
  float gyro_z = gyro.data.z;
  Serial.println("Gyro readings");
  Serial.println(gyro_x);
  Serial.println(gyro_y);
  Serial.println(gyro_z);
  
  unsigned long Time_since_last = millis() - Time; // Remember this value is in milliseconds
  Time = millis();
  
  // Update gyro angles, must be tested to see how it behaves, may increase overtime and become inaccurate
  gyro_overall_angle_x += gyro_x*Time_since_last/1000;
  Serial.print("gyro present angle x ");
  Serial.println(gyro_overall_angle_x);
  gyro_overall_angle_y += gyro_y*Time_since_last/1000;
  Serial.print("gyro present angle y ");
  Serial.println(gyro_overall_angle_y);
  gyro_overall_angle_z += gyro_z*Time_since_last/1000;
  Serial.print("gyro present angle z ");
  Serial.println(gyro_overall_angle_z);

  // Push data to slave arduino for storage first
  push_to_slave(accel_x, '0');
  push_to_slave(accel_y, '1');
  push_to_slave(accel_z, '2');
  push_to_slave(gyro_x, '3');
  push_to_slave(gyro_y, '4');
  push_to_slave(gyro_z, '5');

  Serial.print("Gear from receiver ADC value ");
  Serial.println(analogRead(gear_receiver_pin));
  
  if(analogRead(gear_receiver_pin) < 255/2){
    digitalWrite(relay_switch_pin, LOW);
    // Things to do in manual control in this section
    Serial.println("Manual control mode");
    
  }
  else{
    // Things to do in autopilot control in this section
    digitalWrite(relay_switch_pin, HIGH);
    Serial.println("Autopilot mode");
    ailerons.write(100);
    delay(100);
    ailerons.write(50);
    delay(100);
    // First stabilize yourself in Roll angle

  }
  delay(1000);
}

void push_to_slave(float value, char i){
  String value_string = (String)value;
  char value_chararray[((String)value).length()+1];
  
  for(int x = 0; x < ((String)value).length(); x++){
    value_chararray[x] = value_string[x];
  }
  value_chararray[((String)value).length()] = '\0';
  
  Serial.println(value_chararray);

  char id_chararray[2];
  id_chararray[0] = i;
  id_chararray[1] = '\0';
  
  Wire.beginTransmission(8); // Start transmitting to slave
  Wire.write(value_chararray);
  Wire.write(id_chararray);
  Wire.endTransmission(); // Stop transmitting
}

void level_flight_roll_PID(float accel_value){
  Input_roll = accel_value;
  Roll_PID.Compute();
  ailerons.write(Output_roll);
}

void level_flight_pitch_PID(){
  
}
