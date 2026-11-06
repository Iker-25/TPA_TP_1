# Pruebas del TPI 01

## Integrante 4 — Pantalla
- Nombre y fecha: pendiente.
- Entorno y controlador OLED: pendiente.
- Casos ejecutados y resultados: pendiente.
- Evidencia sin credenciales: pendiente.
- Fallos o pendientes: pendiente.

## Integrante 5 — WiFi y telemetría
- Nombre y fecha: pendiente.
- Entorno: pendiente.
- Casos ejecutados y resultados: pendiente.
- Evidencia de ambos feeds: pendiente.
- Fallos o pendientes: pendiente.

## Integrante 6 — Telegram
- Nombre y fecha: pendiente.
- Entorno: pendiente.
- Comandos y alias probados: pendiente.
- Historial, errores y envío manual: pendiente.
- Fallos o pendientes: pendiente.

## Integrante 7 — Integración
- Nombre y fecha: 5/11/2026.
- Entorno y placa: PlatformIO (`esp32doit-devkit-v1`, RAM 15.6 %, Flash 98.7 %) + Simulador Wokwi CLI v0.29.1 (`board-esp32-devkit-c-v4`, OLED `SSD1306` I2C `0x3C` en GPIO 21/22).
- Matriz final y resultados reales:
  - **Inicio:** Arranque en modo manual, LEDs verde (GPIO 23) y azul (GPIO 2) apagados, umbral inicial `0.50 V`, pantalla OLED en vista `Inicio` (`TPI 01 - ESP32`) con historial vacío (`APROBADO`).
  - **Menú:** `/start` y alias `1` devuelven el mismo menú enriquecido en formato HTML con emojis (`APROBADO`).
  - **Manual:** Encendido y apagado individual de LED verde (`/verdeon`, `7`, `/verdeoff`, `8`) y LED azul (`/azulon`, `5`, `/azuloff`, `6`) sin afectar al otro LED (`APROBADO`).
  - **Referencia (`/autoon` / `3`):** Captura el voltaje real del potenciómetro (`0.40 V` en prueba) como base, sin la constante `0.80 V` del diagnóstico anterior (`APROBADO`).
  - **Cruce e igualdad:** En modo automático, con `voltaje == base + umbral` (`0.90 V`) el LED azul permanece apagado; al superar `base + umbral` (`0.95 V`) enciende; al bajar (`0.85 V`) se apaga (`APROBADO`).
  - **Repetición de `/autoon`:** Enviar `/autoon` nuevamente con otro voltaje (`2.10 V`) conserva la referencia anterior (`0.40 V`) (`APROBADO`).
  - **Rechazo en automático:** Órdenes manuales (`/verdeon`) en modo automático son rechazadas con el aviso `"Primero envia /autooff para usar el modo manual."` y no entran al historial OLED (`APROBADO`).
  - **Salida (`/autooff` / `4`):** Vuelve a modo manual conservando el estado actual de los LEDs y habilitando control manual (`APROBADO`).
  - **Umbral y alias con coma:** Acepta extremos `1.0` (`1.00 V`), `1.5` (`1.50 V`) y `9 1,2` (`1.20 V`). Valores fuera de rango (`0.9`, `1.6`) restauran `0.50 V`, informan `"Fuera de rango. Se restauro 0.50 V."` y no se registran como éxito (`APROBADO`).
  - **Sintaxis inválida:** `/umbral abc`, `/umbral nan`, `/umbral 1.2abc` y `1abc` se rechazan sin conversión parcial ni cambio del umbral previo (`APROBADO`).
  - **OLED e historial:** Vistas `11` (`/displayled`), `12` (`/displaypot`) y `13` (`/displaydht`) actualizan la mitad superior; tras 4 comandos válidos (`/estado`, `11`, `12`, `13`) solo quedan los últimos 3 (`/displaydht`, `/displaypot`, `/displayled`), el más reciente arriba (`APROBADO`).
  - **Error OLED (`/displayxyz`):** Muestra `"Comando desconocido"` en la mitad superior y conserva intactos los 3 últimos comandos exitosos en la mitad inferior (`APROBADO`).
  - **Nube manual (`/thingup` / `10`) y espera:** Publicación HTTPS exitosa y confirmada en ambos feeds del grupo `tpi` (`tpi.temperatura = 24.0`, `tpi.humedad = 40.0`, `updated_at = 2026-11-06T01:48:17Z`). La repetición inmediata devuelve `"Reintento disponible en 20 s."` sin volver a publicar (`APROBADO`).
  - **DHT inválido y errores de cuenta:** Con `dhtValido = false` (`NAN`) devuelve `"No hay lectura valida del DHT22."` sin publicar `NAN`. Con credenciales incompletas informa `"Falta configurar Adafruit IO."` y con clave inválida devuelve `"Adafruit IO devolvio HTTP/transporte 401."` sin falso éxito (`APROBADO`).
- Diferencias respecto de Wokwi: Pendiente de contrastar lectura del ADC real y controlador físico OLED (SSD1306 vs SH1106) en el montaje en placa de laboratorio.
- Fallos o pendientes: Ninguno en simulación e integración de software; pendiente únicamente prueba en placa física UTN.
