/******************************************************************
Team Sleipnir steering wheel LCD

Hardware:
	- Teensy 3.2
	- Adafruit RA8875 touch LCD controller
	- 800 x 480 LCD
	- NPIC6C4894 shift registers
	- MCP2551 CAN transceiver

Written by Einar Arnason
******************************************************************/

#include <FlexCAN.h>
#include <SPI.h>
#include <stdint.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include "./images/logo.h"
#include "./images/fanIcon.h"
#include "./images/brakeTempIcon.h"
#include "./images/oilTempIcon.h"
#include "./images/waterTempIcon.h"
#include "./images/batteryIcon.h"
#include "constants.h"
#include "Vehicle.h"

// LCD driver
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

// CAN BUS driver
class CanListener : public CANListener {
public:
	//overrides the parent version
	bool frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller);
};

CanListener canListener;
unsigned int txTimer, rxTimer;

// Vehicle class instance
Vehicle vehicle;

// Shift register variables
uint8_t warningSetBits;
uint8_t ledBarSetBits;
uint8_t srWarningCounter;
uint8_t srLedCounter;

bool CanListener::frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller) {
	
	switch (frame.id) {
	case 1:
		vehicle.rpm = frame.buf[0] | (frame.buf[1] << 8);
		Serial.println(vehicle.rpm);
		vehicle.voltage = CANIntToFloat(frame.buf[2] | (frame.buf[3] << 8));
		vehicle.waterTemp = CANKelvinToFloat(frame.buf[4] | (frame.buf[5] << 8));
		vehicle.speed = frame.buf[6] | (frame.buf[7] << 8);
		break;
	case 2:
		vehicle.oilTemp = CANKelvinToFloat(frame.buf[0] | (frame.buf[1] << 8));
		vehicle.gear = frame.buf[2] | (frame.buf[3] << 8);
		break;
	}

	return true;
}

void printIcons() {
	tft.graphicsMode();
	// Draw the icon for cooling fan
	tft.drawXBitmap(
		fanIconPos[xPos], 
		fanIconPos[yPos], 
		fanIcon, 
		fanWidth, 
		fanHeight, 
		RA8875_WHITE
	);
	// Draw battery icon
	tft.drawXBitmap(
		batteryIconPos[xPos], 
		batteryIconPos[yPos], 
		batteryIcon, 
		batteryWidth, 
		batteryHeight, 
		RA8875_WHITE
	);
	// Draw engine oil temperature icon
	tft.drawXBitmap(
		oilLabelPos[xPos], 
		oilLabelPos[yPos], 
		oilTempIcon, 
		oilTempWidth, 
		oilTempHeight, 
		RA8875_WHITE
	);
	// Draw coolant temperature icon
	tft.drawXBitmap(
		waterLabelPos[xPos],
		waterLabelPos[yPos],
		waterTempIcon,
		waterTempWidth,
		waterTempHeight,
		RA8875_WHITE
	);
	// Draw brake temperature icon
	tft.drawXBitmap(
		brakesLabelPos[xPos],
		brakesLabelPos[yPos],
		brakeTempIcon,
		brakeTempWidth,
		brakeTempHeight,
		RA8875_WHITE
	);
	tft.textMode();
}

void printLabels() {
	tft.textEnlarge(3);
	tft.textSetCursor(speedLabelPos[xPos], speedLabelPos[yPos]);
	tft.textWrite("km/h");
	tft.textSetCursor(rpmLabelPos[xPos], rpmLabelPos[yPos]);
	tft.textColor(RA8875_WHITE, RA8875_BLACK);
	tft.textWrite(" RPM");
}

void printFrames() {
	tft.drawFastVLine(280, 0, 480, RA8875_WHITE);
	tft.drawFastVLine(500, 0, 480 - (480 - 350), RA8875_WHITE);
	tft.drawFastHLine(280, 350, (800 - 280), RA8875_WHITE);
	tft.drawFastHLine(0, oilTempPos[yPos] + oilTempHeight + 30, 280, RA8875_WHITE);
	tft.drawFastHLine(0, waterTempPos[yPos] + waterTempHeight + 30, 280, RA8875_WHITE);
	tft.drawFastHLine(0, brakesTempPos[yPos] + brakeTempHeight + 30, 280, RA8875_WHITE);
	tft.drawFastHLine(0, voltagePos[yPos] + batteryHeight + 30, 280, RA8875_WHITE);
}

void printInt(const uint16_t& x,
	const uint16_t& y,
	const uint16_t& value,
	uint16_t& prevValue,
	char* charValue,
	const uint8_t& fontSize,
	bool warning) {

	sprintf(charValue, "%*d", sizeof(charValue), value);
	prevValue = value;
	printValue(x, y, charValue, fontSize, warning);
}

void printFloat(
	const uint16_t& x,
	const uint16_t& y,
	const float& value,
	float& prevValue,
	char* charValue,
	const uint8_t& fontSize,
	bool warning) {

	sprintf(charValue, "%*.01f", sizeof(charValue), value);
	prevValue = value;
	printValue(x, y, charValue, fontSize, warning);
}

