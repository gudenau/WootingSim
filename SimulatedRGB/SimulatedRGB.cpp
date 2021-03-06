#include "common.h"

void disconnectFromSimulator(void);
extern SimulatedState *simulatedState;

static bool sendCommand(SimulatedCommandPacket commandPacket) {
	SimulatedCommandPacket responcePacket;

	DWORD bytesRead;
	if (!TransactNamedPipe(
		simulatedState->pipeHandle,
		&commandPacket,
		commandPacket.meta.size,
		&responcePacket,
		sizeof(SimulatedCommandPacket),
		&bytesRead,
		null
	) || bytesRead != responcePacket.meta.size) {
		disconnectFromSimulator();
		return false;
	}

	if (bytesRead != sizeof(SimulatedCommandPacketMeta) + sizeof(SimulatedCommandAck)) {
		disconnectFromSimulator();
		return false;
	}

	if (responcePacket.meta.command != SimulatedCommand_ack || responcePacket.data.ack.command != commandPacket.meta.command) {
		disconnectFromSimulator();
		return false;
	}

	return true;
}

/** @brief Check if keyboard connected.

This function offers a check if the keyboard is connected.
The read functions will automatically call this function on first use.

@ingroup API

@returns
This function returns true (1) if keyboard is found.
*/
WOOTINGRGBSDK_API bool wooting_rgb_kbd_connected(void) {
	return simulatedState != null;
}

/** @brief Set callback for when a keyboard disconnects.

The callback will be called when a Wooting keyboard disconnects. The callback will only trigger after a failed color change.

@ingroup API
@param cb The function pointer of the callback

@returns
None.
*/
WOOTINGRGBSDK_API void wooting_rgb_set_disconnected_cb(void_cb cb) {
	if (simulatedState && cb) {
		size_t oldSize = simulatedState->disconnectCallbackCount;
		void_cb* oldCallbacks = simulatedState->disconnectCallbacks;

		simulatedState->disconnectCallbackCount++;
		if (!simulatedState->disconnectCallbacks) {
			simulatedState->disconnectCallbacks = (void_cb*)HeapAlloc(
				GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				sizeof(void_cb)
			);
		}
		else {
			simulatedState->disconnectCallbacks = (void_cb*)HeapReAlloc(
				GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				simulatedState->disconnectCallbacks,
				sizeof(void_cb) * (simulatedState->disconnectCallbackCount)
			);
		}

		if (simulatedState->disconnectCallbacks) {
			simulatedState->disconnectCallbacks[oldSize] = cb;
		}
		else {
			cb();
			simulatedState->disconnectCallbackCount = oldSize;
			simulatedState->disconnectCallbacks = oldCallbacks;
			disconnectFromSimulator();
		}
	}
}

/** @brief Reset all colors on keyboard to the original colors.

This function will restore all the colours to the colours that were originally on the keyboard. This function
should always be called when you close the application.

@ingroup API

@returns
Return true (1) if the keyboard is reset
*/
WOOTINGRGBSDK_API bool wooting_rgb_reset(void) {
	if (simulatedState) {
		SimulatedCommandPacket commandPacket;
		memset(&commandPacket, 0, sizeof(SimulatedCommandPacket));
		commandPacket.meta.command = SimulatedCommand_reset;
		commandPacket.meta.size = sizeof(SimulatedCommandReset) + sizeof(SimulatedCommandPacketMeta);
		return sendCommand(commandPacket);
	}
	else {
		return false;
	}
}

/** @brief Directly set and update 1 key on the keyboard.

This function will directly change the color of 1 key on the keyboard. This will not influence the keyboard color array.

Use this function for simple applifications, like a notification. Use the array functions if you want to change the entire keyboard.

@ingroup API
@param row The horizontal index of the key
@param coloum The vertical index of the key
@param red 0-255 value of the red color
@param green 0-255 value of the green color
@param blue 0-255 value of the blue color

@returns
This functions return true (1) if the colour is set.
*/
WOOTINGRGBSDK_API bool wooting_rgb_direct_set_key(uint8_t row, uint8_t column, uint8_t red, uint8_t green, uint8_t blue) {
	if (simulatedState) {
		SimulatedCommandPacket commandPacket;
		memset(&commandPacket, 0, sizeof(SimulatedCommandPacket));
		commandPacket.meta.command = SimulatedCommand_setKey;
		commandPacket.meta.size = sizeof(SimulatedCommandSetKey) + sizeof(SimulatedCommandPacketMeta);
		commandPacket.data.setKey.positon.row = row;
		commandPacket.data.setKey.positon.column = column;
		commandPacket.data.setKey.color.red = red;
		commandPacket.data.setKey.color.green = green;
		commandPacket.data.setKey.color.blue = blue;
		return sendCommand(commandPacket);
	}
	else {
		return false;
	}
}

