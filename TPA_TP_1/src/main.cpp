#include <Arduino.h>
#include <Sensores.h>
#include <Luces.h>
#include <Pantalla.h>

Sensores sensores(33, 32);
Luces luces(23, 2);
Pantalla pantalla;

void setup() {
  Serial.begin(115200);
  sensores.iniciar();
  luces.iniciar();
  luces.activarAuto(0.8f); // Prueba existente del integrante 3.
  pantalla.iniciar();
  pantalla.seleccionar(Vista::Pot);
}

void loop() {
  sensores.actualizar();
  luces.actualizar(sensores.datos().voltaje);
  pantalla.actualizar(sensores.datos(), luces);
  const Medicion& m = sensores.datos();
  Serial.printf("T %.1f C | H %.1f %% | Pote %.2f V | DHT valido %d\r\n",
                m.temperatura, m.humedad, m.voltaje, m.dhtValido);
  delay(250); // Solo para este diagnostico.
}
