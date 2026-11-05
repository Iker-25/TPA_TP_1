#include "ConexionWifi.h"

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

void ConexionWifi::iniciar(const char* ssid, const char* clave) {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, clave);
  ultimoIntento = millis();
  Serial.println(F("[WIFI] Conectando a red..."));
}

void ConexionWifi::actualizar() {
  const bool disponible = WiFi.status() == WL_CONNECTED;
  if (disponible && !conectada) {
    Serial.println(F("[WIFI] Conectado. Sincronizando hora..."));
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  }
  conectada = disponible;

  if (!disponible && millis() - ultimoIntento >= 15000UL) {
    ultimoIntento = millis();
    Serial.println(F("[WIFI] Reintentando conexion..."));
    WiFi.reconnect();
  }
}
