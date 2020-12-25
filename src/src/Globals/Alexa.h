#pragma once

#include "../ESPEasyCore/ESPEasy_Log.h"

#define ESPALEXA_MAXDEVICES 2

//#define ESPALEXA_DEBUG
#include <Espalexa.h>

class ShadesAlexa : public Espalexa {
public:
    void init(ESP8266WebServer* server);
    void updateDeviceNames();
};

extern ShadesAlexa alexa;
