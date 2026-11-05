#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Sensores.h>
#include <Luces.h>

enum class Vista { Inicio, Leds, Pot, Dht, Error };

class Pantalla {
public:
  void iniciar();
  void seleccionar(Vista nueva);
  void registrar(const String& comando);
  void actualizar(const Medicion& medicion, const Luces& luces);
private:
  Adafruit_SSD1306 oled{128, 64, &Wire, -1};
  Vista vista = Vista::Inicio;
  String historial[3];
  bool lista = false;
  bool cambio = true;
  unsigned long ultimoCuadro = 0;
};
