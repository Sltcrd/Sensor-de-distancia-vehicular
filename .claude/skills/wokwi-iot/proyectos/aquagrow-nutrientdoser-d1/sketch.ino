/**
 * @file sketch.ino
 * @brief AquaGrow NutrientDoser Node D1.
 * @author Antonio Rodrigo Duran Diaz
 */
#include "Device.h"

Device device;

void setup() { device.setup(); }
void loop() { device.loop(); }
