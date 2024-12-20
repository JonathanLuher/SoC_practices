#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

// Inicializar instancias
Adafruit_BME280 bme; // Sensor BME280
Adafruit_ADS1115 ads; // Convertidor ADS1115

// Pines LCD (ajusta si es necesario)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

// Variables globales
bool bmeConectado = false; // Bandera para validar si el BME280 está conectado
float voltaje;             // Voltaje leído del potenciómetro
float temperatura;         // Temperatura del BME280
float presion;             // Presión del BME280

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);  // Inicializa LCD de 16x2
  lcd.print("Iniciando...");
  
  // Inicializar BME280
  if (bme.begin(0x76)) {
    bmeConectado = true;
    Serial.println("BME280 conectado correctamente.");
  } else {
    bmeConectado = false;
    Serial.println("Advertencia: BME280 no conectado.");
  }

  // Inicializar ADS1115
  ads.setGain(GAIN_TWOTHIRDS); // Ganancia para rango ±6.144V
  ads.begin();
  Serial.println("ADS1115 inicializado correctamente.");

  delay(2000); // Pausa para mostrar mensajes iniciales
  lcd.clear();
}

void loop() {
  // Leer datos del potenciómetro
  int16_t valorPot = ads.readADC_SingleEnded(0); // Canal A0
  voltaje = (valorPot * 6.144) / 32767.0; // Conversión a voltaje

  // Leer datos del BME280 si está conectado
  if (bmeConectado) {
    temperatura = bme.readTemperature();
    presion = bme.readPressure();
  } else {
    temperatura = 0.0; // Valores por defecto si no está conectado
    presion = 0.0;
  }

  // Mostrar datos en el LCD
  lcd.setCursor(0, 0); // Línea superior
  lcd.print("T:");
  lcd.print(temperatura, 1);
  lcd.print("C,");
  lcd.print(presion, 0);
  lcd.print("Pa");
  
  lcd.setCursor(0, 1); // Línea inferior
  lcd.print("V:");
  lcd.print(voltaje, 2);
  lcd.print("V   ");

  // Mostrar datos en el monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C, Presión: ");
  Serial.print(presion);
  Serial.print(" Pa, Voltaje: ");
  Serial.print(voltaje, 2);
  Serial.println(" V");

  delay(1000); // Pausa de 1 segundo
}
