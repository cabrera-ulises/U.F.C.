/*
 * ════════════════════════════════════════════════════════════════════
 * ESCUDERÍA U.F.C. — Vehículo: Dagestan
 * Proyecto Informático 1 | Prof. Cynthia Berea | 2026
 * Ingenieros: Ulises Cabrera, Fabricio Crespo, Clovis Clemencot
 * ────────────────────────────────────────────────────────────────────
 * Hardware:
 *   - Arduino UNO R3 (ATmega328P, 16 MHz, 5V TTL)
 *   - Motor Shield L293D (4 puentes H independientes)
 *   - Módulo Bluetooth HC-06 (UART, modo Esclavo, 9600 bps)
 *   - 4x Motores DC reductores amarillos (3–6V, 200 mA típico)
 *   - Portapilas 4xAA (6V, fuente independiente de motores)
 * ────────────────────────────────────────────────────────────────────
 * Comandos Bluetooth (carácter ASCII):
 *   F → Avanzar      B → Retroceder
 *   L → Girar izq.   R → Girar der.
 *   S → Detener
 * ────────────────────────────────────────────────────────────────────
 * Restricciones pedagógicas (cátedra Prof. Berea):
 *   - NO se utiliza switch/case en ninguna parte del sketch.
 *     El despacho de comandos se realiza con if/else if/else.
 *   - NO se utiliza std::find ni ninguna función automática
 *     de búsqueda en cadenas. La búsqueda es indexada y manual.
 * ════════════════════════════════════════════════════════════════════
 */

// ── Librería del Motor Shield L293D ─────────────────────────────────
#include <AFMotor.h>

// ── Instanciación de los cuatro motores ──────────────────────────────
// El shield L293D numera sus canales del 1 al 4.
AF_DCMotor motorDelanteroDerecho(1);    // Canal M1 — rueda delantera der.
AF_DCMotor motorDelanteroIzquierdo(2);  // Canal M2 — rueda delantera izq.
AF_DCMotor motorTraseroDerecho(3);      // Canal M3 — rueda trasera der.
AF_DCMotor motorTraseroIzquierdo(4);    // Canal M4 — rueda trasera izq.

// ── Constante de velocidad (rango 0–255, escala PWM) ────────────────
const int VELOCIDAD = 180;

// ── Buffer de recepción serial (Bluetooth HC-06) ─────────────────────
// TAM_BUFFER: capacidad máxima del arreglo incluyendo el '\0' terminal.
const int TAM_BUFFER = 16;
char bufferSerial[TAM_BUFFER];
int indiceBuf = 0;

// ── Prototipos de funciones ──────────────────────────────────────────
void avanzar();
void retroceder();
void girarIzquierda();
void girarDerecha();
void detener();
char leerComandoDelBuffer();

// ════════════════════════════════════════════════════════════════════
// setup()
// Inicializa el puerto serie, configura velocidades de los motores
// y establece el estado inicial detenido del vehículo.
// ════════════════════════════════════════════════════════════════════
void setup() {
  // Inicialización del puerto serie (compartido con HC-06 vía D0/D1).
  // ATENCIÓN: se debe desconectar el HC-06 de los pines D0 y D1 antes
  // de cargar el sketch por cable USB para evitar conflictos de UART.
  Serial.begin(9600);

  // Configuración de velocidad PWM para los cuatro canales del shield.
  motorDelanteroDerecho.setSpeed(VELOCIDAD);
  motorDelanteroIzquierdo.setSpeed(VELOCIDAD);
  motorTraseroDerecho.setSpeed(VELOCIDAD);
  motorTraseroIzquierdo.setSpeed(VELOCIDAD);

  // Estado inicial: todos los motores en reposo (freno activo RELEASE).
  detener();

  // Inicialización defensiva del buffer: se garantiza que el arreglo
  // empieza limpio desde el primer ciclo del loop().
  bufferSerial[0] = '\0';
}

// ════════════════════════════════════════════════════════════════════
// loop()
// Ciclo principal: acumula bytes del UART en el buffer, detecta el
// terminador de trama, extrae el comando y ejecuta la acción.
// ════════════════════════════════════════════════════════════════════
void loop() {

  // ── Lectura del buffer serie ──────────────────────────────────────
  // Se leen todos los bytes disponibles en el UART. Los caracteres se
  // acumulan en bufferSerial hasta recibir un terminador de línea
  // ('\n' o '\r') o hasta alcanzar el límite del buffer.
  while (Serial.available() > 0) {
    char byteRecibido = (char)Serial.read();

    if (byteRecibido == '\n' || byteRecibido == '\r') {
      // ── Terminador de trama detectado ────────────────────────────
      if (indiceBuf > 0) {
        // Cerrar la cadena con el carácter nulo para delimitarla.
        bufferSerial[indiceBuf] = '\0';

        // Reiniciar el índice para la próxima trama.
        indiceBuf = 0;

        // CORRECCIÓN CRÍTICA (Optimización de firmware):
        // Aunque el índice se reinició a 0, la posición [0] del arreglo
        // todavía conserva el primer carácter del comando anterior.
        // En el siguiente ciclo, si no llegan bytes nuevos antes de que
        // leerComandoDelBuffer() sea invocada, la función leería un
        // comando fantasma residual de la trama previa.
        // Para evitar este comportamiento, se invalida explícitamente
        // el contenido visible del buffer colocando '\0' en la posición
        // inicial: la función de lectura interpretará la cadena como
        // vacía y retornará '\0' de forma segura.
        bufferSerial[0] = '\0';
      }
    } else {
      // ── Acumulación de carácter ───────────────────────────────────
      // Solo se almacena si hay espacio libre antes del '\0' terminal.
      if (indiceBuf < TAM_BUFFER - 1) {
        bufferSerial[indiceBuf] = byteRecibido;
        indiceBuf++;
      }
      // Si el buffer está lleno se descartan los bytes excedentes para
      // evitar desbordamiento. El índice no se incrementa.
    }
  }

  // ── Extracción manual del comando del buffer ──────────────────────
  // La búsqueda se realiza de forma indexada, carácter por carácter,
  // sin recurrir a std::find ni a funciones automáticas de búsqueda
  // (restricción pedagógica de la cátedra).
  char comando = leerComandoDelBuffer();

  // ── Despacho del comando mediante if/else if/else ─────────────────
  // Restricción pedagógica: NO se utiliza switch/case.
  // Se aceptan tanto mayúsculas como minúsculas para cada comando.
  if (comando == 'F' || comando == 'f') {
    // Comando reconocido: Avanzar
    avanzar();

  } else if (comando == 'B' || comando == 'b') {
    // Comando reconocido: Retroceder
    retroceder();

  } else if (comando == 'L' || comando == 'l') {
    // Comando reconocido: Girar a la izquierda
    girarIzquierda();

  } else if (comando == 'R' || comando == 'r') {
    // Comando reconocido: Girar a la derecha
    girarDerecha();

  } else if (comando == 'S' || comando == 's') {
    // Comando reconocido: Detener
    detener();

  } else {
    // Comando no reconocido o buffer vacío ('\0'):
    // No se ejecuta ninguna acción; se mantiene el estado actual.
  }
}

