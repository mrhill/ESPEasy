#include "Alexa.h"
#include "../ESPEasyCore/ESPEasy_Log.h"
#include "../ESPEasyCore/ESPEasyRules.h"
#include "../Globals/Settings.h"

ShadesAlexa alexa;

static void deviceCallback(EspalexaDevice* d)
{
    String log = "Alexa: " + d->getName() + ' ' + d->getId() + ' ' + d->getValue();
    addLog(LOG_LEVEL_INFO, log);

    String event = String(d->getValue() ? F("up") : F("down")) + d->getId();
    rulesProcessing(event);
}

static String getAlexaDeviceName(int devIdx) {
    String name = Settings.getHostname(false);
    if (name.length() && !isdigit(name[name.length()-1]))
        name += Settings.Unit;
    name += F(" Motor ");
    name += devIdx;
    return name;
}

void ShadesAlexa::init(ESP8266WebServer* server)
{
    this->addDevice(getAlexaDeviceName(1), deviceCallback);
    this->addDevice(getAlexaDeviceName(2), deviceCallback);
    this->begin(server);
}

void ShadesAlexa::updateDeviceNames()
{
    this->renameDevice(1, getAlexaDeviceName(1));
    this->renameDevice(2, getAlexaDeviceName(2));
}
