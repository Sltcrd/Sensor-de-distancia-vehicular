/*
  ModestIoT.h
  Cia Steel Faucet - Embedded Application (Modest IoT based)

  Core of the framework (ESP32):
  * Event / Command: value objects (events inform, commands order).
  * Sensor: reads hardware and notifies an Event to its handler (the Device).
  * Actuator: executes Commands.
  * Device: mediator that owns sensors and actuators.
  * PeriodicTimer / OneShotTimer: non-blocking timing with millis().

  Created 19 November 2026
  By Antonio Rodrigo Duran Diaz
*/

#ifndef MODEST_IOT_H
#define MODEST_IOT_H

#include <Arduino.h>

/** Event emitted by a sensor: which sensor (sourceId) and what happened (type). */
struct Event {
    static const int NONE = 0;     ///< Nothing to notify (not sent).
    static const int MEASURED = 1; ///< New valid value available.
    static const int FAULT = 2;    ///< Sensor error or invalid reading.

    int sourceId;
    int type;
    Event(int source, int eventType) : sourceId(source), type(eventType) {}
};

/** Command executed by an actuator. */
struct Command {
    static const int TURN_ON = 1;
    static const int TURN_OFF = 2;
    static const int TOGGLE = 3;

    int type;
    explicit Command(int commandType) : type(commandType) {}
};

/** Interface for components that react to events. */
class EventHandler {
public:
    virtual void on(Event event) = 0;
    virtual ~EventHandler() {}
};

/** Interface for components that execute commands. */
class CommandHandler {
public:
    virtual void handle(Command command) = 0;
    virtual ~CommandHandler() {}
};

/** Base class for sensors. Subclasses implement read() and return an Event type. */
class Sensor {
protected:
    int id;                     ///< Identifier used by the Device to know the source.
    int pin;                    ///< Main signal pin.
    EventHandler* eventHandler; ///< Receiver of the events (the Device).
    float value = 0.0f;         ///< Last valid value.

    /** Reads the hardware, updates value and returns Event::MEASURED, FAULT or NONE. */
    virtual int read() = 0;

public:
    Sensor(int sensorId, int signalPin, EventHandler* handler)
        : id(sensorId), pin(signalPin), eventHandler(handler) {}
    virtual ~Sensor() {}

    /** Configures the hardware. Call it from Device::setup(). */
    virtual bool begin() { return true; }

    /** Takes a reading and notifies the handler (unless there is nothing new). */
    void measure() {
        int eventType = read();
        if (eventType != Event::NONE && eventHandler != nullptr) {
            eventHandler->on(Event(id, eventType));
        }
    }

    float getValue() const { return value; }
    int getId() const { return id; }
};

/** Base class for actuators. */
class Actuator : public CommandHandler {
protected:
    int pin;

public:
    explicit Actuator(int signalPin) : pin(signalPin) {}

    /** Configures the hardware. Call it from Device::setup(). */
    virtual void begin() {}
};

/** Base class for devices: mediator between sensors and actuators. */
class Device : public EventHandler {
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
};

/** Repeats an action every interval without delay(). */
class PeriodicTimer {
private:
    unsigned long intervalInMilliseconds;
    unsigned long lastTimestamp = 0;

public:
    explicit PeriodicTimer(unsigned long interval) : intervalInMilliseconds(interval) {}

    /** Returns true once each time the interval has elapsed. */
    bool isDue() {
        unsigned long now = millis();
        if (now - lastTimestamp >= intervalInMilliseconds) {
            lastTimestamp = now;
            return true;
        }
        return false;
    }
};

/** Runs once for a duration (e.g. keep a relay open for 5 s) without delay(). */
class OneShotTimer {
private:
    unsigned long durationInMilliseconds;
    unsigned long startTimestamp = 0;
    bool running = false;

public:
    explicit OneShotTimer(unsigned long duration) : durationInMilliseconds(duration) {}

    void start() {
        startTimestamp = millis();
        running = true;
    }

    void stop() { running = false; }

    bool isRunning() const { return running; }

    /** Returns true only once, when the duration has just finished. */
    bool hasExpired() {
        if (running && millis() - startTimestamp >= durationInMilliseconds) {
            running = false;
            return true;
        }
        return false;
    }
};

#endif // MODEST_IOT_H
