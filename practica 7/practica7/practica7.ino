#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 7
LiquidCrystal lcd(8, 9, 5, 4, 3, 2);

// Instancia del lector RFID
MFRC522 rfid(SS_PIN, RST_PIN);

// UID autorizado (73 B8 96 A1)
byte uidAutorizado[] = {0xE3, 0xF3, 0x19, 0x14};

void setup() {
  lcd.begin(16, 2); // Configuración del LCD de 16x2
  lcd.print("CERRADO"); // Mensaje inicial

  // Inicialización del Serial
  Serial.begin(9600);

  // Inicialización del SPI y el lector RFID
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Lector listo.");
}

void loop() {
  // Verifica si hay una nueva tarjeta
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Comparar el UID leído con el UID autorizado
  if (verificarUID()) {
    // Tarjeta autorizada
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BIENVENIDO");
    lcd.setCursor(0, 1);
    lcd.print("Acceso autorizado");

    Serial.println("Acceso autorizado.");
  } else {
    // Tarjeta no autorizada
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fuera!");
    lcd.setCursor(0, 1);
    lcd.print("Acceso denegado");

    Serial.println("Acceso denegado.");
  }

  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CERRADO");

  // Detiene la comunicación con la tarjeta
  rfid.PICC_HaltA();
}

// Función para verificar si el UID leído coincide con el autorizado
bool verificarUID() {
  if (rfid.uid.size != sizeof(uidAutorizado)) return false;

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != uidAutorizado[i]) return false;
  }

  return true;
}
