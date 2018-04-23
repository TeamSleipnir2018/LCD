#pragma once
#include <stdint.h>

class Vehicle {
public:
	// Vehicle values
	uint16_t rpm;
	uint16_t prevRPM;

	float oilTemp;
	float prevOilTemp;

	float waterTemp;
	float prevWaterTemp;

	uint16_t brakeTemp;
	uint16_t prevBrakeTemp;

	uint8_t gear;
	uint8_t prevGear;

	uint16_t speed;
	uint16_t prevSpeed;

	float voltage;
	float prevVoltage;

	bool fanOn;
	bool prevFanOn;

	Vehicle();
	virtual ~Vehicle();
};

