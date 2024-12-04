// Pines para los segmentos del primer display (unidades)
int segA_unidades = 2;
int segB_unidades = 3;
int segC_unidades = 4;
int segD_unidades = 5;
int segE_unidades = 6;
int segF_unidades = 7;
int segG_unidades = 8;

// Pines para los segmentos del segundo display (decenas)
int segA_decenas = 9;
int segB_decenas = 10;
int segC_decenas = 11;
int segD_decenas = 12;
int segE_decenas = 13;
int segF_decenas = A0;  // Usando A0 como pin digital
int segG_decenas = A1;  // Usando A1 como pin digital

// Pin para la lectura del potenciómetro
int potPin = A2;  // Pin donde conectas el potenciómetro

// Array que contiene los segmentos para los números 0-9
int numeros[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 0, 0, 1, 1}  // 9
};

// Inicialización de los pines
void setup() {
  // Configurar pines de los segmentos de unidades como salidas
  pinMode(segA_unidades, OUTPUT);
  pinMode(segB_unidades, OUTPUT);
  pinMode(segC_unidades, OUTPUT);
  pinMode(segD_unidades, OUTPUT);
  pinMode(segE_unidades, OUTPUT);
  pinMode(segF_unidades, OUTPUT);
  pinMode(segG_unidades, OUTPUT);

  // Configurar pines de los segmentos de decenas como salidas
  pinMode(segA_decenas, OUTPUT);
  pinMode(segB_decenas, OUTPUT);
  pinMode(segC_decenas, OUTPUT);
  pinMode(segD_decenas, OUTPUT);
  pinMode(segE_decenas, OUTPUT);
  pinMode(segF_decenas, OUTPUT);
  pinMode(segG_decenas, OUTPUT);
}

// Función para mostrar un número en el display de unidades
void mostrarUnidades(int numero) {
  digitalWrite(segA_unidades, numeros[numero][0]);
  digitalWrite(segB_unidades, numeros[numero][1]);
  digitalWrite(segC_unidades, numeros[numero][2]);
  digitalWrite(segD_unidades, numeros[numero][3]);
  digitalWrite(segE_unidades, numeros[numero][4]);
  digitalWrite(segF_unidades, numeros[numero][5]);
  digitalWrite(segG_unidades, numeros[numero][6]);
}

// Función para mostrar un número en el display de decenas
void mostrarDecenas(int numero) {
  digitalWrite(segA_decenas, numeros[numero][0]);
  digitalWrite(segB_decenas, numeros[numero][1]);
  digitalWrite(segC_decenas, numeros[numero][2]);
  digitalWrite(segD_decenas, numeros[numero][3]);
  digitalWrite(segE_decenas, numeros[numero][4]);
  digitalWrite(segF_decenas, numeros[numero][5]);
  digitalWrite(segG_decenas, numeros[numero][6]);
}

void loop() {
  // Leer el valor del potenciómetro (0-1023)
  int valorPot = analogRead(potPin);

  // Mapear el valor del potenciómetro de 0-1023 a 0-10 (número máximo 10)
  int valorEscalado = map(valorPot, 0, 1023, 0, 10);

  // Calcular decenas y unidades
  int decenas = valorEscalado / 10;
  int unidades = valorEscalado % 10;

  // Mostrar las unidades en el primer display
  mostrarUnidades(unidades);

  // Mostrar las decenas en el segundo display
  mostrarDecenas(decenas);

  // Pequeña pausa
  delay(10);
}
