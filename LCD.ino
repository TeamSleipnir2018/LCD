/******************************************************************
Team Sleipnir steering wheel LCD

Hardware:
	- Teensy 3.2
	- Adafruit RA8875 touch LCD controller
	- 800 x 480 LCD
	- NPIC6C4894 shift registers

Written by Einar Arnason
******************************************************************/

#include <FlexCAN.h>
#include <Metro.h>
#include <SPI.h>
#include <stdint.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
#include "./images/logo.h"
#include "./images/fanIcon.h"
#include "./images/brakeTempIcon.h"
#include "./images/oilTempIcon.h"
#include "./images/waterTempIcon.h"
#include "./images/batteryIcon.h"
#include "constants.h"
<<<<<<< HEAD
=======
#include "logo.h"
#include "fanIcon.h"
#include "CanListener.h"

// Pin assignment
// LCD
const uint8_t RA8875_INT = 2;
const uint8_t RA8875_CS = 10;
const uint8_t RA8875_RESET = 9;
// Shift register
const uint8_t SR_CLOCK_OUT = 16;
const uint8_t SR_DATA_OUT = 15;
const uint8_t SR_LATCH = 14;
>>>>>>> Fixing some conflicts
=======
>>>>>>> Added icons, calibrated ECU values, moved constants to another file

// LCD driver
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

// CAN BUS driver
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
class CanListener : public CANListener {
public:
	//overrides the parent version
	bool frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller);
};
<<<<<<< HEAD
=======

>>>>>>> Fixing some conflicts

CanListener canListener;
unsigned int txTimer, rxTimer;
<<<<<<< HEAD
=======
// Translate float values from CAN BUS
inline float CANIntToFloat(uint16_t floatValue) {
	return floatValue / 1000.0;
}
>>>>>>> Merging
=======

CanListener canListener;
unsigned int txTimer, rxTimer;
>>>>>>> Added icons, calibrated ECU values, moved constants to another file

// Vehicle values
uint16_t rpm;
uint16_t prevRPM;
char rpmDisp[6];

float oilTemp;
float prevOilTemp;
char oilTempDisp[8];

float waterTemp;
float prevWaterTemp;
char waterTempDisp[8];

uint16_t brakeTemp;
uint16_t prevBrakeTemp;
char brakeTempDisp[8];

uint8_t gear;
uint8_t prevGear;
char gearDisp;

<<<<<<< HEAD
<<<<<<< HEAD
uint16_t speed;
uint16_t prevSpeed;
char speedDisp[3];

float voltage;
float prevVoltage;
char voltageDisp[8];
=======
<<<<<<< HEAD
uint8_t speed;
uint8_t prevSpeed;
char speedDisp[2];
uint8_t speedCount;
=======
=======
>>>>>>> Merging
uint16_t speed;
uint16_t prevSpeed;
char speedDisp[3];
uint16_t speedCount;
<<<<<<< HEAD
>>>>>>> 0d966d39b1d0f694c7cdf1ac084a26e00ea884fc
>>>>>>> Cleanup
=======
>>>>>>> Merging

float voltage;
float prevVoltage;
char voltageDisp[8];

bool fanOn;
bool prevFanOn;

<<<<<<< HEAD
// Shift register variables
=======
bool CanListener::frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller) {

	switch (frame.id) {
	case 1:
		rpm = frame.buf[0] | (frame.buf[1] << 8);
		voltage = CANIntToFloat(frame.buf[2] | (frame.buf[3] << 8));
		waterTemp = CANKelvinToFloat(frame.buf[4] | (frame.buf[5] << 8));
		speed = frame.buf[6] | (frame.buf[7] << 8);
		break;
	case 2:
		oilTemp = CANKelvinToFloat(frame.buf[0] | (frame.buf[1] << 8));
		gear = frame.buf[2] | (frame.buf[3] << 8);
		break;
	}

	return true;
}

