#include "Pantalla.h"

void Pantalla::iniciar() {
  Wire.begin(21, 22);
  lista = oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (!lista) {
    Serial.println("No se pudo iniciar la OLED");
    return;
  }
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextWrap(false);
  cambio = true;
}

void Pantalla::seleccionar(Vista nueva) {
  vista = nueva;
  cambio = true;
}

void Pantalla::registrar(const String& comando) {
  historial[2] = historial[1];
  historial[1] = historial[0];
  historial[0] = comando.substring(0, 21);
  cambio = true;
}

void Pantalla::actualizar(const Medicion& medicion, const Luces& luces) {
  if (!lista) return;
  unsigned long ahora = millis();
  if (!cambio && ahora - ultimoCuadro < 250UL) return;
  ultimoCuadro = ahora;
  cambio = false;

  oled.clearDisplay();
  oled.setCursor(0, 0);
  switch (vista) {
    case Vista::Inicio:
      oled.println("TPI 01 - ESP32");
      break;
    case Vista::Leds:
      oled.printf("Verde: %s\nAzul: %s",
                  luces.verdeOn() ? "encendido" : "apagado",
                  luces.azulOn() ? "encendido" : "apagado");
      break;
    case Vista::Pot:
      oled.printf("Potenciometro\n%.2f V", medicion.voltaje);
      break;
    case Vista::Dht:
      if (medicion.dhtValido) {
        oled.printf("Temp: %.1f C\nHum: %.1f %%",
                    medicion.temperatura, medicion.humedad);
      } else {
        oled.print("DHT sin lectura");
      }
      break;
    case Vista::Error:
      oled.print("Comando desconocido");
      break;
  }

  oled.setCursor(0, 32);
  oled.print("Ultimos comandos:");
  for (int i = 0; i < 3; ++i) {
    oled.setCursor(0, 40 + i * 8);
    oled.print(historial[i]);
  }
  oled.display();
}
