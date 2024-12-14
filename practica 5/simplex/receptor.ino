const int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};  // Pines para los segmentos a-g

// Mapeo de segmentos para cada dígito (0-9)
// Cada número es un arreglo de 7 elementos que representa el estado de cada segmento
const bool digits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1}   // 9
};

void setup() {
  Serial.begin(9600);  // Inicializa la comunicación UART
  
  // Configura los pines de los segmentos como salida
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);  // Apaga todos los segmentos al inicio
  }
}

void loop() {
  if (Serial.available() > 0) {             // Verifica si hay datos disponibles
    int receivedNumber = Serial.read();      // Lee el número recibido (0-9)
    if (receivedNumber >= 0 && receivedNumber <= 9) {  // Verifica si el número está en el rango
      displayDigit(receivedNumber);          // Muestra el dígito en el display
    }
  }
}

// Función para mostrar un dígito en el display de 7 segmentos
void displayDigit(int digit) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[digit][i]);  // Activa o desactiva cada segmento
  }
}
