#include <SoftwareSerial.h>

void setup() {
    // Configura la velocidad de transmisión
    Serial.begin(9600); // Velocidad del Arduino
}

void loop() {
    // Envía un mensaje a través del puerto serial
    Serial.println("Hola desde Arduino");
    delay(1000); // Espera 1 segundo antes de enviar el siguiente mensaje
}
