#pragma once
#include <UniversalTelegramBot.h>
#include <Sensores.h>
#include <Luces.h>
#include <Pantalla.h>
#include <Telemetria.h>
class Comandos
{
public:
    Comandos(UniversalTelegramBot &bot, Sensores &sensores, Luces &luces,
             Pantalla &pantalla, Telemetria &nube);
    void atender();

private:
    UniversalTelegramBot &bot;
    Sensores &sensores;
    Luces &luces;
    Pantalla &pantalla;
    Telemetria &nube;
    unsigned long ultimaConsulta = 0;
    String normalizar(String texto) const;
    String estado() const;
    bool ejecutar(const String &texto, String &respuesta);
};