/** @brief Directly reset 1 key on the keyboard to the original color.

This function will directly reset the color of 1 key on the keyboard. This will not influence the keyboard color array.

Use this function for simple applifications, like a notification. Use the array functions if you want to change the entire keyboard.

@ingroup API
@param row The horizontal index of the key
@param coloum The vertical index of the key

@returns
This functions return true (1) if the colour is reset.
*/
WOOTINGRGBSDK_API bool wooting_rgb_direct_reset_key(uint8_t row, uint8_t column) {
	if (simulatedState) {
		SimulatedCommandPacket commandPacket;
		memset(&commandPacket, 0, sizeof(SimulatedCommandPacket));
		commandPacket.meta.command = SimulatedCommand_resetKey;
		commandPacket.meta.size = sizeof(SimulatedCommandResetKey) + sizeof(SimulatedCommandPacketMeta);
		commandPacket.data.setKey.positon.row = row;
		commandPacket.data.setKey.positon.column = column;
		return sendCommand(commandPacket);
	}
	else {
		return false;
	}
}

/** @brief Send the colors from the color array to the keyboard.

This function will send the changes made with the wooting_rgb_array single and full functions to the keyboard.

@ingroup API

@returns
This functions return true (1) if the colours are updated.
*/
WOOTINGRGBSDK_API bool wooting_rgb_array_update_keyboard(void) {
	if (simulatedState) {
		SimulatedCommandPacket commandPacket;
		memset(&commandPacket, 0, sizeof(SimulatedCommandPacket));
		commandPacket.meta.command = SimulatedCommand_update;
		commandPacket.meta.size = sizeof(SimulatedCommandUpdate) + sizeof(SimulatedCommandPacketMeta);
		memcpy(commandPacket.data.update.colors, simulatedState->keyboardColors, WOOTING_RGB_ROWS * WOOTING_RGB_COLS * 3);
		return sendCommand(commandPacket);
	}
	else {
		return false;
	}
}

/** @brief Change the auto update flag for the wooting_rgb_array single and full functions functions.

This function can be used to set a auto update trigger after every change with the wooting_rgb_array single and full functions function.

Standard is set to false.

@ingroup API
@param auto_update Change the auto update flag

@returns
None.
*/
WOOTINGRGBSDK_API void wooting_rgb_array_auto_update(bool auto_update) {
	if (simulatedState) {
		simulatedState->autoUpdate = auto_update;
	}
}

/** @brief Set a single color in the colour array.

This function will set a single color in the colour array. This will not directly update the keyboard (unless the flag is set), so
it can be called frequently. For example in a loop that updates the entire keyboard. This way you can avoid dealing with C arrays from
different languages.

@ingroup API
@param row The horizontal index of the key
@param coloum The vertical index of the key
@param red 0-255 value of the red color
@param green 0-255 value of the green color
@param blue 0-255 value of the blue color

@returns
This functions return true (1) if the colours are changed (if auto update flag: updated).
*/
WOOTINGRGBSDK_API bool wooting_rgb_array_set_single(uint8_t row, uint8_t column, uint8_t red, uint8_t green, uint8_t blue) {
	if (simulatedState) {
		if (row >= WOOTING_RGB_ROWS || column >= WOOTING_RGB_COLS) {
			return false;
		}

		WootingColor color = *(&(simulatedState->keyboardColors[column + row * WOOTING_RGB_COLS]));
		color.red = red;
		color.green = green;
		color.blue = blue;

		if (simulatedState->autoUpdate) {
			return wooting_rgb_array_update_keyboard();
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

/** @brief Set a full colour array.

This function will set a complete color array. This will not directly update the keyboard (unless the flag is set).

If you use a non-C language it is recommended to use the wooting_rgb_array_set_single function to change the colors to avoid compatibility issues.

Buffer should be layout out as [Row0Col0Red, Row0Col0Green, Row0Col0Blue, Row0Col1Red, Row0Col1Green, Row0Col1Blue, ... Row5Row20Red, Row5Row20Green, Row5Row20Blue].
Expected size is 6 row * 21 columns * 3 colors per key = 576 bytes.

@ingroup API
@param colors_buffer Pointer to a buffer of a full color array

@returns
This functions return true (1) if the colours are changed (if auto update flag: updated).
*/
WOOTINGRGBSDK_API bool wooting_rgb_array_set_full(const uint8_t *colors_buffer) {
	if (simulatedState) {
		memcpy(simulatedState->keyboardColors, colors_buffer, WOOTING_RGB_ROWS * WOOTING_RGB_COLS * 3);

		if (simulatedState->autoUpdate) {
			return wooting_rgb_array_update_keyboard();
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}
