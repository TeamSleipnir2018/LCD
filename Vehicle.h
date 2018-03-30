#pragma once
#include <stdint.h>

class Vehicle {
public:
	// Vehicle values
	uint16_t rpm;
	uint16_t prevRPM;
	char rpmDisp[6];

	float oilTemp;
	float prevOilTemp;
	char oilTempDisp[5];

	float waterTemp;
	float prevWaterTemp;
	char waterTempDisp[5];

	uint16_t brakeTemp;
	uint16_t prevBrakeTemp;
	char brakeTempDisp[5];

	uint8_t gear;
	uint8_t prevGear;
	char gearDisp;

	uint16_t speed;
	uint16_t prevSpeed;
	char speedDisp[4];

	float voltage;
	float prevVoltage;
	char voltageDisp[5];

	bool fanOn;
	bool prevFanOn;

	Vehicle();
	virtual ~Vehicle();
};

