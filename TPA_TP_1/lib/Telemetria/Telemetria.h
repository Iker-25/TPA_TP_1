#pragma once

#include <Arduino.h>
#include <Sensores.h>
#include <WiFiClientSecure.h>

class Telemetria {
public:
  void iniciar(const char* usuario, const char* clave, const char* grupo,
               const char* temperatura, const char* humedad);
  bool enviar(const Medicion& medicion, String& respuesta);
  void actualizar(const Medicion& medicion);

private:
  WiFiClientSecure cliente;
  String url;
  String key;
  String feedTemperatura;
  String feedHumedad;
  bool habilitada = false;
  bool huboIntento = false;
  unsigned long ultimoIntento = 0;
  unsigned long ultimoPeriodo = 0;
  unsigned long espera = 20000UL;
};
