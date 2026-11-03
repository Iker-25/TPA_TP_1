#include "PruebaCircuito.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>

namespace {
constexpr uint8_t LED_VERDE = 23, LED_AZUL = 2, POT = 32, PIN_DHT = 33;
constexpr uint8_t DIRECCION_OLED = 0x3C;
constexpr unsigned long INTERVALO_MS = 2100;
DHT dht(PIN_DHT, DHT22);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
bool mostrarEnOled = false;
bool verdeEncendido = false;
unsigned long ultimaPrueba;
}

void PruebaCircuito::iniciar() {
  Serial.begin(115200);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
  analogReadResolution(12);
  dht.begin();
  Wire.begin(21, 22);
  Wire.beginTransmission(DIRECCION_OLED);
  mostrarEnOled = Wire.endTransmission() == 0 &&
                  oled.begin(SSD1306_SWITCHCAPVCC, DIRECCION_OLED);
  if (mostrarEnOled) {
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.print("Prueba de circuito");
    oled.display();
  } else {
    Serial.println("Revisar OLED: SDA 21, SCL 22, direccion 0x3C");
  }
  ultimaPrueba = millis();
}

void PruebaCircuito::actualizar() {
  unsigned long ahora = millis();
  if (ahora - ultimaPrueba < INTERVALO_MS) return;
  ultimaPrueba = ahora;
  verdeEncendido = !verdeEncendido;
  digitalWrite(LED_VERDE, verdeEncendido ? HIGH : LOW);
  digitalWrite(LED_AZUL, verdeEncendido ? LOW : HIGH);

  float voltaje = analogRead(POT) * 3.3f / 4095.0f;
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  char ambiente[48];
  if (isnan(t) || isnan(h)) {
    snprintf(ambiente, sizeof(ambiente), "DHT sin lectura");
  } else {
    snprintf(ambiente, sizeof(ambiente), "T:%.1f C H:%.1f %%", t, h);
  }
  const char* color = verdeEncendido ? "verde" : "azul";
  Serial.printf("%s | Pote: %.2f V | LED: %s\n", ambiente, voltaje, color);
  if (mostrarEnOled) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.printf("Prueba de circuito\n%s\nPote: %.2f V\nLED: %s", ambiente, voltaje, color);
    oled.display();
  }
}
