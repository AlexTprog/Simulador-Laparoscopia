#include <SoftwareSerial.h>
#include <Servo.h>
Servo myservo;

#include <Ticker.h>
SoftwareSerial gas(10, 11);

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
SoftwareSerial pulso(3, 4);

SoftwareSerial temperatura(5, 6);

SoftwareSerial proximidad(9, 8);

//Variable global
float pot = -1;
int myBPM = 0;
int sensorTemp = 0;
int led = 12;
int buz = 7;

long duracion;
long distancia = 0;
int echo = 8;
int trig = 9;
int pos = 0;

//Variables pulso
const int PulseWire2 = 1;
const int LED13 = 13;
int Threshold = 550;

PulseSensorPlayground pulseSensor;

//Variables temperatura
int tempval;

//Codigo del sensor de Gas

void SensorDeGas() {
  int rango_dato = analogRead(A0);

  float voltaje = rango_dato * (5.0 / 1023.0);
  //Calculamos Rs con un RL de 1k
  float Rs = 1000 * ((5 - voltaje) / voltaje);
  // calculamos la concentración  de alcohol con la ecuación obtenida.
  double alcohol = 0.4091 * pow(Rs / 5463, -1.497);

  if (pot != alcohol) {
    pot = alcohol;
    gas.println(pot);
  }
}

void fnDistancia() {
  //Para estabilizar nuestro módulo ultrasónico
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  //disparo de un pulso en el trigger de longitud 10us
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  //Lectura de la duración del pulso HIGH generado hasta recibir el Echo
  duracion = pulseIn(echo, HIGH);
  //Calculo distancia


  if (distancia > 300 && distancia <= 400) {
    digitalWrite(led, HIGH);
  } else if (distancia > 400) {
    digitalWrite(led, LOW);
  } else if (distancia > 380) {
    pinMode(buz, HIGH);
  }

  if (distancia != duracion / 100) {
    distancia = duracion / 100;  // (cm) //58.4//100 para elegir nuestro rango
    proximidad.println(distancia);
  }
}

void SensorPulso() {
  int bpm = pulseSensor.getBeatsPerMinute();
  pulseSensor.sawStartOfBeat();
  if (myBPM != bpm) {
    myBPM = bpm;
    pulso.println(myBPM);
  }
}

//Codigo del sensor de Temperatura
void SensorTemperatura() {
  tempval = analogRead(A2);
  float mv = (tempval / 1024.0) * 5000;
  float temp = mv / 10;
  if (sensorTemp != temp) {
    sensorTemp = temp;
    temperatura.println(sensorTemp);
  }
}

void motor() {
  int val = analogRead(3);
  val = map(val, 0, 1023, 0, 180);
  myservo.write(val);
}

Ticker ticDistancia(fnDistancia, 500);  //Funcion, tiempo
Ticker ticGas(SensorDeGas, 500);
Ticker ticPulso(SensorPulso, 500);
Ticker ticTemperatura(SensorTemperatura, 500);
Ticker ticMotor(motor, 300);

double EnviarGas;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buz, OUTPUT);
  /**********************
   * Sensor de proximidad
   *********************/
  Serial.begin(9600);
  pinMode(trig, OUTPUT);  // emisor
  pinMode(echo, INPUT);   // Receptor
  myservo.attach(9);
  /**********************
   * Sensor de proximidad
   *********************/
  gas.begin(9600);
  ticDistancia.start();

  ticGas.start();
  /**********************
   * Sensor de pulso
   *********************/
  pulso.begin(9600);
  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire2);
  //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();
  ticPulso.start();

  //Sensor de pulso
  temperatura.begin(9600);
  ticTemperatura.start();

  ticMotor.start();
}

void loop() {
  ticDistancia.update();
  ticGas.update();
  ticPulso.update();
  ticTemperatura.update();
  ticMotor.update();
}
