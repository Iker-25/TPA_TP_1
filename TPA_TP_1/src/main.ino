#include <Arduino.h>
#include <Sensores.h>
Sensores sensores(33, 32);

void setup() {
  Serial.begin(115200);
  sensores.iniciar();
}

void loop() {
  sensores.actualizar();
  const Medicion& m = sensores.datos();
  Serial.printf("T %.1f C | H %.1f %% | Pote %.2f V | DHT valido %d\n",
                m.temperatura, m.humedad, m.voltaje, m.dhtValido);
  delay(250); // Solo para esta prueba de monitor.
}

