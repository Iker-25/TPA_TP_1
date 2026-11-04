#include <Arduino.h>
#include <Sensores.h>
#include <Luces.h>                // <- Luces
Sensores sensores(33, 32);
Luces luces(23, 2);               // <- Luces

void setup() {
  Serial.begin(115200);
  sensores.iniciar();
  luces.iniciar();                // <- Luces
  luces.activarAuto(0.8f);        // <- Luces
}

void loop() {
  sensores.actualizar();
  luces.actualizar(sensores.datos().voltaje);   // <- Luces
  const Medicion& m = sensores.datos();
  Serial.printf("T %.1f C | H %.1f %% | Pote %.2f V | DHT valido %d\r\n",
                m.temperatura, m.humedad, m.voltaje, m.dhtValido);
  delay(250); // Solo para esta prueba de monitor.
}