void printFloatNoPoint(
	const uint16_t& x,
	const uint16_t& y,
	const float& value,
	float& prevValue,
	char* charValue,
	const uint8_t& fontSize,
	bool warning) {

	sprintf(charValue, "%*d", sizeof(charValue), (int)value);
	prevValue = value;
	printValue(x, y, charValue, fontSize, warning);
}

void printValue(
	const uint16_t& x,
	const uint16_t& y,
	char* charValue, 
	const uint8_t& fontSize,
	bool warning
	) {
	tft.textMode();
	tft.textSetCursor(x, y);
	tft.textEnlarge(fontSize);
	if (warning) {
		tft.textColor(RA8875_BLACK, RA8875_RED);
	}
	else {
		tft.textColor(RA8875_WHITE, RA8875_BLACK);
	}
	tft.textWrite(charValue);
	tft.textColor(RA8875_WHITE, RA8875_BLACK);
}

void printValues() {
	if (vehicle.voltage != vehicle.prevVoltage) {
		printFloat(
			voltagePos[xPos],
			voltagePos[yPos],
			vehicle.voltage,
			vehicle.prevVoltage,
			vehicle.voltageDisp,
			3,
			false
		);
		tft.textWrite("v");
	}
	if (vehicle.prevOilTemp != vehicle.oilTemp) {
		if (vehicle.oilTemp > 250) {
			printFloatNoPoint(
				oilTempPos[xPos], 
				oilTempPos[yPos], 
				vehicle.oilTemp,
				vehicle.prevOilTemp,
				vehicle.oilTempDisp,
				3, 
				true
			);
		}
		else {
			printFloatNoPoint(
				oilTempPos[xPos], 
				oilTempPos[yPos], 
				vehicle.oilTemp,
				vehicle.prevOilTemp,
				vehicle.oilTempDisp,
				3, 
				false
			);
		}
		tft.textEnlarge(2);
		tft.textWrite(celcius);
	}
	if (vehicle.prevWaterTemp != vehicle.waterTemp) {
		if (vehicle.waterTemp > 250) {
			printFloatNoPoint(
				waterTempPos[xPos], 
				waterTempPos[yPos], 
				vehicle.waterTemp,
				vehicle.prevWaterTemp,
				vehicle.waterTempDisp,
				3, 
				true
			);
		}
		else {
			printFloatNoPoint(
				waterTempPos[xPos], 
				waterTempPos[yPos], 
				vehicle.waterTemp,
				vehicle.prevWaterTemp,
				vehicle.waterTempDisp,
				3, 
				false
			);
		}
		tft.textEnlarge(2);
		tft.textWrite(celcius);
	}
	if (vehicle.prevBrakeTemp != vehicle.brakeTemp) {
		if (vehicle.brakeTemp > 250) {
			printInt(
				brakesTempPos[xPos], 
				brakesTempPos[yPos], 
				vehicle.brakeTemp,
				vehicle.prevBrakeTemp,
				vehicle.brakeTempDisp,
				3, 
				true
			);
		}
		else {
			printInt(
				brakesTempPos[xPos], 
				brakesTempPos[yPos], 
				vehicle.brakeTemp,
				vehicle.prevBrakeTemp,
				vehicle.brakeTempDisp,
				3, 
				false
			);
		}
		tft.textEnlarge(2);
		tft.textWrite(celcius);
	}
	if (vehicle.prevRPM != vehicle.rpm) {
		if (vehicle.rpm > MAX_RPM - 500) {
			printInt(
				rpmPos[xPos], 
				rpmPos[yPos], 
				vehicle.rpm,
				vehicle.prevRPM,
				vehicle.rpmDisp,
				3, 
				false
			);
		}
		else {
			printInt(
				rpmPos[xPos], 
				rpmPos[yPos], 
				vehicle.rpm,
				vehicle.prevRPM,
				vehicle.rpmDisp,
				3, 
				false
			);
		}
	}
	if (vehicle.prevSpeed != vehicle.speed) {
		
		if (vehicle.speed > vehicle.prevSpeed) {
			for (int i = vehicle.prevSpeed; i < vehicle.speed; i++) {
				drawSpeedLine(i, RA8875_RED);
			}
		}
		else {
			for (int i = vehicle.prevSpeed; i >= vehicle.speed; i--) {
				drawSpeedLine(i, RA8875_BLACK);
			}
		}

		printInt(
			speedPos[xPos], 
			speedPos[yPos], 
			vehicle.speed,
			vehicle.prevSpeed,
			vehicle.speedDisp,
			3, 
			false
		);
		vehicle.prevSpeed = vehicle.speed;
	}
	if (vehicle.prevGear != vehicle.gear) {
		if (vehicle.gear == 0) {
			tft.drawChar(gearPos[xPos], gearPos[yPos], 'N', 0xffff, 0x0000, gearSize);
			vehicle.prevGear = vehicle.gear;
		}
		else {
			vehicle.gearDisp = 48 + vehicle.gear;
			vehicle.prevGear = vehicle.gear;
			tft.drawChar(gearPos[xPos], gearPos[yPos], vehicle.gearDisp, 0xffff, 0x0000, gearSize);
		}
	}
}

