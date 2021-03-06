#include "common.h"

#define PIPE_NAME TEXT("\\\\.\\pipe\\wootingsimulatorrgb")

void connectToSimulator(void);
void disconnectFromSimulator(void);

SimulatedState *simulatedState;

BOOL APIENTRY DllMain(
	HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
){
    switch (ul_reason_for_call){
    case DLL_PROCESS_ATTACH:
		connectToSimulator();
		break;
    case DLL_PROCESS_DETACH:
		disconnectFromSimulator();
        break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
    }
    return TRUE;
}

void connectToSimulator(void) {
	HANDLE pipeHandle;
	do {
		pipeHandle = CreateFile(
			PIPE_NAME,
			GENERIC_READ | GENERIC_WRITE,
			0,
			null,
			OPEN_EXISTING,
			0,
			null
		);

		if (pipeHandle == INVALID_HANDLE_VALUE) {
			if (GetLastError() != ERROR_PIPE_BUSY) {
				break;
			}
			if (!WaitNamedPipe(PIPE_NAME, 100)) {
				break;
			}
		}
	} while (pipeHandle == INVALID_HANDLE_VALUE);

	if (pipeHandle != INVALID_HANDLE_VALUE) {
		DWORD pipeMode = PIPE_READMODE_MESSAGE;

		if (!SetNamedPipeHandleState(
			pipeHandle,
			&pipeMode,
			null,
			null
		)) {
			simulatedState = null;
		}
		else {

			simulatedState = (SimulatedState*)HeapAlloc(
				GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				sizeof(SimulatedState)
			);

			if (simulatedState) {
				simulatedState->pipeHandle = pipeHandle;
			}
			else {
				CloseHandle(pipeHandle);
			}
		}
	}
	else {
		simulatedState = null;
	}
}

void disconnectFromSimulator(void) {
	if (simulatedState) {
		CloseHandle(simulatedState->pipeHandle);

		if (simulatedState->disconnectCallbacks) {
			for (size_t i = 0; i < simulatedState->disconnectCallbackCount; i++) {
				simulatedState->disconnectCallbacks[i]();
			}
		}

		HeapFree(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			simulatedState
		);
		simulatedState = null;
	}
}
