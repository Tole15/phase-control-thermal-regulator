#include <PWMUNO.h>S

int period = 8033; 
unsigned int dim;
const int pinLM35 = A0; 
float tempC = 0; 
int zeroCrossCounter = 0; 


void deteccion_Cruce_cero() {
  digitalWrite(13, HIGH); 
  PWM1.restart();
  PWM1.dutyCycleA(dim);
  digitalWrite(13, LOW); 

  zeroCrossCounter++;
  if (zeroCrossCounter >= 60) {
    leerTemp(); 
    displayInfo(); 
    zeroCrossCounter = 0; 
  }
}


void displayInfo() {
  Serial.print(tempC); 
  Serial.print(" grados Celsius"); 
}


float leerTemp() {
  const float mvC = 10.0;
  const float mvU = 1.07; 
  float sensorTemp = analogRead(pinLM35); 
  tempC = mvU * sensorTemp; 
  tempC /= mvC; 
  return tempC; 
}

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(13, OUTPUT); 
  attachInterrupt(0, deteccion_Cruce_cero, RISING);
  PWM1.setPeriod(period);
  dim = 0;
  analogReference(INTERNAL);
}

void loop() {

}

