#pragma once

#include "wled.h"

const int pwmChannel = 0;
const int resolution = 8;

class Nema23Mod : public Usermod {
  private:
    long _stepFrequency = 2000;
    bool _enabled = false;
    bool _direction = true;
    byte _stepPin = 15;
    byte _enabledPin = 4;
    byte _directionPin = 2;

  public:
    void setup() {
        initPins();
    }

    void initPins() {
        ledcSetup(pwmChannel, _stepFrequency, resolution);
        ledcAttachPin(_stepPin, pwmChannel);
        ledcWrite(pwmChannel, 128);

        if (_enabledPin > 0) {
            pinMode(_enabledPin, OUTPUT);
            digitalWrite(_enabledPin, _enabled ? LOW : HIGH);
        }

        if (_directionPin > 0) {
            pinMode(_directionPin, OUTPUT);
            digitalWrite(_directionPin, _direction ? HIGH : LOW);
        }
    }

    void loop() {
    }

    void addToConfig(JsonObject& root) {
      JsonObject config = root.createNestedObject(FPSTR(USERMOD_NAME));
      config[F(STEP_PIN)] = _stepPin;
      config[F(ENABLED)] = _enabled;
      config[F(ENABLED_PIN)] = _enabledPin;
      config[F(DIRECTION)] = _direction;
      config[F(DIRECTION_PIN)] = _directionPin;
      config[F(STEP_FREQUENCY)] = _stepFrequency;

      initPins();
    }

    bool readFromConfig(JsonObject &root) {
        JsonObject top = root[FPSTR(USERMOD_NAME)];
        if (top.isNull()) {
            return false;
        }

        bool configComplete = !top.isNull();
        configComplete &= getJsonValue(top[FPSTR(STEP_PIN)], _stepPin, _stepPin);
        configComplete &= getJsonValue(top[FPSTR(STEP_FREQUENCY)], _stepFrequency, _stepFrequency);
        configComplete &= getJsonValue(top[FPSTR(ENABLED)], _enabled, _enabled);
        configComplete &= getJsonValue(top[FPSTR(ENABLED_PIN)], _enabledPin, _enabledPin);
        configComplete &= getJsonValue(top[FPSTR(DIRECTION)], _direction, _direction);
        configComplete &= getJsonValue(top[FPSTR(DIRECTION_PIN)], _directionPin, _directionPin);

        return configComplete;
        
    }

  static const char USERMOD_NAME[];
  static const char STEP_PIN[];
  static const char STEP_FREQUENCY[];
  static const char ENABLED[];
  static const char ENABLED_PIN[];
  static const char DIRECTION[];
  static const char DIRECTION_PIN[];
};

// strings to reduce flash memory usage (used more than twice)
const char Nema23Mod::USERMOD_NAME[] PROGMEM = "NEMA23 Motor Control";
const char Nema23Mod::STEP_PIN[] PROGMEM = "step-pin";
const char Nema23Mod::STEP_FREQUENCY[] PROGMEM = "step-frequency";
const char Nema23Mod::ENABLED[] PROGMEM = "enabled";
const char Nema23Mod::ENABLED_PIN[] PROGMEM = "enabled-pin";
const char Nema23Mod::DIRECTION[] PROGMEM = "direction";
const char Nema23Mod::DIRECTION_PIN[] PROGMEM = "direction-pin";