/**
 * @file TelemetryPackage.h
 * @brief Operation metrics for the Device Maker.
 * @author Antonio Rodrigo Duran Diaz - AquaGrow
 */
#pragma once
#include <Arduino.h>

struct TelemetryPackage {
    unsigned dosingCycles = 0, proximityEvents = 0, modeTransitions = 0;
    String tankState = "OK";

    void print() const {
        Serial.printf("[TELEMETRY] dosingCycles=%u proximityEvents=%u modeTransitions=%u tank=%s\n",
                      dosingCycles, proximityEvents, modeTransitions, tankState.c_str());
    }
};
