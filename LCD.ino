/******************************************************************
Team Sleipnir steering wheel LCD

Hardware:
	- Teensy 3.2 (Previously Arduino Pro Mini)
	- Adafruit RA8875 touch LCD controller
	- 480 x 272 touch LCD
	- NPIC6C4894 shift registers

Written by Einar Arnason
******************************************************************/

#include <kinetis_flexcan.h>
#include <FlexCAN.h>
#include <Metro.h>
#include <SPI.h>
#include <stdint.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include "logo.h"
#include "fanIcon.h"

// Pin assignment
// LCD
const uint8_t RA8875_INT = 2;
const uint8_t RA8875_CS = 10;
const uint8_t RA8875_RESET = 9;
// Shift register
const uint8_t SR_CLOCK_OUT = 16;
const uint8_t SR_DATA_OUT = 15;
const uint8_t SR_LATCH = 14;

// LCD driver
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

// CAN BUS driver
FlexCAN CANbus(3);
Metro sysTimer = Metro(1);// milliseconds
static CAN_message_t msg, rxmsg;
//static uint8_t hex[17] = "0123456789abcdef";
int txCount, rxCount;
unsigned int txTimer, rxTimer;
// Translate float values from CAN BUS
inline float CANIntToFloat(uint16_t floatValue) {
	return floatValue / 1000;
}

// Vehicle values
const uint16_t MAX_RPM = 14000;

uint16_t rpm;
uint16_t prevRPM;
char rpmDisp[6];

uint16_t oilTemp;
uint16_t prevOilTemp;
char oilTempDisp[5];

uint16_t waterTemp;
uint16_t prevWaterTemp;
char waterTempDisp[5];

uint16_t brakeTemp;
uint16_t prevBrakeTemp;
char brakeTempDisp[5];

uint8_t gear;
uint8_t prevGear;
char gearDisp;

uint16_t speed;
uint16_t prevSpeed;
char speedDisp[3];
uint16_t speedCount;

bool fanOn;
bool prevFanOn;

// LCD positioning
const bool xPos = 0;
const bool yPos = 1;
const uint16_t gearPos[] = { 360, 200 };
const uint16_t oilLabelPos[] = {10, 10};
//const uint16_t oilTempPos[];
const uint16_t waterLabelPos[] = { 10, 160 };
//const uint16_t waterTempPos[];
const uint16_t brakesLabelPos[] = { 10, 300 };
//const uint16_t brakesTempPos[];
const uint16_t speedLabelPos[] = { 620, 240 };
//const uint16_t speedPos[];
const uint16_t rpmLabelPos[] = { 500, 400 };
//const uint16_t rpmPos[];

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
uint8_t warningSetBits;
uint8_t ledBarSetBits;
uint8_t srWarningCounter;
uint8_t srLedCounter;

// Timers
uint8_t tempTimer;

// Speedometer vector
int cX;
int cY;
uint16_t speedoRadius;

// Celcius symbol for arduino
const static char celcius[3] = { 0xb0, 0x43 };

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
		if (speedCount == 3) {
			speed++;
			speedCount = 0;
		}
		else {
			speedCount++;
		}
	}
}

void printLabels() {
	tft.textSetCursor(oilLabelPos[xPos], oilLabelPos[yPos]);
	tft.textEnlarge(3);
	tft.textColor(RA8875_WHITE, RA8875_BLACK);
	tft.textWrite("OIL: ");
	tft.textSetCursor(waterLabelPos[xPos], waterLabelPos[yPos]);
	tft.textWrite("WATER: ");
	tft.textSetCursor(brakesLabelPos[xPos], brakesLabelPos[yPos]);
	tft.textWrite("BRAKES: ");
	//tft.drawCircle(370, 110, 80, 0xffff);
	tft.textSetCursor(speedLabelPos[xPos], speedLabelPos[yPos]);
	tft.textWrite("km/h");
	tft.textSetCursor(rpmLabelPos[xPos], rpmLabelPos[yPos]);
	tft.textEnlarge(3);
	tft.textColor(RA8875_WHITE, RA8875_BLACK);
	tft.textWrite(" RPM");
}