<<<<<<< HEAD
// LCD positioning
const uint16_t lcdWidth = 800;
const uint16_t lcdHeight = 480;
const uint16_t logoPos[] = { ((lcdWidth - logoWidth) / 2), ((lcdHeight - logoHeight) / 2) };
const bool xPos = 0;
const bool yPos = 1;
const uint16_t gearSize = 20;
const uint16_t gearPos[] = { ((lcdWidth - (5 * gearSize)) / 2), ((lcdHeight - (8 * gearSize)) / 2) };
const uint16_t oilLabelPos[] = { 10, 10 };
const uint16_t oilTempPos[] = { 10, 80};
const uint16_t waterLabelPos[] = { 10, 160 };
const uint16_t waterTempPos[] = { 10, 230 };
const uint16_t brakesLabelPos[] = { 10, 300 };
const uint16_t brakesTempPos[] = { 10, 380 };
const uint16_t speedLabelPos[] = { 620, 240 };
const uint16_t speedPos[] = { 520, 240 };
const uint16_t rpmLabelPos[] = { 480, 380 };
const uint16_t rpmPos[] = { 320, 380 };

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
>>>>>>> Fixing some conflicts
uint8_t warningSetBits;
uint8_t ledBarSetBits;
uint8_t srWarningCounter;
uint8_t srLedCounter;

bool CanListener::frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller) {
	Serial.println(frame.id);
	switch (frame.id) {
	case 1:
		rpm = frame.buf[0] | (frame.buf[1] << 8);
		voltage = CANIntToFloat(frame.buf[2] | (frame.buf[3] << 8));
		waterTemp = CANKelvinToFloat(frame.buf[4] | (frame.buf[5] << 8));
		speed = frame.buf[6] | (frame.buf[7] << 8);
		break;
	case 2:
		oilTemp = CANKelvinToFloat(frame.buf[0] | (frame.buf[1] << 8));
		gear = frame.buf[2] | (frame.buf[3] << 8);
		break;
	}

	return true;
}

=======
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
// Timers
uint8_t tempTimer;

/*
// Circular speedometer vector
int cX;
int cY;
uint16_t speedoRadius;
*/

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
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
	tft.drawXBitmap(
		batteryIconPos[xPos], 
		batteryIconPos[yPos], 
		batteryIcon, 
		batteryWidth, 
		batteryHeight, 
		RA8875_WHITE
	);
	//drawFanDisabled();
	tft.drawXBitmap(
		oilLabelPos[xPos], 
		oilLabelPos[yPos], 
		oilTempIcon, 
		oilTempWidth, 
		oilTempHeight, 
		RA8875_WHITE
	);
	tft.drawXBitmap(
		waterLabelPos[xPos],
		waterLabelPos[yPos],
		waterTempIcon,
		waterTempWidth,
		waterTempHeight,
		RA8875_WHITE
	);
	tft.drawXBitmap(
		brakesLabelPos[xPos],
		brakesLabelPos[yPos],
		brakeTempIcon,
		brakeTempWidth,
		brakeTempHeight,
		RA8875_WHITE
	);
	tft.textMode();
=======
// Celcius symbol, b0 is a RA8875 specific value for ° symbol
=======
// Celcius symbol, b0 is a RA8875 specific value for ï¿½ symbol
>>>>>>> Fixing some conflicts
const static char celcius[3] = { 0xb0, 0x43 };

