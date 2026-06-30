/*
 * ════════════════════════════════════════════════════════════════════
 * ESCUDERÍA U.F.C. — Vehículo: Dagestan
 * Proyecto Informático 1 | Prof. Cyntyha Berea | 2026
 * Ingenieros: Ulises Cabrera, Fabricio Crespo, Clovis Clemencot
 * ────────────────────────────────────────────────────────────────────
 * Hardware:
 * - Arduino UNO R3 (ATmega328P, 16 MHz, 5V TTL)
 * - Motor Shield L293D (4 puentes H independientes)
 * - Módulo Bluetooth HC-06 (SoftwareSerial en pines RX=9, TX=10, 9600 bps)
 * - 4x Motores DC reductores amarillos (3–6V)
 * - Alimentación principal: Pila 9V (según especificaciones técnicas)
 * ────────────────────────────────────────────────────────────────────
 * Comandos Bluetooth (carácter ASCII):
 * F → Avanzar      B → Retroceder
 * L → Girar izq.   R → Girar der.
 * ────────────────────────────────────────────────────────────────────
 * Actualización / Lógica de Control:
 * - Se implementa la estructura switch/case para optimizar la
 * lógica de control y despacho de comandos serie.
 * - Uso de la librería SoftwareSerial para evitar conflictos con el
 * puerto serie de hardware nativo del Arduino (TX/RX 0 y 1).
 * - La velocidad PWM (255) y el estado (RELEASE/0) se asignan de 
 * forma dinámica dentro de cada función cinemática.
 * ════════════════════════════════════════════════════════════════════
 */

// ── Librerías del sistema ────────────────────────────────────────────
#include <AFMotor.h>
#include <SoftwareSerial.h>

// ── Configuración del Módulo Bluetooth ───────────────────────────────
SoftwareSerial bluetoothSerial(9, 10); // RX, TX

// ── Configuración del Motor Shield L293D ─────────────────────────────
// Pines iniciales de los motores y frecuencia PWM
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

// Variable global para almacenar el comando recibido
char Variable;

void setup()
{
  bluetoothSerial.begin(9600);  // Establecemos los baudios para el módulo bluetooth.
}

void loop() {
  // Verificamos si hay datos disponibles en el puerto serial virtual
  if (bluetoothSerial.available() > 0) {
    Variable = bluetoothSerial.read(); // Leemos el byte entrante

    Stop(); // Inicializo con motores detenidos por seguridad antes del cambio de estado
    
    // Despacho del carácter de control
    switch (Variable) {
      case 'F':
        adelante();
        break;
      case 'B':
        atras();
        break;
      case 'L':
        izquierda();
        break;
      case 'R':
        derecha();
        break;
    }
  }
}

// ── IMPLEMENTACIÓN DE FUNCIONES DE CONTROL CINEMÁTICO MÓVIL ──────────

void adelante()
{
  motor1.setSpeed(255); // Asigno maxima velocidad al motor 1
  motor1.run(FORWARD);  // Hago girar el motor 1 hacia delante
  motor2.setSpeed(255); // Asigno maxima velocidad al motor 2
  motor2.run(FORWARD);  // Hago girar el motor 2 hacia delante
  motor3.setSpeed(255); // Asigno maxima velocidad al motor 3
  motor3.run(FORWARD);  // Hago girar el motor 3 hacia delante
  motor4.setSpeed(255); // Asigno maxima velocidad al motor 4
  motor4.run(FORWARD);  // Hago girar el motor 4 hacia delante
}

void atras()
{
  motor1.setSpeed(255); // Asigno maxima velocidad al motor 1
  motor1.run(BACKWARD); // Hago girar el motor 1 hacia atras
  motor2.setSpeed(255); // Asigno maxima velocidad al motor 2
  motor2.run(BACKWARD); // Hago girar el motor 2 hacia atras
  motor3.setSpeed(255); // Asigno maxima velocidad al motor 3
  motor3.run(BACKWARD); // Hago girar el motor 3 hacia atras
  motor4.setSpeed(255); // Asigno maxima velocidad al motor 4
  motor4.run(BACKWARD); // Hago girar el motor 4 hacia atras
}

void izquierda()
{
  motor1.setSpeed(255); // Asigno maxima velocidad al motor 1
  motor1.run(FORWARD);  // Hago girar el motor 1 hacia delante
  motor2.setSpeed(255); // Asigno maxima velocidad al motor 2
  motor2.run(BACKWARD); // Hago girar el motor 2 hacia atras
  motor3.setSpeed(255); // Asigno maxima velocidad al motor 3
  motor3.run(BACKWARD); // Hago girar el motor 3 hacia atras
  motor4.setSpeed(255); // Asigno maxima velocidad al motor 4
  motor4.run(FORWARD);  // Hago girar el motor 4 hacia delante
}

void derecha()
{
  motor1.setSpeed(255); // Asigno maxima velocidad al motor 1
  motor1.run(BACKWARD); // Hago girar el motor 1 hacia atras
  motor2.setSpeed(255); // Asigno maxima velocidad al motor 2
  motor2.run(FORWARD);  // Hago girar el motor 2 hacia delante
  motor3.setSpeed(255); // Asigno maxima velocidad al motor 3
  motor3.run(FORWARD);  // Hago girar el motor 3 hacia delante
  motor4.setSpeed(255); // Asigno maxima velocidad al motor 4
  motor4.run(BACKWARD); // Hago girar el motor 4 hacia atras
}

void Stop()
{
  motor1.setSpeed(0);  // Asigno minima velocidad al motor 1
  motor1.run(RELEASE); // Paro el motor 1 cuando dejo de pulsar el boton
  motor2.setSpeed(0);  // Asigno minima velocidad al motor 2
  motor2.run(RELEASE); // Paro el motor 2 cuando dejo de pulsar el boton
  motor3.setSpeed(0);  // Asigno minima velocidad al motor 3
  motor3.run(RELEASE); // Paro el motor 3 cuando dejo de pulsar el boton
  motor4.setSpeed(0);  // Asigno minima velocidad al motor 4
  motor4.run(RELEASE); // Paro el motor 4 cuando dejo de pulsar el boton
}
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
