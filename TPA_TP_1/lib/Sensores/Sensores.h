#pragma once
#include <Arduino.h>
#include <DHT.h>

struct Medicion {
  float temperatura = NAN;
  float humedad = NAN;
  float voltaje = 0;
  bool dhtValido = false;
};

class Sensores {
public:
  Sensores(uint8_t pinDht, uint8_t pinPot);
  void iniciar();
  void actualizar();
  const Medicion& datos() const;
private:
  DHT dht;
  uint8_t pinPot;
  Medicion muestra;
  unsigned long ultimaDht = 0;
  bool primera = true;
};