#include <stdio.h>

#include <Windows.h>

#include "common.h"

#define PIPE_NAME TEXT("\\\\.\\pipe\\wootingsimulatorrgb")

int main(int argc, char** argv){
	printf("%ld\n", sizeof(size_t));
	exit(0);
	
	HANDLE pipeHandle = CreateNamedPipe(
		PIPE_NAME,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		sizeof(SimulatedCommandPacket),
		sizeof(SimulatedCommandPacket),
		0,
		null
	);

	if (pipeHandle == INVALID_HANDLE_VALUE) {
		printf("Failed to create named pipe\n");
		return 1;
	}

	if (!ConnectNamedPipe(
		pipeHandle,
		null
	)) {
		CloseHandle(pipeHandle);
		return 1;
	}

	SimulatedCommandPacket command;
	SimulatedCommandPacket responce;
	while (true) {
		DWORD readBytes;
		if (!ReadFile(
			pipeHandle,
			&command,
			sizeof(SimulatedCommandPacket),
			&readBytes,
			null
		)) {
			CloseHandle(pipeHandle);
			return 2;
		}

		if (readBytes != command.meta.size) {
			CloseHandle(pipeHandle);
			return 3;
		}

		switch (command.meta.command) {
		case SimulatedCommand_ack: printf("SimulatedCommand_ack\n"); break;
		case SimulatedCommand_nack: printf("SimulatedCommand_nack\n"); break;
		case SimulatedCommand_reset: printf("SimulatedCommand_reset\n"); break;
		case SimulatedCommand_setKey: printf("SimulatedCommand_setKey\n"); break;
		case SimulatedCommand_resetKey: printf("SimulatedCommand_resetKey\n"); break;
		case SimulatedCommand_update: printf("SimulatedCommand_update\n"); break;
		}

		responce.meta.command = SimulatedCommand_ack;
		responce.meta.size = sizeof(SimulatedCommandPacketMeta) + sizeof(SimulatedCommandAck);
		responce.data.ack.command = command.meta.command;
		DWORD written;
		if (!WriteFile(
			pipeHandle,
			&responce,
			responce.meta.size,
			&written,
			null
		)) {
			CloseHandle(pipeHandle);
			return 4;
		}
	}
}
