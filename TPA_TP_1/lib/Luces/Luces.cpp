#include "Luces.h"

Luces::Luces(uint8_t pinVerde, uint8_t pinAzul)
    : pinVerde(pinVerde), pinAzul(pinAzul) {}

void Luces::iniciar() {
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);
  escribir();
}

void Luces::escribir() {
  digitalWrite(pinVerde, verde ? HIGH : LOW);
  digitalWrite(pinAzul, azul ? HIGH : LOW);
}

void Luces::activarAuto(float voltaje) {
  if (automatico) return; // Repetir /autoon no desplaza la referencia.
  base = voltaje;
  automatico = true;
  actualizar(voltaje);
}

void Luces::desactivarAuto() {
  automatico = false;
}

bool Luces::ajustarUmbral(float valor) {
  bool valido = valor >= 1.0f && valor <= 1.5f;
  delta = valido ? valor : 0.5f;
  return valido;
}

void Luces::actualizar(float voltaje) {
  if (!automatico) return;
  bool nuevoAzul = voltaje > limite();
  if (nuevoAzul != azul) {
    azul = nuevoAzul;
    escribir();
  }
}

bool Luces::manual(bool elegirAzul, bool encender) {
  if (automatico) return false;
  if (elegirAzul) azul = encender;
  else verde = encender;
  escribir();
  return true;
}
