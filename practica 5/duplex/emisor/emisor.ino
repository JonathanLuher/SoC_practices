// Arduino Emisor
int ledsEmisor[4] = {2, 3, 4, 5}; // Pines de LEDs para el emisor
int numeroEmisor;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledsEmisor[i], OUTPUT); // Configura los LEDs del emisor
  }
  randomSeed(analogRead(0)); // Para generar números aleatorios
}

void loop() {
  numeroEmisor = random(1, 16); // Genera un número aleatorio entre 1 y 15
  Serial.write(numeroEmisor); // Envía el número al receptor

  // Muestra el número en binario usando los LEDs del emisor
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledsEmisor[i], (numeroEmisor >> i) & 0x01); // Ajusta cada LED
  }

  delay(1000); // Mantiene los LEDs encendidos durante 1 segundo

  // Apaga los LEDs después de mostrar el número
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledsEmisor[i], LOW);
  }

  // Espera por el número del receptor
  if (Serial.available() > 0) {
    int numeroRecibido = Serial.read(); // Lee el número enviado por el receptor

    // Muestra el número recibido en binario usando los LEDs del emisor
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsEmisor[i], (numeroRecibido >> i) & 0x01);
    }

    delay(1000); // Muestra el número recibido durante 1 segundo

    // Apaga los LEDs del emisor
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsEmisor[i], LOW);
    }
  }

  delay(1000); // Espera 1 segundo antes de enviar el siguiente número
}