void printValue(const uint16_t& x,
				const uint16_t& y,
				const uint16_t& value,
				uint16_t& prevValue,
				char* charValue,
				const uint8_t& fontSize,
				bool warning) {
	sprintf(charValue, "%d", value);
	prevValue = value;
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
	if (tempTimer == 255) {
		if (prevOilTemp != oilTemp) {
			if (oilTemp > 250) {
				printValue(10, 80, oilTemp, prevOilTemp, oilTempDisp, 3, true);
			}
			else {
				printValue(10, 80, oilTemp, prevOilTemp, oilTempDisp, 3, false);
			}
			tft.textWrite(celcius);
		}
		if (prevWaterTemp != waterTemp) {
			if (waterTemp > 250) {
				printValue(10, 230, waterTemp, prevWaterTemp, waterTempDisp, 3, true);
			}
			else {
				printValue(10, 230, waterTemp, prevWaterTemp, waterTempDisp, 3, false);
			}
			tft.textWrite(celcius);
		}
		if (prevBrakeTemp != brakeTemp) {
			if (brakeTemp > 250) {
				printValue(10, 380, brakeTemp, prevBrakeTemp, brakeTempDisp, 3, true);
			}
			else {
				printValue(10, 380, brakeTemp, prevBrakeTemp, brakeTempDisp, 3, false);
			}
			tft.textWrite(celcius);
		}
		tempTimer = 0;
	}
	else {
		tempTimer++;
	}
	if (prevRPM != rpm) {
		if (rpm > MAX_RPM - 500) {
			printValue(340, 400, rpm, prevRPM, rpmDisp, 3, true);
		}
		else {
			printValue(340, 400, rpm, prevRPM, rpmDisp, 3, false);
		}
	}
	if (prevSpeed != speed) {
		
		//drawSpeedLine(prevSpeed, 0x0000);
		//drawSpeedLine(speed, RA8875_RED);
		
		printValue(520, 230, speed, prevSpeed, speedDisp, 3, false);
		switch (speed) {
		case 28 :
			tft.fillRect(500, 210, 18, 30, 0xffff);
			break;
		case 56 :
			tft.fillRect(520, 200, 18, 35, 0xf877);
			break;
		case 84 :
			tft.fillRect(540, 190, 18, 40, 0xf866);
			break;
		case 112 :
			tft.fillRect(560, 180, 18, 45, 0xf855);
			break;
		case 140 :
			tft.fillRect(580, 170, 18, 50, 0xf844);
			break;
		case 168 :
			tft.fillRect(600, 165, 18, 55, 0xf833);
			break;
		case 196 :
			tft.fillRect(620, 160, 18, 60, 0xf822);
			break;
		case 224 :
			tft.fillRect(640, 155, 18, 65, 0xf811);
			break;
		case 252 :
			tft.fillRect(660, 150, 18, 70, 0xf800);
			break;
		}
		//prevSpeed = speed;
	}
	if (prevGear != gear) {
		if (gear == 0) {
			tft.drawChar(gearPos[xPos], gearPos[yPos], 'N', 0xffff, 0x0000, 20);
			prevGear = gear;
		}
		else {
			gearDisp = 48 + gear;
			prevGear = gear;
			tft.drawChar(gearPos[xPos], gearPos[yPos], gearDisp, 0xffff, 0x0000, 20);
		}
	}
}

void drawFanDisabled() {
	tft.drawCircle(238, 23, 20, RA8875_RED);
	tft.drawLine(225, 10, 251, 36, RA8875_RED);
}

void drawSpeedLine(const uint8_t& value, const uint16_t& color) {
	int speedToDeg = 315 - value;
	int u = (speedoRadius * sin(speedToDeg * (PI / 180))) + cX;
	int v = (speedoRadius * cos(speedToDeg * (PI / 180))) + cY;
	tft.drawLine(cX, cY, u, v, color);
}

void runShiftRegister() {
	// Close the latch to write into register memory
	digitalWrite(SR_LATCH, LOW);
	// Scale RPM to number of LEDs
	ledBarSetBits = rpm / RPM_SCALE;
	// Shift bits to register
	for (int i = 0; i < 48; i++) {	
		if (i < ledBarSetBits + 8) {
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

const CAN_filter_t canFilter = { 0, 0, 0 };

void setup() {
	Serial.begin(9600);
	Serial.println("RA8875 start");

	// Initialize the CAN bus
	CANbus.begin();

	// Enable shiftregister output
	pinMode(SR_CLOCK_OUT, OUTPUT);
	pinMode(SR_DATA_OUT, OUTPUT);
	pinMode(SR_LATCH, OUTPUT);

	// Initialise the display using 'RA8875_480x272'
	if (!tft.begin(RA8875_800x480)) {
		Serial.println("RA8875 Not Found!");
		while (1);
	}
	digitalWrite(RA8875_CS, LOW);

	Serial.println("Found RA8875");

	tft.displayOn(true);
	tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
	tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
	tft.PWM1out(0); // Start faded out
	
	//Draw logo
	tft.graphicsMode();
	tft.drawXBitmap(8, 104, logo, 464, 64, RA8875_RED);
	
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
	oilTemp = 0;
	prevOilTemp = 255;
	waterTemp = 0;
	prevWaterTemp = 255;
	brakeTemp = 0;
	prevBrakeTemp = 255;
	gear = 0;
	prevGear = 255;
	speed = 0;
	prevSpeed = 255;
	fanOn = false;
	prevFanOn = true;

	// Initialize speedometer values
	cX = 370;
	cY = 110;
	speedoRadius = 70;
	speedCount = 0;

	// Initialize timers
	tempTimer = 0;

	// Clear sceen
	tft.fillScreen(RA8875_BLACK);
	// Draw the icon for cooling fan
	tft.drawXBitmap(225, 10, fanIcon, 25, 25, 0xffff);
	drawFanDisabled();
	tft.textMode();
	tft.textColor(RA8875_WHITE, RA8875_BLACK);
	printLabels();
	printValues();

	sysTimer.reset();
}

void loop() {
	
	float xScale = 1024.0F / tft.width();
	float yScale = 1024.0F / tft.height();

	demo();
	printValues();
	runShiftRegister();
	//listenOnCAN();

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
						tft.drawXBitmap(225, 10, fanIcon, 25, 25, 0xffff);
						tft.textMode();
						fanOn = true;
					}
				}
			}
		}
	}

	if (gear == 6 && rpm == 14000 && tempTimer == 255) {
		setup();
	}
}