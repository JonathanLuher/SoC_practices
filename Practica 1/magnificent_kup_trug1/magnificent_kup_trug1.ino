// Definición de pines
const int ledVerde = 8;    // Pin del LED verde
const int ledAmbar = 9;     // Pin del LED ámbar
const int ledRojo = 10;     // Pin del LED rojo
const int boton = 7;        // Pin del botón

// Variables de tiempo
unsigned long tiempoAnterior = 0;
unsigned long intervaloVerde = 3000;  // 3 segundos en milisegundos
unsigned long intervaloAmbar = 3000;  // 1 segundo en milisegundos
unsigned long intervaloRojo = 3000;   // 3 segundos en milisegundos
unsigned long intervaloRojoBoton = 4000; // 4 segundos en milisegundos

// Variables para el control del estado del semáforo
int estadoActual = 0;        // 0: Verde, 1: Ámbar, 2: Rojo
bool botonPresionado = false;
unsigned long tiempoInicioBoton = 0;

void setup() {
  // Configuración de los pines como salidas
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmbar, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(boton, INPUT_PULLUP); // Botón con resistencia pull-up interna

  // Inicialización de LEDs (apagados al inicio)
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmbar, LOW);
  digitalWrite(ledRojo, LOW);
}

void loop() {
  // Verificar si el botón fue presionado
  if (digitalRead(boton) == LOW && !botonPresionado) {
    botonPresionado = true;  // Marcamos que el botón fue presionado
    tiempoInicioBoton = millis();  // Guardamos el tiempo de inicio
  }

  if (botonPresionado) {
    // Si el botón fue presionado, el LED rojo se enciende durante 4 segundos
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmbar, LOW);
    digitalWrite(ledRojo, HIGH);

    // Verificar si han pasado los 40 segundos
    if (millis() - tiempoInicioBoton >= intervaloRojoBoton) {
      botonPresionado = false;  // Termina el modo del botón y vuelve al ciclo normal
      tiempoAnterior = millis(); // Reiniciamos el tiempo del semáforo normal
      estadoActual = 0;          // Reiniciamos al estado verde
    }
  } else {
    // Control normal del semáforo (si el botón no fue presionado)
    unsigned long tiempoActual = millis();

    switch (estadoActual) {
      case 0:  // Estado verde
        digitalWrite(ledVerde, HIGH);
        digitalWrite(ledAmbar, LOW);
        digitalWrite(ledRojo, LOW);
        if (tiempoActual - tiempoAnterior >= intervaloVerde) {
          estadoActual = 1;  // Cambiamos al estado ámbar
          tiempoAnterior = tiempoActual;
        }
        break;

      case 1:  // Estado ámbar
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledAmbar, HIGH);
        digitalWrite(ledRojo, LOW);
        if (tiempoActual - tiempoAnterior >= intervaloAmbar) {
          estadoActual = 2;  // Cambiamos al estado rojo
          tiempoAnterior = tiempoActual;
        }
        break;

      case 2:  // Estado rojo
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledAmbar, LOW);
        digitalWrite(ledRojo, HIGH);
        if (tiempoActual - tiempoAnterior >= intervaloRojo) {
          estadoActual = 0;  // Regresamos al estado verde
          tiempoAnterior = tiempoActual;
        }
        break;
    }
  }
}
