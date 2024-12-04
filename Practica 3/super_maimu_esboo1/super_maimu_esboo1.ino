#include <avr/wdt.h>

const int ledPins[] = {3, 5, 6, 9, 10}; // Pines de los LEDs
const int photoresistorPin = A0; // Pin de la fotoresistencia
const int potentiometerPin = A1; // Pin del potenciómetro

int lightLevel = 0; // Variable para leer la luz de la fotoresistencia
int potValue = 0; // Valor del potenciómetro
int pwmValue = 255; // Valor inicial PWM para los LEDs
unsigned long timeInterval = 8000; // Duración total de 8 segundos
unsigned long startTime;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    analogWrite(ledPins[i], 0); // Apaga los LEDs al inicio
  }
  pinMode(photoresistorPin, INPUT);
  pinMode(potentiometerPin, INPUT);
  
  wdt_disable(); // Deshabilitar el watchdog al inicio
}

void loop() {
  lightLevel = analogRead(photoresistorPin); // Leer el valor de la fotoresistencia

  if (lightLevel < 512) { // Solo inicia si no hay luz en la fotoresistencia
    startTime = millis(); // Guardar el tiempo de inicio del ciclo de apagado

    while (millis() - startTime <= timeInterval) { // Proceso de apagado gradual de LEDs durante 8 segundos
      // Leer el valor del potenciómetro
      potValue = analogRead(potentiometerPin); 
      int mappedPotValue = map(potValue, 0, 1023, 0, 255); // Ajustar el valor del potenciómetro al rango de PWM
      
      // Calcular el tiempo transcurrido y ajustar el brillo
      unsigned long elapsedTime = millis() - startTime;
      float fadeFactor = 1.0 - (float)elapsedTime / timeInterval;
      pwmValue = mappedPotValue * fadeFactor; // Ajustar el brillo según el potenciómetro y el tiempo

      // Aplicar el brillo a los LEDs
      for (int i = 0; i < 5; i++) {
        analogWrite(ledPins[i], pwmValue);
      }

      // Verificar si es tiempo de activar el watchdog
      if (elapsedTime >= timeInterval - 2000) { // Justo antes de apagarse (últimos 2000ms)
        wdt_enable(WDTO_1S); // Activar el watchdog con un retraso de 1 segundo para el reinicio
      }
    }
    
    // Reinicio del ciclo, el watchdog se activará y reiniciará el sistema
  }
}
