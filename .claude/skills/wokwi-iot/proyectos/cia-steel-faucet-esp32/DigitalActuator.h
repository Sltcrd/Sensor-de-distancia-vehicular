/*
  DigitalActuator.h
  Cia Steel Faucet - Embedded Application (Modest IoT based)

  On/off actuators: Led (indicator) and Relay (water valve).
  Commands: TURN_ON, TURN_OFF, TOGGLE. setState() only sends a command
  when the state really changes.

  Created 19 November 2026
  By Antonio Rodrigo Duran Diaz
*/

#ifndef DIGITAL_ACTUATOR_H
#define DIGITAL_ACTUATOR_H

#include "ModestIoT.h"

class DigitalActuator : public Actuator {
protected:
    bool turnedOn = false;

    /** Writes the state to the hardware. */
    virtual void apply(bool turnOn) {
        turnedOn = turnOn;
        digitalWrite(pin, turnedOn ? HIGH : LOW);
    }

public:
    explicit DigitalActuator(int signalPin) : Actuator(signalPin) {}

    void begin() override {
        pinMode(pin, OUTPUT);
        apply(false);
    }

    void handle(Command command) override {
        switch (command.type) {
            case Command::TURN_ON:  apply(true); break;
            case Command::TURN_OFF: apply(false); break;
            case Command::TOGGLE:   apply(!turnedOn); break;
            default:                break; // Unknown command: ignored
        }
    }

    void setState(bool shouldBeOn) {
        if (shouldBeOn == turnedOn) {
            return;
        }
        if (shouldBeOn) {
            handle(Command(Command::TURN_ON));
        } else {
            handle(Command(Command::TURN_OFF));
        }
    }

    bool isOn() const { return turnedOn; }
};

/** Light indicator. */
class Led : public DigitalActuator {
public:
    explicit Led(int signalPin) : DigitalActuator(signalPin) {}
};

/** Relay module that drives the water valve. */
class Relay : public DigitalActuator {
public:
    explicit Relay(int signalPin) : DigitalActuator(signalPin) {}
};

#endif // DIGITAL_ACTUATOR_H
