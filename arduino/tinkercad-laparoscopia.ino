int TEMPERATURA = 0; // PIN A0
int GAS = 1;         // PIN A1
int PULSO = 2;       // PIN A2

float gas = 0;
float tmp = 0;
int cm = 0;
int ps = 0;

long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT); // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void showTmp() {
  tmp = map(analogRead(TEMPERATURA), 0, 1023, -50, 450);
  Serial.print(tmp);
  Serial.println(" C");
  delay(100);
}

void showGas() {
  gas = analogRead(GAS);
  Serial.print(gas);
  Serial.println("Co2");
  delay(100);
}

void setup() { Serial.begin(9600); }

void showDistance() {
  cm = 0.01723 * readUltrasonicDistance(7, 7);
  Serial.print(cm);
  Serial.println("cm");
  delay(100); // Wait for 100 millisecond(s)
}

void showPulso() {
  ps = analogRead(PULSO);
  Serial.print(ps);
  Serial.println("bt");
  delay(100); // Wait for 100 millisecond(s)
}

void loop() {
  showPulso();
  showTmp();
  showDistance();
  showGas();
}