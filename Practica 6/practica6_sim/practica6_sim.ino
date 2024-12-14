#include <SPI.h>
#include <LiquidCrystal.h>

// Pines LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Pines de selección de esclavo para los sensores
const int CSB_PIN1 = 10; 
const int CSB_PIN2 = 9;

// Variables para los coeficientes de calibración
uint16_t dig_T1_1, dig_T1_2;
int16_t dig_T2_1, dig_T2_2;
int16_t dig_T3_1, dig_T3_2;
uint32_t t_fine_1, t_fine_2;

void setup() {
  // Configurar LCD
  lcd.begin(16, 2);
  lcd.print("Iniciando...");

  // Configurar pines de esclavo y comunicación SPI
  SPI.begin();
  pinMode(CSB_PIN1, OUTPUT);
  pinMode(CSB_PIN2, OUTPUT);
  digitalWrite(CSB_PIN1, HIGH);
  digitalWrite(CSB_PIN2, HIGH);

  Serial.begin(9600);

  // Inicializar sensores
  initBMP280(CSB_PIN1);
  initBMP280(CSB_PIN2);
}

void loop() {
  // Leer datos de ambos sensores
  float temperatura1 = readTemperature(CSB_PIN1, 1);
  float presion1 = readPressure(CSB_PIN1, 1);
  float temperatura2 = readTemperature(CSB_PIN2, 2);
  float presion2 = readPressure(CSB_PIN2, 2);

  // Mostrar datos en el LCD
  lcd.clear();

  // Datos Sensor 1 en la línea superior
  lcd.setCursor(0, 0);
  lcd.print("S1 T:");
  lcd.print(temperatura1, 1);
  lcd.print("C");

  lcd.setCursor(9, 0);
  lcd.print("P:");
  lcd.print(presion1 / 100, 1); // Presión en hPa

  // Datos Sensor 2 en la línea inferior
  lcd.setCursor(0, 1);
  lcd.print("S2 T:");
  lcd.print(temperatura2, 1);
  lcd.print("C");

  lcd.setCursor(9, 1);
  lcd.print("P:");
  lcd.print(presion2 / 100, 1); // Presión en hPa

  delay(1000);
}

void initBMP280(int csbPin) {
  digitalWrite(csbPin, LOW);
  writeRegister(csbPin, 0xF4, 0x27); // Configurar control (modo normal, oversampling)
  writeRegister(csbPin, 0xF5, 0xA0); // Configurar tiempo de espera y filtro

  // Leer coeficientes de calibración
  dig_T1_1 = readRegister16(csbPin, 0x88);
  dig_T2_1 = readRegister16Signed(csbPin, 0x8A);
  dig_T3_1 = readRegister16Signed(csbPin, 0x8C);

  dig_T1_2 = dig_T1_1; // Usar mismas calibraciones para ambos sensores (ajustar si necesario)
  dig_T2_2 = dig_T2_1;
  dig_T3_2 = dig_T3_1;

  digitalWrite(csbPin, HIGH);
}

float readTemperature(int csbPin, int sensor) {
  digitalWrite(csbPin, LOW);
  int32_t rawTemp = ((uint32_t)readRegister(csbPin, 0xFA) << 12) |
                    ((uint32_t)readRegister(csbPin, 0xFB) << 4) |
                    ((readRegister(csbPin, 0xFC) >> 4));
  digitalWrite(csbPin, HIGH);

  int32_t var1, var2;
  if (sensor == 1) {
    var1 = ((((rawTemp >> 3) - ((int32_t)dig_T1_1 << 1))) * ((int32_t)dig_T2_1)) >> 11;
    var2 = (((((rawTemp >> 4) - ((int32_t)dig_T1_1)) * ((rawTemp >> 4) - ((int32_t)dig_T1_1))) >> 12) * ((int32_t)dig_T3_1)) >> 14;
    t_fine_1 = var1 + var2;
    return ((t_fine_1 * 5 + 128) >> 8) / 100.0;
  } else {
    var1 = ((((rawTemp >> 3) - ((int32_t)dig_T1_2 << 1))) * ((int32_t)dig_T2_2)) >> 11;
    var2 = (((((rawTemp >> 4) - ((int32_t)dig_T1_2)) * ((rawTemp >> 4) - ((int32_t)dig_T1_2))) >> 12) * ((int32_t)dig_T3_2)) >> 14;
    t_fine_2 = var1 + var2;
    return ((t_fine_2 * 5 + 128) >> 8) / 100.0;
  }
}

float readPressure(int csbPin, int sensor) {
  digitalWrite(csbPin, LOW);
  int32_t rawPress = ((uint32_t)readRegister(csbPin, 0xF7) << 12) |
                     ((uint32_t)readRegister(csbPin, 0xF8) << 4) |
                     ((readRegister(csbPin, 0xF9) >> 4));
  digitalWrite(csbPin, HIGH);

  return rawPress / 256.0; // Ajustar según la conversión del sensor específico
}

void writeRegister(int csbPin, byte reg, byte value) {
  digitalWrite(csbPin, LOW);
  SPI.transfer(reg & 0x7F); // Escribir en el registro
  SPI.transfer(value);
  digitalWrite(csbPin, HIGH);
}

uint16_t readRegister16(int csbPin, byte reg) {
  return (readRegister(csbPin, reg) | (readRegister(csbPin, reg + 1) << 8));
}

int16_t readRegister16Signed(int csbPin, byte reg) {
  return (int16_t)readRegister16(csbPin, reg);
}

byte readRegister(int csbPin, byte reg) {
  digitalWrite(csbPin, LOW);
  SPI.transfer(reg | 0x80); // Leer del registro
  byte result = SPI.transfer(0x00);
  digitalWrite(csbPin, HIGH);
  return result;
}
