#include <Arduino_FreeRTOS.h>
#include <semphr.h>
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

// Definiciones de FreeRTOS
SemaphoreHandle_t xSerialSemaphore;
QueueHandle_t xSensorDataQueue;

// Función de configuración
void setup() {
  // Inicialización de componentes
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_SERIES_PIN, OUTPUT);
  pinMode(LED_BLINK_PIN, OUTPUT);
  
  lcd.begin(16, 2);
  if (!bme.begin(0x76)) {
    lcd.print("BME280 error!");
    while (1);
  }

  Serial.begin(9600);

  // Crear semáforo y cola
  xSerialSemaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(xSerialSemaphore);
  xSensorDataQueue = xQueueCreate(10, sizeof(float)); // Incrementar capacidad de la cola

  // Crear tareas
  xTaskCreate(taskButtonLEDs, "ButtonLEDs", 128, NULL, 2, NULL);
  xTaskCreate(taskReadBME280, "ReadBME280", 128, NULL, 3, NULL);
  xTaskCreate(taskBlinkLED, "BlinkLED", 128, NULL, 1, NULL);

  vTaskStartScheduler(); // Inicia el sistema FreeRTOS
}

void loop() {
  // No se usa el loop en FreeRTOS
}

// Tarea 1: Leer botón y controlar LEDs en serie
void taskButtonLEDs(void *pvParameters) {
  for (;;) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      digitalWrite(LED_SERIES_PIN, HIGH);
    } else {
      digitalWrite(LED_SERIES_PIN, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Espera 100 ms
  }
}

// Tarea 2: Leer BME280 y enviar datos por serial y LCD
void taskReadBME280(void *pvParameters) {
  for (;;) {
    float temperature = bme.readTemperature();

    if (xSemaphoreTake(xSerialSemaphore, pdMS_TO_TICKS(100))) { // Tiempo de espera para evitar bloqueo
      Serial.print("Temp: ");
      Serial.println(temperature);
      xSemaphoreGive(xSerialSemaphore);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(6, 0);
    lcd.print(temperature, 1);
    lcd.print("C");

    // Evitar bloqueo indefinido al enviar a la cola
    if (xQueueSend(xSensorDataQueue, &temperature, pdMS_TO_TICKS(100)) != pdPASS) {
      Serial.println("Cola llena, datos descartados.");
    }

    vTaskDelay(pdMS_TO_TICKS(500)); // Espera 500 ms
  }
}

// Tarea 3: Parpadear LED
void taskBlinkLED(void *pvParameters) {
  for (;;) {
    digitalWrite(LED_BLINK_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(500));
    digitalWrite(LED_BLINK_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
