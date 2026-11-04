#pragma once
#include <Arduino.h>

class Luces {
public:
  Luces(uint8_t pinVerde, uint8_t pinAzul);
  void iniciar();
  void activarAuto(float voltaje);
  void desactivarAuto();
  bool ajustarUmbral(float valor);
  void actualizar(float voltaje);
  bool manual(bool elegirAzul, bool encender);
  bool esAuto() const { return automatico; }
  bool azulOn() const { return azul; }
  bool verdeOn() const { return verde; }
  float umbral() const { return delta; }
  float referencia() const { return base; }
  float limite() const { return base + delta; }
private:
  uint8_t pinVerde, pinAzul;
  bool verde = false, azul = false, automatico = false;
  float base = 0, delta = 0.5f;
  void escribir();
};
