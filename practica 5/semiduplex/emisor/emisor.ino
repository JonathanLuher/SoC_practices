// Arduino Emisor
int ledRecepcion = 7;
int numeroEnviado;

void setup() {
  Serial.begin(9600);
  pinMode(ledRecepcion, OUTPUT);
  randomSeed(analogRead(0)); // Para generar números aleatorios
}

void loop() {
  numeroEnviado = random(0, 16); // Genera número entre 0 y 15
  Serial.write(numeroEnviado); // Envía el número al receptor
  delay(1000); // Pequeña pausa para asegurar la transmisión
  
  // Espera la confirmación de recepción
  if (Serial.available() > 0) {
    int confirmacion = Serial.read();
    if (confirmacion == 1) {
      digitalWrite(ledRecepcion, HIGH); // Enciende el LED de recepción
      delay(1000); // Mantén encendido el LED durante 1 segundo
      digitalWrite(ledRecepcion, LOW); // Apaga el LED
    }
  }
  
  delay(1000); // Pausa antes de enviar el siguiente número
}
