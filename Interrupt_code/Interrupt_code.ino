volatile int GEAR = 0;

void setup() {
  attachInterrupt(2, INTgear, RISING);
  pinMode(13, OUTPUT);
}

void loop() {
  if (GEAR == 1){
    digitalWrite(13, HIGH);
  }

}
