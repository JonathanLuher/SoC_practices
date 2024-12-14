// Arduino Receptor
int ledsReceptor[4] = {2, 3, 4, 5}; // Pines de LEDs para el receptor
int numeroReceptor;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledsReceptor[i], OUTPUT); // Configura los LEDs del receptor
  }
  randomSeed(analogRead(0)); // Para generar números aleatorios
}

void loop() {
  // Espera el número enviado por el emisor
  if (Serial.available() > 0) {
    int numeroRecibido = Serial.read(); // Lee el número enviado por el emisor

    // Muestra el número recibido en binario usando los LEDs del receptor
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsReceptor[i], (numeroRecibido >> i) & 0x01); // Ajusta cada LED
    }

    delay(1000); // Mantiene los LEDs encendidos durante 1 segundo

    // Apaga los LEDs después de mostrar el número
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsReceptor[i], LOW);
    }

    numeroReceptor = random(1, 16); // Genera un número aleatorio entre 1 y 15
    Serial.write(numeroReceptor); // Envía el número al emisor

    // Muestra el número en binario usando los LEDs del receptor
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsReceptor[i], (numeroReceptor >> i) & 0x01); // Ajusta cada LED
    }

    delay(1000); // Muestra el número enviado durante 1 segundo

    // Apaga los LEDs del receptor
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsReceptor[i], LOW);
    }
  }

  delay(1000); // Espera 1 segundo antes de procesar el siguiente número
}
