#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define WOOTINGRGBSDK_EXPORTS
#include <wooting-rgb-sdk.h>

#define null nullptr
#define UNUSED(var) { (var) = (var); }

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} WootingColor;

typedef struct {
	HANDLE pipeHandle;
	size_t disconnectCallbackCount;
	void_cb* disconnectCallbacks;
	bool autoUpdate;
	WootingColor keyboardColors[WOOTING_RGB_ROWS * WOOTING_RGB_COLS];
} SimulatedState;

typedef struct {
	uint8_t row;
	uint8_t column;
} WootingPosition;

typedef enum {
	SimulatedCommand_ack = 0,
	SimulatedCommand_nack,
	SimulatedCommand_reset,
	SimulatedCommand_setKey,
	SimulatedCommand_resetKey,
	SimulatedCommand_update
} SimulatedCommand;

typedef struct {
	SimulatedCommand command;
	uint32_t size;
} SimulatedCommandPacketMeta;

typedef struct {
	SimulatedCommand command;
} SimulatedCommandAck;
typedef SimulatedCommandAck SimulatedCommandNack;

typedef struct {
} SimulatedCommandReset;

typedef struct {
	WootingColor color;
	WootingPosition positon;
} SimulatedCommandSetKey;

typedef struct {
	WootingPosition positon;
} SimulatedCommandResetKey;

typedef struct {
	WootingColor colors[WOOTING_RGB_ROWS * WOOTING_RGB_COLS];
} SimulatedCommandUpdate;

typedef struct {
	SimulatedCommandPacketMeta meta;
	union commandData {
		SimulatedCommandAck ack;
		SimulatedCommandNack nack;
		SimulatedCommandReset reset;
		SimulatedCommandSetKey setKey;
		SimulatedCommandResetKey resetKey;
		SimulatedCommandUpdate update;
	} data;
} SimulatedCommandPacket;
