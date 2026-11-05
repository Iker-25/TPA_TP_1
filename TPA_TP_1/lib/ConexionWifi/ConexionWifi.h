#pragma once

class ConexionWifi {
public:
  void iniciar(const char* ssid, const char* clave);
  void actualizar();

private:
  bool conectada = false;
  unsigned long ultimoIntento = 0;
};