=======
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
/*
void demo() {
	if (rpm != MAX_RPM) {
		rpm += 25;
	}
	else {
		oilTemp = random(110, 150);
		waterTemp = random(110, 150);
		brakeTemp = random(110, 150);
		
		if (gear < 6) {
			gear++;
			rpm = 2000;
		}
		else {
			rpm = 13500;
		}
	}

	if (speed < 255 && gear != 0) {
		if (speedCount == 5) {
			speed++;
			speedCount = 0;
		}
		else {
			speedCount++;
		}
	}
>>>>>>> Merging
}
*/

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
	tft.drawXBitmap(
		batteryIconPos[xPos], 
		batteryIconPos[yPos], 
		batteryIcon, 
		batteryWidth, 
		batteryHeight, 
		RA8875_WHITE
	);
	//drawFanDisabled();
	tft.drawXBitmap(
		oilLabelPos[xPos], 
		oilLabelPos[yPos], 
		oilTempIcon, 
		oilTempWidth, 
		oilTempHeight, 
		RA8875_WHITE
	);
	tft.drawXBitmap(
		waterLabelPos[xPos],
		waterLabelPos[yPos],
		waterTempIcon,
		waterTempWidth,
		waterTempHeight,
		RA8875_WHITE
	);
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

<<<<<<< HEAD
void printFrames() {
	tft.drawFastVLine(280, 0, 480, RA8875_WHITE);
	tft.drawFastVLine(500, 0, 480 - (480 - 350), RA8875_WHITE);
	tft.drawFastHLine(280, 350, (800 - 280), RA8875_WHITE);
	tft.drawFastHLine(0, oilTempPos[yPos] + oilTempHeight + 30, 280, RA8875_WHITE);
	tft.drawFastHLine(0, waterTempPos[yPos] + waterTempHeight + 30, 280, RA8875_WHITE);
	tft.drawFastHLine(0, brakesTempPos[yPos] + brakeTempHeight + 30, 280, RA8875_WHITE);
	tft.drawFastHLine(0, voltagePos[yPos] + batteryHeight + 30, 280, RA8875_WHITE);
}

