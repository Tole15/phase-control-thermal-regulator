#include <PWMUNO.h>
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

// Zero-cross por media onda: 120 Hz  -> 8.333 ms = 8333 us
int PRD = 8333;

unsigned POT;
volatile unsigned int N = 0;
volatile unsigned int CMP = 0;

// Ts = 0.5  60s a 120 Hz
float Ts = 0.5f, Fclk = 0.5e6f;

float Kp = 3.16999550313119f, Ki = 8.0547951106804f, r = 40.0f;

float Vrms = 120.0f, T = 1.0f / 60.0f, omega = 2.0f * M_PI * 60.0f;
float Pmax = 60.0f, R = (Vrms * Vrms) / Pmax;

float e = 0.0f, e_1 = 0.0f, u = 0.0f, u_1 = 0.0f;
float ton = 0.0f, toff = 0.0f, y = 0.0f;

static inline unsigned int clampICR1(long v) {
  if (v < 0) return 0;
  if (v > (long)ICR1) return ICR1;
  return (unsigned int)v;
}

void setup() {
  Serial.begin(9600);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);

  attachInterrupt(0, deteccion_Cruce_cero, RISING);
  PWM1.setPeriod(PRD);

  // Inicial: 50% de potencia máxima
  u = 0.5f * Pmax;
  u_1 = u;

  ton = T / (2.0f * Pmax) * u;
  toff = T - ton;
  CMP = clampICR1((long)(Fclk * toff));

  Serial.print("CMP=");
  Serial.println(CMP);
  Serial.print("ICR1=");
  Serial.println(ICR1);
}

void deteccion_Cruce_cero() {
  digitalWrite(13, HIGH);

  PWM1.restart();
  PWM1.dutyCycleA(CMP);

  N++;
  digitalWrite(13, LOW);
}

void loop() {

  if (N > 59) {
    POT = analogRead(A0);


    Serial.print("y(temperatura)=");
    Serial.println(y);

    e = r - y;

    // PI discreto (forma recursiva)
    u = (Ki * Ts / 2.0f + Kp) * e + (Ki * Ts / 2.0f - Kp) * e_1 + u_1;

    // Saturación
    if (u > Pmax) u = Pmax;
    if (u < 0.0f) u = 0.0f;

    ton = T / (2.0f * Pmax) * u;
    toff = T - ton;

    CMP = clampICR1((long)(Fclk * toff));

    u_1 = u;
    e_1 = e;
    N = 0;

    delay(1);
  }
}

