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

void ShadesAlexa::init(ESP8266WebServer* server)
{
    String hostName = Settings.getHostname(true);
    this->addDevice(hostName + F(" Motor 1"), deviceCallback);
    this->addDevice(hostName + F(" Motor 2"), deviceCallback);
    this->begin(server);
}

void ShadesAlexa::updateDeviceNames()
{
    String hostName = Settings.getHostname(true);
    this->renameDevice(1, hostName + F(" Motor 1"));
    this->renameDevice(2, hostName + F(" Motor 2"));
}