void drawFanDisabled() {
	tft.drawCircle(238, 23, 20, RA8875_RED);
	tft.drawLine(225, 10, 251, 36, RA8875_RED);
}

// Draws the line in a circular speedometer
void drawSpeedLine(const uint8_t& value, const uint16_t& color) {
	int speedToDeg = 280 - value;
	int u0 = (speedoOffsetRadius * sin(speedToDeg * (PI / 180))) + cX;
	int v0 = (speedoOffsetRadius * cos(speedToDeg * (PI / 180))) + cY;
	int u1 = ((speedoBarRadius + speedoOffsetRadius) * sin(speedToDeg * (PI / 180))) + cX;
	int v1 = ((speedoBarRadius + speedoOffsetRadius) * cos(speedToDeg * (PI / 180))) + cY;
	tft.drawLine(u0, v0, u1, v1, color);
}

void runShiftRegister() {
	// Close the latch to write into register memory
	digitalWrite(SR_LATCH, LOW);
	// Scale RPM to number of LEDs
	ledBarSetBits = vehicle.rpm / RPM_SCALE;
	uint8_t warning = warningSetBits | WARNING_LIGHT2 | WARNING_LIGHT4 | WARNING_LIGHT6 | WARNING_LIGHT8;
	
	// Shift bits to register
	for (int i = 0; i < SR_WARNINGBITS; i++) {
		
		digitalWrite(SR_DATA_OUT, (warning & 0x1));
		digitalWrite(SR_CLOCK_OUT, LOW);
		digitalWrite(SR_CLOCK_OUT, HIGH);
		warning = warning >> 1;
	}

	for (int i = 0; i < SR_LEDBITS; i++) {	
		if (i < ledBarSetBits) {
			digitalWrite(SR_DATA_OUT, HIGH);
		}
		else {
			digitalWrite(SR_DATA_OUT, LOW);
		}
		// Manually set clock transition
		digitalWrite(SR_CLOCK_OUT, LOW);
		digitalWrite(SR_CLOCK_OUT, HIGH);
	}
	// Open latch and enable register outputs
	digitalWrite(SR_LATCH, HIGH);
}

void setup() {
	Serial.begin(9600);
	Serial.println("RA8875 start");

	// Initialize the CAN bus
	Can0.begin(500000);
	Can0.attachObj(&canListener);
	canListener.attachGeneralHandler();

	// Enable shiftregister output
	pinMode(SR_CLOCK_OUT, OUTPUT);
	pinMode(SR_DATA_OUT, OUTPUT);
	pinMode(SR_LATCH, OUTPUT);
	pinMode(SR_OUTPUT_ENABLE, OUTPUT);
	digitalWrite(SR_OUTPUT_ENABLE, HIGH);
	warningSetBits = 0;

	// Initialise the display using 'RA8875_480x272'
	while (!tft.begin(RA8875_800x480)) {
		Serial.println("RA8875 Not Found!");
		delay(1000);
	}
	digitalWrite(RA8875_CS, LOW);

	Serial.println("Found RA8875");

	tft.displayOn(true);
	tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
	tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
	tft.PWM1out(0); // Start faded out
	
	//Draw logo
	tft.graphicsMode();
	tft.fillScreen(RA8875_BLACK);
	tft.drawXBitmap(logoPos[xPos], logoPos[yPos], logo, logoWidth, logoHeight, RA8875_RED);
	
	// Fade in and wait 2s
	for (uint8_t i = 0; i != 255; i += 5) {
		tft.PWM1out(i);
		delay(10);
	}
	delay(2000);

	// Enable touch function
	pinMode(RA8875_INT, INPUT);
	digitalWrite(RA8875_INT, HIGH);
	tft.touchEnable(true);
	
	// Clear sceen
	tft.fillScreen(RA8875_BLACK);
	tft.textMode();
	tft.textColor(RA8875_WHITE, RA8875_BLACK);
	printIcons();
	printLabels();
	printFrames();
	printValues();
}

void loop() {

	printValues();
	runShiftRegister();

	/*
	float xScale = 1024.0F / tft.width();
	float yScale = 1024.0F / tft.height();
	
	// Wait around for touch events
	if (digitalRead(RA8875_INT)) {
		if (tft.touched()) {
			tft.touchRead(&tx, &ty);
			uint16_t touchX = (uint16_t)(tx / xScale);
			uint16_t touchY = (uint16_t)(ty / yScale);

			if (touchX >= 220 && touchX <= 255 && touchY >= 0 && touchY <= 40) {
				if (prevFanOn != fanOn) {
					prevFanOn = fanOn;
					if (fanOn) {
						drawFanDisabled();
						fanOn = false;
					}
					else {
						tft.graphicsMode();
						tft.fillRect(215, 0, 50, 50, 0x0000);
						tft.drawXBitmap(225, 10, fanIcon, fanWidth, fanHeight, 0xffff);
						tft.textMode();
						fanOn = true;
					}
				}
			}
		}
	}
	*/
}