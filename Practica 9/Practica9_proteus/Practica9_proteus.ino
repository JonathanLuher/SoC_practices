#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal.h>

// Definiciones de hardware
#define BUTTON_PIN 9
#define LED_SERIES_PIN 8
#define LED_BLINK_PIN 10
#define LCD_RS 7
#define LCD_EN 6
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

// Objeto para el sensor BME280 y LCD
Adafruit_BME280 bme;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Variables para control de tareas
unsigned long previousMillisButton = 0;  // Para el botón con anti-rebote
unsigned long previousMillisBlink = 0;   // Para el parpadeo del LED
const long intervalBlink = 500;           // Intervalo de parpadeo (500ms)
const long debounceDelay = 50;           // Tiempo de anti-rebote del botón (50ms)
bool ledStateBlink = LOW;                // Estado del LED que parpadea
bool ledStateButton = LOW;               // Estado del LED en serie

void setup() {
  // Inicialización de componentes
  pinMode(BUTTON_PIN, INPUT);            // Configura el pin con resistencia pull-down
  pinMode(LED_SERIES_PIN, OUTPUT);
  pinMode(LED_BLINK_PIN, OUTPUT);

  lcd.begin(16, 2);
  if (!bme.begin(0x76)) {
    lcd.print("BME280 error!");
    while (1);  // Detener si no se encuentra el sensor
  }

  Serial.begin(9600);
}

void loop() {
  // Leer el estado del botón con anti-rebote
  unsigned long currentMillis = millis();
  int buttonState = digitalRead(BUTTON_PIN);

  // Verificar el anti-rebote del botón
  if (currentMillis - previousMillisButton > debounceDelay) {
    if (buttonState == HIGH) {
      ledStateButton = HIGH;  // Enciende el LED si el botón está presionado
    } else {
      ledStateButton = LOW;   // Apaga el LED si el botón no está presionado
    }
    previousMillisButton = currentMillis;
  }

  // Control del LED en serie según el estado del botón
  digitalWrite(LED_SERIES_PIN, ledStateButton);

  // Parpadeo del LED
  if (currentMillis - previousMillisBlink >= intervalBlink) {
    previousMillisBlink = currentMillis;
    ledStateBlink = !ledStateBlink;  // Cambia el estado del LED
    digitalWrite(LED_BLINK_PIN, ledStateBlink);  // Actualiza el estado del LED de parpadeo
  }

  // Leer la temperatura del BME280
  float temperature = bme.readTemperature();

  // Enviar la temperatura por serial
  Serial.print("Temp: ");
  Serial.println(temperature);

  // Mostrar la temperatura en el LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(6, 0);
  lcd.print(temperature, 1);
  lcd.print("C");

  // Espera para que el LCD se actualice y para leer nuevamente el sensor
  delay(100);  // Espera 500 ms para leer el sensor de nuevo
}
