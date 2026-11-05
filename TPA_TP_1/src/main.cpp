#include <Arduino.h>
#include <Secretos.h>
#include <Sensores.h>
#include <Luces.h>
#include <Pantalla.h>
#include <ConexionWifi.h>
#include <Telemetria.h>

Sensores sensores(33, 32);
Luces luces(23, 2);
Pantalla pantalla;
ConexionWifi wifi;
Telemetria nube;

void setup() {
  Serial.begin(115200);
  sensores.iniciar();
  luces.iniciar();
  luces.activarAuto(0.8f); // Prueba existente del integrante 3.
  pantalla.iniciar();
  pantalla.seleccionar(Vista::Dht);
  wifi.iniciar(WIFI_SSID, WIFI_PASS);
  nube.iniciar(AIO_USUARIO, AIO_CLAVE, AIO_GRUPO,
               AIO_FEED_TEMPERATURA, AIO_FEED_HUMEDAD);
}

void loop() {
  wifi.actualizar();
  sensores.actualizar();
  luces.actualizar(sensores.datos().voltaje);
  nube.actualizar(sensores.datos());
  pantalla.actualizar(sensores.datos(), luces);
}
