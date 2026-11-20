/**
 * @file Device.h
 * @brief AquaGrow NutrientDoser Node D1: doses nutrients when the substrate is light
 *        and a gardener is near, and locks the dosing when the tank is empty.
 * @author Antonio Rodrigo Duran Diaz - AquaGrow
 */
#pragma once
#include "Sensors.h"
#include "Outputs.h"
#include "TelemetryPackage.h"

enum class Mode { IDLE, DOSING, EMPTY };

class Device {
    static const int TANK_HEIGHT_CM = 40, FULL_DISTANCE_CM = 6, TANK_CAPACITY_ML = 5000;
    static const int PROXIMITY_CM = 25, LOW_LOAD_G = 500, TARGET_LOAD_G = 1500;
    static const unsigned long SAMPLE_MS = 200, REPORT_MS = 2500;

    UltrasonicSensor tankSensor{5, 18}, gardenerSensor{25, 26};
    LoadCellAmplifier loadCell{32, 33};
    ServoMotor doser{13};
    CharacterLcdDisplay display;
    TelemetryPackage telemetry;

    Mode mode = Mode::IDLE;
    int tankPercent = 100;
    float grams = 0;
    bool near = false, tankEmpty = false;
    unsigned long sampledAt = 0, reportedAt = 0;

    static const char* name(Mode m) { return m == Mode::IDLE ? "IDLE" : m == Mode::DOSING ? "DOSING" : "EMPTY"; }

    void setMode(Mode next) {
        if (next == mode) return;
        Serial.printf("[MODE] %s -> %s\n", name(mode), name(next));
        mode = next;
        telemetry.modeTransitions++;
        if (mode == Mode::DOSING) telemetry.dosingCycles++;
        doser.set(mode == Mode::DOSING);
    }

    void update() {
        float tankCm = tankSensor.readCm(), gardenerCm = gardenerSensor.readCm();
        grams = loadCell.readGrams();
        if (tankCm >= 0) {
            tankPercent = constrain((TANK_HEIGHT_CM - tankCm) * 100 / (TANK_HEIGHT_CM - FULL_DISTANCE_CM), 0, 100);
            tankEmpty = tankCm >= TANK_HEIGHT_CM;
        }
        bool wasNear = near;
        near = gardenerCm >= 0 && gardenerCm <= PROXIMITY_CM;
        if (near && !wasNear) telemetry.proximityEvents++;
        telemetry.tankState = tankEmpty ? "EMPTY" : "OK";

        if (tankEmpty) setMode(Mode::EMPTY);
        else if (mode == Mode::DOSING) setMode(grams >= TARGET_LOAD_G ? Mode::IDLE : Mode::DOSING);
        else setMode(grams < LOW_LOAD_G && near ? Mode::DOSING : Mode::IDLE);

        display.show(tankEmpty ? "Tank: Empty" : "Tank: " + String(tankPercent) + "%",
                     "Weight: " + String((int)grams) + " g");
    }

public:
    void setup() {
        Serial.begin(115200);
        Serial.println("AquaGrow - NutrientDoser Node D1 | Developer: Antonio Rodrigo Duran Diaz");
        tankSensor.begin();
        gardenerSensor.begin();
        loadCell.begin();
        doser.begin();
        display.begin();
    }

    void loop() {
        unsigned long now = millis();
        if (now - sampledAt >= SAMPLE_MS) {
            sampledAt = now;
            update();
        }
        if (now - reportedAt >= REPORT_MS) {
            reportedAt = now;
            Serial.printf("[REPORT] Tank: %d%% (%d mL) | Weight: %.0f g | Gardener: %s | Mode: %s\n",
                          tankPercent, tankPercent * TANK_CAPACITY_ML / 100, grams, near ? "NEAR" : "FAR", name(mode));
            telemetry.print();
        }
    }
};