=======
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
void printInt(const uint16_t& x,
	const uint16_t& y,
	const uint16_t& value,
	uint16_t& prevValue,
	char* charValue,
	const uint8_t& fontSize,
	bool warning) {

<<<<<<< HEAD
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

void printFloatNoPoint(
	const uint16_t& x,
	const uint16_t& y,
	const float& value,
	float& prevValue,
	char* charValue,
	const uint8_t& fontSize,
	bool warning) {

	sprintf(charValue, "%d", (int)value);
=======
	sprintf(charValue, "%d", value);
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
	prevValue = value;
	printValue(x, y, charValue, fontSize, warning);
}

<<<<<<< HEAD
=======
void printFloat(
	const uint16_t& x,
	const uint16_t& y,
	const float& value,
	float& prevValue,
	char* charValue,
	const uint8_t& fontSize,
	bool warning) {

	sprintf(charValue, "%.02f", value);
	prevValue = value;
	printValue(x, y, charValue, fontSize, warning);
}

>>>>>>> Added icons, calibrated ECU values, moved constants to another file
void printValue(
	const uint16_t& x,
	const uint16_t& y,
	char* charValue, 
	const uint8_t& fontSize,
	bool warning
	) {
<<<<<<< HEAD
	tft.textMode();
=======
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
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
	//if (tempTimer == 0) {
		if (voltage != prevVoltage) {
			printFloat(
				voltagePos[xPos],
				voltagePos[yPos],
				voltage,
				prevVoltage,
				voltageDisp,
				3,
				false
			);
			tft.textWrite("v");
		}
		if (prevOilTemp != oilTemp) {
			if (oilTemp > 250) {
<<<<<<< HEAD
				printFloatNoPoint(
=======
				printFloat(
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
					oilTempPos[xPos], 
					oilTempPos[yPos], 
					oilTemp, 
					prevOilTemp, 
					oilTempDisp, 
					3, 
					true
				);
			}
			else {
<<<<<<< HEAD
				printFloatNoPoint(
=======
				printFloat(
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
					oilTempPos[xPos], 
					oilTempPos[yPos], 
					oilTemp, 
					prevOilTemp, 
					oilTempDisp, 
					3, 
					false
				);
			}
			tft.textEnlarge(2);
			tft.textWrite(celcius);
		}
		if (prevWaterTemp != waterTemp) {
			if (waterTemp > 250) {
<<<<<<< HEAD
				printFloatNoPoint(
=======
				printFloat(
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
					waterTempPos[xPos], 
					waterTempPos[yPos], 
					waterTemp, 
					prevWaterTemp, 
					waterTempDisp, 
					3, 
					true
				);
			}
			else {
<<<<<<< HEAD
				printFloatNoPoint(
=======
				printFloat(
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
					waterTempPos[xPos], 
					waterTempPos[yPos], 
					waterTemp, 
					prevWaterTemp, 
					waterTempDisp, 
					3, 
					false
				);
			}
			tft.textEnlarge(2);
			tft.textWrite(celcius);
		}
		if (prevBrakeTemp != brakeTemp) {
			if (brakeTemp > 250) {
				printInt(
					brakesTempPos[xPos], 
					brakesTempPos[yPos], 
					brakeTemp, 
					prevBrakeTemp, 
					brakeTempDisp, 
					3, 
					true
				);
			}
			else {
				printInt(
					brakesTempPos[xPos], 
					brakesTempPos[yPos], 
					brakeTemp, 
					prevBrakeTemp, 
					brakeTempDisp, 
					3, 
					false
				);
			}
			tft.textEnlarge(2);
			tft.textWrite(celcius);
		}
		tempTimer = 0;
	/*}
	else {
		tempTimer++;
	}*/
	if (prevRPM != rpm) {
		if (rpm > MAX_RPM - 500) {
			printInt(
				rpmPos[xPos], 
				rpmPos[yPos], 
				rpm, 
				prevRPM, 
				rpmDisp, 
				3, 
				true
			);
		}
		else {
			printInt(
				rpmPos[xPos], 
				rpmPos[yPos], 
				rpm, 
				prevRPM, 
				rpmDisp, 
				3, 
				false
			);
		}
	}
	if (prevSpeed != speed) {
		
		//drawSpeedLine(prevSpeed, 0x0000);
		//drawSpeedLine(speed, RA8875_RED);
		
		printInt(
			speedPos[xPos], 
			speedPos[yPos], 
			speed, 
			prevSpeed, 
			speedDisp, 
			3, 
			false
		);
		switch (speed) {
		case 18 :
			tft.fillRect(500, 210, 18, 30, 0xffff);
			break;
		case 36 :
			tft.fillRect(520, 200, 18, 35, 0xf877);
			break;
		case 54 :
			tft.fillRect(540, 190, 18, 40, 0xf866);
			break;
		case 72 :
			tft.fillRect(560, 180, 18, 45, 0xf855);
			break;
		case 90 :
			tft.fillRect(580, 170, 18, 50, 0xf844);
			break;
		case 108 :
			tft.fillRect(600, 165, 18, 55, 0xf833);
			break;
		case 126 :
			tft.fillRect(620, 160, 18, 60, 0xf822);
			break;
		case 144 :
			tft.fillRect(640, 155, 18, 65, 0xf811);
			break;
		case 162 :
			tft.fillRect(660, 150, 18, 75, 0xf800);
			break;
		case 180:
			tft.fillRect(680, 145, 18, 80, 0xf800);
			break;
		case 198:
			tft.fillRect(700, 140, 18, 85, 0xf800);
			break;
		case 216:
			tft.fillRect(720, 135, 18, 90, 0xf800);
			break;
		case 234:
			tft.fillRect(740, 130, 18, 95, 0xf800);
			break;
		case 252:
			tft.fillRect(760, 125, 18, 100, 0xf800);
			break;
		}
		prevSpeed = speed;
	}
	if (prevGear != gear) {
		if (gear == 0) {
			tft.drawChar(gearPos[xPos], gearPos[yPos], 'N', 0xffff, 0x0000, gearSize);
			prevGear = gear;
		}
		else {
			gearDisp = 48 + gear;
			prevGear = gear;
			tft.drawChar(gearPos[xPos], gearPos[yPos], gearDisp, 0xffff, 0x0000, gearSize);
		}
	}
}

void drawFanDisabled() {
	tft.drawCircle(238, 23, 20, RA8875_RED);
	tft.drawLine(225, 10, 251, 36, RA8875_RED);
}

/*
// Draws the line in a circular speedometer
void drawSpeedLine(const uint8_t& value, const uint16_t& color) {
	int speedToDeg = 315 - value;
	int u = (speedoRadius * sin(speedToDeg * (PI / 180))) + cX;
	int v = (speedoRadius * cos(speedToDeg * (PI / 180))) + cY;
	tft.drawLine(cX, cY, u, v, color);
}
*/

void runShiftRegister() {
	// Close the latch to write into register memory
	digitalWrite(SR_LATCH, LOW);
	// Scale RPM to number of LEDs
	ledBarSetBits = rpm / RPM_SCALE;
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

<<<<<<< HEAD
<<<<<<< HEAD
=======
void listenOnCAN() {

	/*
	Todo: 
	CAN BUS sends 4 uint16_t variables per block
	variables are little-endian
	float values need to by divided with 1000
	use function CANIntToFloat()
	*/


	
	/*
	//example code
	// service software timers based on Metro tick
	if (sysTimer.check()) {
		if (txTimer) {
			--txTimer;
		}
		if (rxTimer) {
			--rxTimer;
		}
	}

	// if not time-delayed, read CAN messages and print 1st byte
	if (!rxTimer) {
		while (CANbus.read(rxmsg)) {
			Serial.print(rxmsg.id);
			//hexDump( sizeof(rxmsg), (uint8_t *)&rxmsg );
			Serial.write(rxmsg.buf[0]);
			rxCount++;
		}
	}

	// insert a time delay between transmissions
	if (!txTimer) {
		// if frames were received, print the count
		if (rxCount) {
			Serial.write('=');
			Serial.print(rxCount);
			rxCount = 0;
		}
		txTimer = 100;//milliseconds
		msg.len = 8;
		msg.id = 0x222;
		for (int idx = 0; idx<8; ++idx) {
			msg.buf[idx] = '0' + idx;
		}
		// send 6 at a time to force tx buffering
		txCount = 6;
		Serial.println(".");
		while (txCount--) {
			CANbus.write(msg);
			msg.buf[0]++;
		}
		// time delay to force some rx data queue use
		rxTimer = 3;//milliseconds
	}
	*/
}

>>>>>>> Merging
=======
>>>>>>> Fixing some conflicts
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

	// Initialize car values
	rpm = 0;
	prevRPM = 1;
	oilTemp = 0.0;
	prevOilTemp = 1.0;
	waterTemp = 0.0;
	prevWaterTemp = 1.0;
	brakeTemp = 0;
	prevBrakeTemp = 1;
	gear = 0;
	prevGear = 1;
	speed = 0;
	prevSpeed = 1;
	fanOn = false;
	prevFanOn = true;
	voltage = 0.0;
<<<<<<< HEAD
<<<<<<< HEAD
	prevVoltage = 1.0;
=======
	prevVoltage = 0.0;
>>>>>>> Fixing some conflicts
=======
	prevVoltage = 1.0;
>>>>>>> Added icons, calibrated ECU values, moved constants to another file

	/*
	// Initialize circular speedometer values
	cX = 370;
	cY = 110;
	speedoRadius = 70;
	speedCount = 0;
	*/

	// Initialize timers
	tempTimer = 0;

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

<<<<<<< HEAD
	demo();
=======
	//demo();

	printValues();
	runShiftRegister();
>>>>>>> Fixing some conflicts

	
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
<<<<<<< HEAD
	
=======
	/*
>>>>>>> Added icons, calibrated ECU values, moved constants to another file
	if (gear == 6 && rpm == 14000 && tempTimer == 255) {
		setup();
	}
	*/
}