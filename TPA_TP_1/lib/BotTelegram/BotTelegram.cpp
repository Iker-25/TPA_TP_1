#include "BotTelegram.h"
#include <WiFi.h>
#include <time.h>
BotTelegram::BotTelegram(const char *token, Sensores &sensores, Luces &luces,
                         Pantalla &pantalla, Telemetria &nube)
    : bot(token, cliente), comandos(bot, sensores, luces, pantalla, nube),
      habilitado(String(token).indexOf(':') > 0) {}
void BotTelegram::iniciar()
{
    cliente.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    cliente.setHandshakeTimeout(5);
    cliente.setTimeout(5); // Segundos en Arduino-ESP32 2.x.
    bot.longPoll = 0;
    bot.waitForResponse = 5000; // Milisegundos.
    if (!habilitado)
        Serial.println("Completar TOKEN_BOT en Secretos.h");
}
void BotTelegram::actualizar()
{
    if (!habilitado || WiFi.status() != WL_CONNECTED)
        return;
    if (time(nullptr) < 1700000000)
        return;
    comandos.atender();
}
