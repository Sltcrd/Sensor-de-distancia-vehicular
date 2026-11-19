# Definition of application service layer requirements — Sensor de distancia vehicular

> Paso 6 de los 12 IoT System Design Steps. Completar con los datos reales del caso (umbrales en cm, colores, estados).

| Servicio | Usuario final | Interfaz (tipo) | Especificación de la interfaz de usuario |
| --- | --- | --- | --- |
| S1. Medición y aviso local de distancia | Conductor | IoT Device: LEDs + buzzer (+ LCD si aplica) | _Distancia en cm, estado (seguro / precaución / peligro), color de LED y frecuencia del buzzer por umbral._ |
| S2. Monitoreo de distancia | Conductor; personal de flota | Móvil (conductor); desktop web (personal) | _Distancia actual, historial, indicador por umbral._ |
| S3. Alertas de proximidad | Conductor; personal de flota | Móvil (push); desktop/tablet (panel) | _Alerta al cruzar umbrales y ante falla del sensor, con hora y botón "atendida"._ |
| S4. Configuración de umbrales | Personal / administrador | Desktop web | _Edición de umbrales (cm) y sonido/color de alerta._ |
| S5. Gestión de dispositivos | Personal de soporte | Desktop web | _Alta de vehículo–dispositivo, estado (activo, en falla, sin datos)._ |

Borrador inicial a validar contra el enunciado. Debe coincidir con lo que muestra el prototipo (Pregunta 2).
