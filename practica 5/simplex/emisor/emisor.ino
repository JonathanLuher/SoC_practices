void setup() {
  Serial.begin(9600);  // Inicializa la comunicación UART
}

void loop() {
  int numberToSend = random(0, 10);   // Genera un número aleatorio entre 0 y 9
  Serial.write(numberToSend);         // Envía el número
  delay(1000);                        // Espera 1 segundo antes de enviar otro número
}
