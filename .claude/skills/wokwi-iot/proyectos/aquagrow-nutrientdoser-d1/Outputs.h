/**
 * @file Outputs.h
 * @brief ServoMotor (nutrient valve) and CharacterLcdDisplay (16x2 I2C).
 * @author Antonio Rodrigo Duran Diaz - AquaGrow
 */
#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

/** Mechanical dosing valve: open at 90 degrees, closed at 0. */
class ServoMotor {
    static const int OPEN_ANGLE = 90, CLOSED_ANGLE = 0;
    Servo servo;
    int pin;
    bool open = false;

public:
    ServoMotor(int pin) : pin(pin) {}
    void begin() { servo.attach(pin); set(false); }
    void set(bool value) { open = value; servo.write(open ? OPEN_ANGLE : CLOSED_ANGLE); }
    bool isOpen() const { return open; }
};

/** 16x2 character display. */
class CharacterLcdDisplay {
    static const int COLUMNS = 16, ROWS = 2;
    LiquidCrystal_I2C lcd{0x27, COLUMNS, ROWS};

    void print(int row, const String& text) {
        lcd.setCursor(0, row);
        lcd.print((text + "                ").substring(0, COLUMNS));
    }

public:
    void begin() { lcd.init(); lcd.backlight(); }
    void show(const String& line1, const String& line2) { print(0, line1); print(1, line2); }
};
