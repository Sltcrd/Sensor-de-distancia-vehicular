---
name: wokwi-iot
description: Guía para desarrollar el prototipo Wokwi (OOP, ModestIoT) y la Pregunta 1 (Application Service Layer Requirements, paso 6 de los 12 IoT System Design Steps) en este repo. Úsala al crear o modificar el sketch, el diagram.json o los requisitos de la capa de servicio.
---

# Wokwi + ModestIoT + capa de servicio

Fuente de referencia (material de estudio no oficial, verificar siempre contra el enunciado y la rúbrica):
https://github.com/marquinho04/wokwi (kit genérico `kit/`, ejemplos y docs `docs/01`–`06`).

## Entregables
1. **Prototipo Wokwi** (C++ / Arduino sketch, OOP basado en ModestIoT).
2. **Pregunta 1:** *Definition of application service layer requirements*, con la interfaz de usuario de cada servicio (móvil, tablet, desktop o más de uno).

## Flujo del prototipo
1. Tabla de requisitos: entrada (sensor) → regla → salida (actuador). Subrayar cada número y componente del enunciado.
2. Elegir clases del kit: `AnalogSensor`, `DigitalSensor`, `UltrasonicSensor` (HC-SR04), `DhtSensor`, `WeightSensor`, `Led`/`Relay`/`Buzzer`, `LcdDisplay`, `PeriodicTimer`, `OneShotTimer`, `LevelClassifier`. Núcleo: `ModestIoT.h` (Event, Command, Sensor, Actuator, Device).
3. Armar y probar el circuito antes del código: placa pedida, resistencia de 220 Ω por LED, GND/VCC en todos los módulos, LCD por I2C (`"pins": "i2c"`, 0x27).
4. Escribir solo el **Device**: constantes con nombre, un objeto por sensor/actuador/timer, `setup()`, `loop()` solo con timers (sin `delay()`), `on(Event)` con las reglas (una rama por sensor, `default`).
5. `sketch.ino` solo instancia el Device y llama `setup()`/`loop()`.
6. Probar cada regla en el límite exacto, justo debajo y justo encima, y la falla del sensor (`Event::FAULT`: mensaje en consola/display y actuadores en estado seguro).
7. Pulido: title block (propósito y autoría) en cada archivo, nombres en inglés (`PascalCase` clases, `camelCase` métodos/variables, `UPPER_SNAKE_CASE` constantes), sin código comentado ni variables sin uso, consola con datos de la empresa y nombre al arrancar y cada cambio de estado (`[STATE] A -> B`).

## Pregunta 1 (método de 5 pasos)
1. Identificar usuarios que el caso sostiene (usuario del dispositivo y quien opera el servicio). No inventar.
2. Un servicio por cada capacidad: monitoreo, alertas, configuración (si hay umbrales), gestión de dispositivos/soporte.
3. Asignar interfaz por tipo de dispositivo con motivo: móvil (usuario final), desktop/tablet (quien supervisa a muchos), IoT device (display/LEDs/buzzer si el servicio es local).
4. Describir qué muestra y qué permite cada interfaz con datos reales del caso (unidades, umbrales, colores, estados).
5. Cuadrar con el prototipo: lo que muestra el display/LED debe coincidir con el servicio local.

Formato de tabla:

| Servicio | Usuario final | Interfaz (tipo) | Especificación de la interfaz de usuario |
| --- | --- | --- | --- |

Plantilla en `references/servicio-capa-aplicacion.md` (en esta misma carpeta).

## Rúbrica (S = sobresaliente)
C01 Application service layer: todos los servicios, interfaz de cada uno, alineados con la solución. C02 Device design: todos los componentes pedidos. C03 Software features: todos los casos y excepciones. C04 Calidad: C++, title blocks, patrones (Mediator, herencia, Event/Command), sin redundancia. C05 Nombres en inglés estándar.

## Convenciones del repo
- Gitflow, conventional commits, historial ordenado, sin coautor en los commits.
- Probar en local antes de subir; no subir a producción sin aprobación del dueño.

## Lecciones de la solución de referencia (PC2 2024-2, Moen, nota 17,5)
- Entrega: un `.zip` del proyecto Wokwi y un `.pptx` con la Pregunta 1, la URL del proyecto, el código de cada archivo (`sketch.ino`, `diagram.json`, `libraries.txt` y cada clase) y una captura del Library Manager.
- La solución de referencia usó un Device que agrupa SensorManager, ActuatorManager y StateManager, con las librerías `WiFi` y `Time`. Al iniciar mostró la empresa y el nombre del developer, y reportó distancia y estado de la válvula cada 2,5 s.
- Pregunta 1 de referencia: cinco servicios (monitoreo de uso de agua, control de válvula, configuración del dispositivo, mantenimiento y soporte, uso del agua), con interfaz móvil y desktop y datos del caso (umbral 10 cm, 5 s).
- Para superar esa nota: cubrir todos los servicios que el caso implica con su tipo de interfaz, implementar lo que el caso nombre aunque ninguna regla dependa de ello (por ejemplo WiFi), mostrar modos de operación y cambios de estado en consola, y mantener title blocks y nombres en inglés en todos los archivos.
- Wokwi: el simulador de distancia del HC-SR04 usa 340 m/s (58,82 µs por cm de ida y vuelta); redondear la distancia a cm enteros evita que 10 cm nominales se lean como 9,9.