// ════════════════════════════════════════════════════════════════════
// leerComandoDelBuffer()
//
// Recorre el arreglo bufferSerial de forma manual e indexada,
// comparando cada carácter con los comandos válidos del protocolo.
// Retorna el primer carácter de comando reconocido que encuentre.
// Retorna '\0' si el buffer está vacío o no contiene ningún comando
// válido.
//
// Implementación sin std::find ni funciones de búsqueda automática
// (restricción pedagógica vigente).
// ════════════════════════════════════════════════════════════════════
char leerComandoDelBuffer() {

  // Calcular la longitud real del contenido del buffer de forma manual,
  // sin usar strlen() ni funciones de la librería estándar de C.
  int longitud = 0;
  while (longitud < TAM_BUFFER && bufferSerial[longitud] != '\0') {
    longitud++;
  }

  // Recorrer cada posición del buffer buscando un carácter de comando.
  int i = 0;
  while (i < longitud) {
    char c = bufferSerial[i];

    // Comparación directa, carácter por carácter, sin búsqueda automática.
    if (c == 'F' || c == 'f' ||
        c == 'B' || c == 'b' ||
        c == 'L' || c == 'l' ||
        c == 'R' || c == 'r' ||
        c == 'S' || c == 's') {
      return c;  // Primer comando válido encontrado — retorno inmediato.
    }
    i++;
  }

  return '\0';  // Buffer vacío o sin comandos reconocibles.
}

// ════════════════════════════════════════════════════════════════════
// avanzar()
// Activa los cuatro motores en sentido FORWARD (tracción delantera y
// trasera en la misma dirección). El vehículo avanza en línea recta.
// ════════════════════════════════════════════════════════════════════
void avanzar() {
  motorDelanteroDerecho.run(FORWARD);
  motorDelanteroIzquierdo.run(FORWARD);
  motorTraseroDerecho.run(FORWARD);
  motorTraseroIzquierdo.run(FORWARD);
}

// ════════════════════════════════════════════════════════════════════
// retroceder()
// Activa los cuatro motores en sentido BACKWARD. El vehículo se
// desplaza en reversa en línea recta.
// ════════════════════════════════════════════════════════════════════
void retroceder() {
  motorDelanteroDerecho.run(BACKWARD);
  motorDelanteroIzquierdo.run(BACKWARD);
  motorTraseroDerecho.run(BACKWARD);
  motorTraseroIzquierdo.run(BACKWARD);
}

// ════════════════════════════════════════════════════════════════════
// girarIzquierda()
// Los motores del lado derecho avanzan (FORWARD) y los del lado
// izquierdo retroceden (BACKWARD). El vehículo pivota hacia la
// izquierda sobre su eje central (giro diferencial).
// ════════════════════════════════════════════════════════════════════
void girarIzquierda() {
  motorDelanteroDerecho.run(FORWARD);
  motorDelanteroIzquierdo.run(BACKWARD);
  motorTraseroDerecho.run(FORWARD);
  motorTraseroIzquierdo.run(BACKWARD);
}

// ════════════════════════════════════════════════════════════════════
// girarDerecha()
// Los motores del lado izquierdo avanzan (FORWARD) y los del lado
// derecho retroceden (BACKWARD). El vehículo pivota hacia la derecha
// sobre su eje central (giro diferencial).
// ════════════════════════════════════════════════════════════════════
void girarDerecha() {
  motorDelanteroDerecho.run(BACKWARD);
  motorDelanteroIzquierdo.run(FORWARD);
  motorTraseroDerecho.run(BACKWARD);
  motorTraseroIzquierdo.run(FORWARD);
}

// ════════════════════════════════════════════════════════════════════
// detener()
// Aplica RELEASE a los cuatro canales del Motor Shield L293D.
// En modo RELEASE el shield corta la corriente al bobinado del motor
// (freno dinámico por inercia). No confundir con BRAKE (corto activo).
// ════════════════════════════════════════════════════════════════════
void detener() {
  motorDelanteroDerecho.run(RELEASE);
  motorDelanteroIzquierdo.run(RELEASE);
  motorTraseroDerecho.run(RELEASE);
  motorTraseroIzquierdo.run(RELEASE);
}
