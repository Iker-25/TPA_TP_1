#pragma once
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Comandos.h>
class BotTelegram
{
public:
    BotTelegram(const char *token, Sensores &sensores, Luces &luces,
                Pantalla &pantalla, Telemetria &nube);
    void iniciar();
    void actualizar();

private:
    WiFiClientSecure cliente;
    UniversalTelegramBot bot;
    Comandos comandos;
    bool habilitado;
};