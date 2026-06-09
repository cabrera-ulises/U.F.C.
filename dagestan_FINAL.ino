/*
 * ════════════════════════════════════════════════════════════════════
 * ESCUDERÍA U.F.C. — Vehículo: Dagestan
 * Proyecto Informático 1 | Prof. Cynthia Berea | 2026
 * Ingenieros: Ulises Cabrera, Fabricio Crespo, Clovis Clemencot
 * ────────────────────────────────────────────────────────────────────
 * Hardware:
 * - Arduino UNO R3 (ATmega328P, 16 MHz, 5V TTL)
 * - Motor Shield L293D (4 puentes H independientes)
 * - Módulo Bluetooth HC-06 (UART, modo Esclavo, 9600 bps)
 * - 4x Motores DC reductores amarillos (3–6V, 200 mA típico)
 * - Portapilas 4xAA (6V, fuente independiente de motores)
 * ────────────────────────────────────────────────────────────────────
 * Comandos Bluetooth (carácter ASCII):
 * F → Avanzar      B → Retroceder
 * L → Girar izq.   R → Girar der.
 * S → Detener
 * ────────────────────────────────────────────────────────────────────
 * Actualización de Cátedra:
 * - Se implementa la estructura switch/case para optimizar la
 * lógica de control y despacho de comandos serie.
 * - Se mantiene la búsqueda e indexación manual en el buffer
 * sin librerías automatizadas para control estricto de memoria.
 * ════════════════════════════════════════════════════════════════════
 */

// ── Librería del Motor Shield L293D ──────────────────────────────────
#include <AFMotor.h>

// Definición de los 4 canales de asignación de motores independientes
AF_DCMotor motorDelanteroDerecho(1);
AF_DCMotor motorDelanteroIzquierdo(2);
AF_DCMotor motorTraseroDerecho(3);
AF_DCMotor motorTraseroIzquierdo(4);

// Parámetros de configuración del sistema
const int VELOCIDAD_CONSIGNA = 200; // Valor PWM de velocidad (0 a 255)
char bufferSerial[10];              // Array estático para almacenar datos serie
int indiceBuf = 0;                  // Puntero de índice para el buffer

// Declaración de prototipos de funciones cinemáticas
void avanzar();
void retroceder();
void girarIzquierda();
void girarDerecha();
void detener();
void procesarComando(char comando);

void setup() {
  // Inicialización del puerto serie para el módulo HC-06 a 9600 baudios
  Serial.begin(9600);
  
  // Establecer la velocidad nominal en los cuatro controladores de potencia
  motorDelanteroDerecho.setSpeed(VELOCIDAD_CONSIGNA);
  motorDelanteroIzquierdo.setSpeed(VELOCIDAD_CONSIGNA);
  motorTraseroDerecho.setSpeed(VELOCIDAD_CONSIGNA);
  motorTraseroIzquierdo.setSpeed(VELOCIDAD_CONSIGNA);
  
  // Inicializar el vehículo en estado pasivo/frenado por seguridad
  detener();
}

void loop() {
  // Bucle indexado manual para la escucha y llenado del buffer ASCII
  while (Serial.available() > 0) {
    char byteRecibido = Serial.read();
    
    // Detección de caracteres de fin de línea / retorno de carro
    if (byteRecibido == '\n' || byteRecibido == '\r') {
      if (indiceBuf > 0) {
        bufferSerial[indiceBuf] = '\0'; // Terminación segura de la cadena
        
        // Despacho del carácter de control alojado en la primera posición
        procesarComando(bufferSerial[0]);
        
        // CONTROL DE CALIDAD: Limpieza explícita de memoria residual
        bufferSerial[0] = '\0'; 
        indiceBuf = 0; // Reinicio del puntero de asignación
      }
    } else {
      // Prevención de desborde de memoria (Buffer Overflow)
      if (indiceBuf < 9) {
        bufferSerial[indiceBuf] = byteRecibido;
        indiceBuf++;
      }
    }
  }
}

/*
 * ════════════════════════════════════════════════════════════════════
 * procesarComando(char comando)
 * OPTIMIZACIÓN DE HARDWARE/SOFTWARE: Despacho modular mediante switch
 * reemplazando las ramificaciones condicionales anidadas previas.
 * ════════════════════════════════════════════════════════════════════
 */
void procesarComando(char comando) {
  switch (comando) {
    case 'F':
      avanzar();
      break;
    case 'B':
      retroceder();
      break;
    case 'L':
      girarIzquierda();
      break;
    case 'R':
      girarDerecha();
      break;
    case 'S':
      detener();
      break;
    default:
      // Ante cualquier anomalía o carácter corrupto, detener el coche
      detener();
      break;
  }
}

// ── IMPLEMENTACIÓN DE FUNCIONES DE CONTROL CINEMÁTICO MÓVIL ──────────

void avanzar() {
  motorDelanteroDerecho.run(FORWARD);
  motorDelanteroIzquierdo.run(FORWARD);
  motorTraseroDerecho.run(FORWARD);
  motorTraseroIzquierdo.run(FORWARD);
}

void retroceder() {
  motorDelanteroDerecho.run(BACKWARD);
  motorDelanteroIzquierdo.run(BACKWARD);
  motorTraseroDerecho.run(BACKWARD);
  motorTraseroIzquierdo.run(BACKWARD);
}

void girarIzquierda() {
  motorDelanteroDerecho.run(FORWARD);
  motorDelanteroIzquierdo.run(BACKWARD);
  motorTraseroDerecho.run(FORWARD);
  motorTraseroIzquierdo.run(BACKWARD);
}

void girarDerecha() {
  motorDelanteroDerecho.run(BACKWARD);
  motorDelanteroIzquierdo.run(FORWARD);
  motorTraseroDerecho.run(BACKWARD);
  motorTraseroIzquierdo.run(FORWARD);
}

void detener() {
  motorDelanteroDerecho.run(RELEASE);
  motorDelanteroIzquierdo.run(RELEASE);
  motorTraseroDerecho.run(RELEASE);
  motorTraseroIzquierdo.run(RELEASE);
}
