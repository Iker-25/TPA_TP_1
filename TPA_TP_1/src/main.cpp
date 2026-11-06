#include <Arduino.h>
#include <Secretos.h>
#include <Sensores.h>
#include <Luces.h>
#include <Pantalla.h>
#include <ConexionWifi.h>
#include <Telemetria.h>
#include <BotTelegram.h>

Sensores sensores(33, 32);
Luces luces(23, 2);
Pantalla pantalla;
ConexionWifi wifi;
Telemetria nube;
BotTelegram telegram(TOKEN_BOT, sensores, luces, pantalla, nube);

void setup() {
  Serial.begin(115200);
  sensores.iniciar();
  luces.iniciar();
  pantalla.iniciar();
  wifi.iniciar(WIFI_SSID, WIFI_PASS);
  nube.iniciar(AIO_USUARIO, AIO_CLAVE, AIO_GRUPO,
               AIO_FEED_TEMPERATURA, AIO_FEED_HUMEDAD);
  telegram.iniciar();
}

void loop() {
  wifi.actualizar();
  sensores.actualizar();
  luces.actualizar(sensores.datos().voltaje);
  telegram.actualizar();
  nube.actualizar(sensores.datos());
  pantalla.actualizar(sensores.datos(), luces);
}
