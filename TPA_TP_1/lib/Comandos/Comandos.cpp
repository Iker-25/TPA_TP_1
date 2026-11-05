#include "Comandos.h"
#include <stdlib.h>
#include <math.h>
Comandos::Comandos(UniversalTelegramBot &bot, Sensores &sensores, Luces &luces,
                   Pantalla &pantalla, Telemetria &nube)
    : bot(bot), sensores(sensores), luces(luces), pantalla(pantalla), nube(nube) {}
String Comandos::normalizar(String texto) const
{
    texto.trim();
    const char *nombres[] = {
        "/start", "/estado", "/autoon", "/autooff",
        "/azulon", "/azuloff", "/verdeon", "/verdeoff",
        "/umbral", "/thingup", "/displayled", "/displaypot", "/displaydht"};
    for (int i = 0; i < 13; ++i)
    {
        if (texto == String(i + 1))
            return String(nombres[i]);
    }
    if (texto.startsWith("9 "))
        return String("/umbral ") + texto.substring(2);
    return texto;
}
String Comandos::estado() const
{
    const Medicion &m = sensores.datos();
    String respuesta;
    if (m.dhtValido)
    {
        respuesta = "Temperatura: " + String(m.temperatura, 1) + " C\n" + "Humedad: " + String(m.humedad, 1) + " %";
    }
    else
    {
        respuesta = "DHT22 sin lectura valida";
    }
    respuesta += "\nPotenciometro: " + String(m.voltaje, 2) + " V";
    respuesta += String("\nVerde: ") + (luces.verdeOn() ? "encendido" : "apagado");
    respuesta += String("\nAzul: ") + (luces.azulOn() ? "encendido" : "apagado");
    respuesta += String("\nModo: ") + (luces.esAuto() ? "automatico" : "manual");
    respuesta += "\nUmbral: " + String(luces.umbral(), 2) + " V";
    if (luces.esAuto())
    {
        respuesta += "\nBase: " + String(luces.referencia(), 2) + " V";
        respuesta += "\nLimite: " + String(luces.limite(), 2) + " V";
    }
    return respuesta;
}
bool Comandos::ejecutar(const String &texto, String &respuesta)
{
    const Medicion &m = sensores.datos();
    if (texto == "/start")
    {
        respuesta = "🌡 <b>Estacion ambiental</b>\n"
                    "Escribi el numero o el comando:\n"
                    "1 /start\n2 /estado\n"
                    "💡 <b>Luces</b>\n3 /autoon\n4 /autooff\n"
                    "5 /azulon\n6 /azuloff\n7 /verdeon\n8 /verdeoff\n"
                    "9 1.2 = /umbral 1.2\n"
                    "☁ <b>Adafruit IO</b>\n10 /thingup\n"
                    "🖥 <b>Pantalla</b>\n11 /displayled\n12 /displaypot\n13 /displaydht";
        return true;
    }
    if (texto == "/estado")
    {
        respuesta = estado();
        return true;
    }
    if (texto == "/thingup")
    {
        return nube.enviar(m, respuesta);
    }
    if (texto == "/autoon")
    {
        luces.activarAuto(m.voltaje);
        respuesta = "Automatico. Base: " + String(luces.referencia(), 2) + " V.";
        return true;
    }
    if (texto == "/autooff")
    {
        luces.desactivarAuto();
        respuesta = "Modo manual activo.";
        return true;
    }
    if (texto == "/umbral" || texto.startsWith("/umbral "))
    {
        String argumento = texto.substring(7);
        argumento.trim();
        argumento.replace(',', '.');
        const char *inicio = argumento.c_str();
        char *fin = nullptr;
        float valor = strtof(inicio, &fin);
        if (fin == inicio || *fin != '\0' || !isfinite(valor))
        {
            respuesta = "Uso: /umbral 1.2 (entre 1.0 y 1.5 V).";
            return false;
        }
        bool valido = luces.ajustarUmbral(valor);
        luces.actualizar(m.voltaje);
        respuesta = valido ? "Umbral: " + String(valor, 2) + " V."
                           : String("Fuera de rango. Se restauro 0.50 V.");
        return valido;
    }
    if (texto == "/azulon" || texto == "/azuloff" ||
        texto == "/verdeon" || texto == "/verdeoff")
    {
        bool azul = texto.startsWith("/azul");
        bool encender = texto.endsWith("on");
        if (!luces.manual(azul, encender))
        {
            respuesta = "Primero envia /autooff para usar el modo manual.";
            return false;
        }
        respuesta = String(azul ? "Azul " : "Verde ") + (encender ? "encendido." : "apagado.");
        return true;
    }
    if (texto.startsWith("/display"))
    {
        if (texto == "/displayled")
            pantalla.seleccionar(Vista::Leds);
        else if (texto == "/displaypot")
            pantalla.seleccionar(Vista::Pot);
        else if (texto == "/displaydht")
            pantalla.seleccionar(Vista::Dht);
        else
        {
            pantalla.seleccionar(Vista::Error);
            respuesta = "Componente de display desconocido.";
            return false;
        }
        respuesta = "Vista OLED actualizada.";
        return true;
    }
    respuesta = "Comando desconocido. Escribi /start.";
    return false;
}
void Comandos::atender()
{
    unsigned long ahora = millis();
    if (ahora - ultimaConsulta < 1000UL)
        return;
    ultimaConsulta = ahora;
    int cantidad = bot.getUpdates(bot.last_message_received + 1);
    for (int i = 0; i < cantidad; ++i)
    {
        String texto = normalizar(bot.messages[i].text);
        sensores.actualizar();
        luces.actualizar(sensores.datos().voltaje);
        String respuesta;
        bool exito = ejecutar(texto, respuesta);
        if (exito)
            pantalla.registrar(texto);
        pantalla.actualizar(sensores.datos(), luces);
        bot.sendMessage(bot.messages[i].chat_id, respuesta,
                        texto == "/start" ? "HTML" : "");
    }
}
