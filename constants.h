#pragma once

/*
	Pin assignment
*/
// LCD
<<<<<<< HEAD
const uint8_t RA8875_INT = 8;
const uint8_t RA8875_CS = 10;
const uint8_t RA8875_RESET = 9;
// Shift register
const uint8_t SR_CLOCK_OUT = 17;
const uint8_t SR_DATA_OUT = 18;
const uint8_t SR_LATCH = 19;
const uint8_t SR_OUTPUT_ENABLE = 16;
=======
const uint8_t RA8875_INT = 2;
const uint8_t RA8875_CS = 10;
const uint8_t RA8875_RESET = 9;
// Shift register
const uint8_t SR_CLOCK_OUT = 16;
const uint8_t SR_DATA_OUT = 15;
const uint8_t SR_LATCH = 14;

const uint16_t MAX_RPM = 14000;
>>>>>>> Added icons, calibrated ECU values, moved constants to another file

// Translate float values from CAN BUS
inline float CANIntToFloat(uint16_t floatValue) {
	return floatValue / 1000.0;
}
// Translate kelvin temperature values from CAN BUS
inline float CANKelvinToFloat(uint16_t kelvinValue) {
	float result = kelvinValue / 10.0;
	result = result - 273.15;

	return result;
}

// LCD positioning
const uint16_t lcdWidth = 800;
const uint16_t lcdHeight = 480;
const uint16_t logoPos[] = { ((lcdWidth - logoWidth) / 2), ((lcdHeight - logoHeight) / 2) };
const bool xPos = 0;
const bool yPos = 1;
const uint16_t gearSize = 20;
const uint16_t gearPos[] = { ((lcdWidth - (5 * gearSize)) / 2), ((lcdHeight - (8 * gearSize)) / 2) };
<<<<<<< HEAD
const uint16_t oilLabelPos[] = { 10, 15 };
const uint16_t oilTempPos[] = { oilTempWidth + 30, 5 };
const uint16_t waterLabelPos[] = { 10, 105 };
const uint16_t waterTempPos[] = { waterTempWidth + 30, 100 };
const uint16_t brakesLabelPos[] = { 10, 200 };
const uint16_t brakesTempPos[] = { brakeTempWidth + 30, 190 };
const uint16_t batteryIconPos[] = { 10, 295, };
const uint16_t voltagePos[] = { batteryWidth + 30, 285 };
const uint16_t speedLabelPos[] = { 650, 240 };
const uint16_t speedPos[] = { 510, 240 };
const uint16_t rpmLabelPos[] = { 480, 380 };
const uint16_t rpmPos[] = { 320, 380 };
const uint16_t fanIconPos[] = { (lcdWidth / 2) - (fanWidth / 2), 10 };
=======
const uint16_t oilLabelPos[] = { 10, 10 };
const uint16_t oilTempPos[] = { 10, 80 };
const uint16_t waterLabelPos[] = { 10, 160 };
const uint16_t waterTempPos[] = { 10, 230 };
const uint16_t brakesLabelPos[] = { 10, 300 };
const uint16_t brakesTempPos[] = { 10, 380 };
const uint16_t speedLabelPos[] = { 620, 240 };
const uint16_t speedPos[] = { 520, 240 };
const uint16_t rpmLabelPos[] = { 480, 380 };
const uint16_t rpmPos[] = { 320, 380 };
const uint16_t batteryIconPos[] = { 550, 20, };
const uint16_t voltagePos[] = { 600, 10, };
const uint16_t fanIconPos[] = { 225, 10 };
>>>>>>> Added icons, calibrated ECU values, moved constants to another file

// Shift register values
const uint8_t WARNING_LIGHT1 = 128;
const uint8_t WARNING_LIGHT2 = 64;
const uint8_t WARNING_LIGHT3 = 32;
const uint8_t WARNING_LIGHT4 = 16;
const uint8_t WARNING_LIGHT5 = 8;
const uint8_t WARNING_LIGHT6 = 4;
const uint8_t WARNING_LIGHT7 = 2;
const uint8_t WARNING_LIGHT8 = 1;
const uint8_t SR_LEDBITS = 40;
const uint8_t SR_WARNINGBITS = 8;
const uint16_t RPM_SCALE = 350;
<<<<<<< HEAD
const uint8_t TOTAL_LEDS = 48;

const uint16_t MAX_RPM = 14000;
=======
uint8_t warningSetBits;
uint8_t ledBarSetBits;
uint8_t srWarningCounter;
uint8_t srLedCounter;
>>>>>>> Added icons, calibrated ECU values, moved constants to another file

// Celcius symbol
const static char celcius[3] = { 0xb0, 0x43 };