#include "Sensores.h"

Sensores::Sensores(uint8_t pinDht, uint8_t pinPot)
    : dht(pinDht, DHT22), pinPot(pinPot) {}

void Sensores::iniciar() {
  pinMode(pinPot, INPUT);
  analogReadResolution(12);
  dht.begin();
}

void Sensores::actualizar() {
  muestra.voltaje = analogRead(pinPot) * 3.3f / 4095.0f;
  unsigned long ahora = millis();
  if (!primera && ahora - ultimaDht < 2000UL) return;
  primera = false;
  ultimaDht = ahora;
  muestra.temperatura = dht.readTemperature();
  muestra.humedad = dht.readHumidity();
  muestra.dhtValido = !isnan(muestra.temperatura) && !isnan(muestra.humedad);
}

const Medicion& Sensores::datos() const {
  return muestra;
}