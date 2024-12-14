// Arduino Receptor
int leds[4] = {2, 3, 4, 5}; // Pines para representar en binario

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT); // Configura los LEDs como salida
  }
}

void loop() {
  if (Serial.available() > 0) {
    int numeroRecibido = Serial.read(); // Lee el número enviado por el emisor

    // Muestra el número en binario usando los LEDs
    for (int i = 0; i < 4; i++) {
      digitalWrite(leds[i], (numeroRecibido >> i) & 0x01); // Ajusta cada LED
    }
    
    delay(1000); // Mantiene el número mostrado durante 1 segundo

    // Apaga los LEDs después de un segundo
    for (int i = 0; i < 4; i++) {
      digitalWrite(leds[i], LOW);
    }
    
    delay(1000); // Pausa breve antes de enviar la confirmación

    Serial.write(1); // Envía confirmación de recepción al emisor
  }
}
