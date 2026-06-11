<p align="center">
  <img src="logo_ufc.png" width="180" alt="Logo U.F.C.">
</p>

# 🏎️ Escudería U.F.C. — Vehículo Dagestan

> Proyecto Informático 1 | Prof. Cynthia Berea | 2026
>
> Asignatura de Formación Técnica Específica

## Descripción

Prototipo robótico de tracción integral 4WD controlado de forma inalámbrica mediante Bluetooth. Vehículo bautizado oficialmente como **Dagestan**, basado en el chasis original del kit 4WD agrandado en el taller tras descartar el diseño Fennec. Desarrollado como producto *Llave en Mano* al término del 1er bimestre 2026.

## Integrantes

| Nombre | Rol Scrum |
|---|---|
| Ulises Cabrera | Scrum Master |
| Fabricio Crespo | Product Owner |
| Clovis Clemencot | Development Team |

## Hardware

| Componente | Modelo | Función |
|---|---|---|
| Microcontrolador | Arduino UNO R3 | Cerebro del sistema |
| Controlador de motores | Motor Shield L293D | Control de 4 motores DC independientes |
| Comunicación | HC-06 Bluetooth | Recepción de comandos desde el celular |
| Tracción | 4x Motores DC reductores | Propulsión 4WD |
| Alimentación | Portapilas 4xAA | Fuente de 6V independiente para motores |

## Comandos de control (Bluetooth)

| Carácter | Acción |
|---|---|
| `F` | Avanzar |
| `B` | Retroceder |
| `L` | Girar a la izquierda |
| `R` | Girar a la derecha |
| `S` | Detener |

## Cómo clonar y compilar

```bash
# 1. Clonar el repositorio
git clone https://github.com/cabrera-ulises/U.F.C.git
cd U.F.C.

# 2. Abrir en Arduino IDE
# Archivo: firmware/dagestan_FINAL.ino

# 3. Configuración del IDE
# Placa   : Arduino UNO
# Puerto  : COMx o /dev/ttyUSBx (según sistema operativo)
# ATENCIÓN: desconectar HC-06 (pines RX/TX) antes de cargar el sketch

# 4. Compilar y subir
# Sketch → Verificar/Compilar → Subir
```

## Estado del proyecto

✅ MVP entregado — Sprint 1 finalizado

## Licencia

© 2026 Escudería U.F.C. — Ulises Cabrera, Fabricio Crespo, Clovis Clemencot.
Todos los derechos reservados. Ver [LICENCIA.md](LICENCIA.md) para condiciones de uso y distribución.
