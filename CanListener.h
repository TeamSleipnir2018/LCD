#pragma once
#include <stdint.h>

class CanListener : public CANListener {
public:
	bool frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller); //overrides the parent version so we can actually do something
};