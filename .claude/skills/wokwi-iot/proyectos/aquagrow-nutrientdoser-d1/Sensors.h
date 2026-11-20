/**
 * @file Sensors.h
 * @brief UltrasonicSensor (HC-SR04) and LoadCellAmplifier (HX711, 5 kg).
 * @author Antonio Rodrigo Duran Diaz - AquaGrow
 */
#pragma once
#include <Arduino.h>
#include <HX711.h>

/** Distance sensor. Returns centimeters, or -1 when there is no echo. */
class UltrasonicSensor {
    static const unsigned long TIMEOUT_US = 30000;
    static constexpr float US_PER_CM = 58.82f;  // 340 m/s, round trip
    int triggerPin, echoPin;

public:
    UltrasonicSensor(int triggerPin, int echoPin) : triggerPin(triggerPin), echoPin(echoPin) {}

    void begin() {
        pinMode(triggerPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }

    float readCm() {
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        unsigned long echoUs = pulseIn(echoPin, HIGH, TIMEOUT_US);
        return echoUs ? roundf(echoUs / US_PER_CM) : -1;
    }
};

/** Load cell amplifier. Returns the tray weight in grams. */
class LoadCellAmplifier {
    static constexpr float SCALE = 0.42f;
    HX711 hx711;
    int dataPin, clockPin;

public:
    LoadCellAmplifier(int dataPin, int clockPin) : dataPin(dataPin), clockPin(clockPin) {}

    void begin() {
        hx711.begin(dataPin, clockPin);
        hx711.set_scale(SCALE);
        hx711.tare();
    }

    float readGrams() { return max(0.0f, roundf(hx711.get_units(1))); }
};
