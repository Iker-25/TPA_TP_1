#include "Telemetria.h"

#include "CertificadoAdafruit.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <time.h>

void Telemetria::iniciar(const char* usuario, const char* clave,
                         const char* grupo, const char* temperatura,
                         const char* humedad) {
  key = clave;
  feedTemperatura = temperatura;
  feedHumedad = humedad;

  const String usuarioConfigurado(usuario);
  const String grupoConfigurado(grupo);
  habilitada = usuarioConfigurado.length() > 0
    && usuarioConfigurado != "COMPLETAR_LOCALMENTE"
    && key.length() > 0 && key != "COMPLETAR_LOCALMENTE"
    && grupoConfigurado.length() > 0
    && feedTemperatura.length() > 0 && feedHumedad.length() > 0
    && feedTemperatura != feedHumedad;

  url = String("https://io.adafruit.com/api/v2/") + usuarioConfigurado
    + "/groups/" + grupoConfigurado + "/data";
  cliente.setCACert(CERTIFICADO_ADAFRUIT);
  cliente.setHandshakeTimeout(5);
  cliente.setTimeout(5000);
  ultimoPeriodo = millis();

  if (!habilitada) {
    Serial.println(F("[AIO] Completar usuario, clave y feeds en include/Secretos.h"));
  }
}

bool Telemetria::enviar(const Medicion& medicion, String& respuesta) {
  if (!habilitada) {
    respuesta = F("Falta configurar Adafruit IO.");
    return false;
  }
  if (!medicion.dhtValido) {
    respuesta = F("No hay lectura valida del DHT22.");
    return false;
  }
  if (WiFi.status() != WL_CONNECTED) {
    respuesta = F("No hay conexion WiFi.");
    return false;
  }
  if (time(nullptr) < 1700000000) {
    respuesta = F("Esperando sincronizacion de hora.");
    return false;
  }

  const unsigned long transcurrido = millis() - ultimoIntento;
  if (huboIntento && transcurrido < espera) {
    respuesta = String("Reintento disponible en ")
      + String((espera - transcurrido + 999UL) / 1000UL) + " s.";
    return false;
  }

  StaticJsonDocument<512> documento;
  JsonArray feeds = documento.createNestedArray("feeds");
  JsonObject datoTemperatura = feeds.createNestedObject();
  datoTemperatura["key"] = feedTemperatura;
  datoTemperatura["value"] = medicion.temperatura;
  JsonObject datoHumedad = feeds.createNestedObject();
  datoHumedad["key"] = feedHumedad;
  datoHumedad["value"] = medicion.humedad;

  if (documento.overflowed()) {
    respuesta = F("No se pudo preparar el JSON.");
    return false;
  }

  String cuerpo;
  serializeJson(documento, cuerpo);

  ultimoIntento = millis();
  huboIntento = true;
  HTTPClient http;
  http.setConnectTimeout(5000);
  http.setTimeout(5000);
  if (!http.begin(cliente, url)) {
    respuesta = F("No se pudo iniciar HTTPS con Adafruit IO.");
    return false;
  }

  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-AIO-Key", key);
  const int codigo = http.POST(cuerpo);
  String recibido;
  if (codigo >= 200 && codigo < 300) {
    recibido = http.getString();
  }
  http.end();

  ultimoIntento = millis();
  espera = codigo == 429 ? 60000UL : 20000UL;
  if (codigo < 200 || codigo >= 300) {
    respuesta = String("Adafruit IO devolvio HTTP/transporte ")
      + String(codigo) + ".";
    return false;
  }

  StaticJsonDocument<1024> resultado;
  if (deserializeJson(resultado, recibido)) {
    respuesta = F("Adafruit IO respondio con JSON invalido.");
    return false;
  }

  bool guardoTemperatura = false;
  bool guardoHumedad = false;
  for (JsonObject dato : resultado.as<JsonArray>()) {
    const String feed = dato["feed_key"] | "";
    if (!dato["id"].isNull()) {
      if (feed == feedTemperatura) guardoTemperatura = true;
      if (feed == feedHumedad) guardoHumedad = true;
    }
  }

  if (!guardoTemperatura || !guardoHumedad) {
    respuesta = F("Confirmacion incompleta: revisar los dos feeds en Adafruit IO.");
    return false;
  }

  respuesta = F("Temperatura y humedad confirmadas en Adafruit IO.");
  return true;
}

void Telemetria::actualizar(const Medicion& medicion) {
  if (!habilitada || !medicion.dhtValido
      || millis() - ultimoPeriodo < 60000UL) {
    return;
  }

  ultimoPeriodo = millis();
  String respuesta;
  enviar(medicion, respuesta);
  Serial.print(F("[AIO] "));
  Serial.println(respuesta);
}
