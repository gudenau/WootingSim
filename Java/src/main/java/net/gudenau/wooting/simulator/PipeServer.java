package net.gudenau.wooting.simulator;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static net.gudenau.wooting.simulator.Native.*;

class PipeServer{
    private static final String PIPE_NAME = "\\\\.\\pipe\\wootingsimulatorrgb";
    private static final int WOOTING_RGB_ROWS = 6;
    private static final int WOOTING_RGB_COLS = 21;
    private final KeyboardRenderer renderer;

    private volatile boolean running = true;

    public PipeServer(KeyboardRenderer keyboardRenderer){
        this.renderer = keyboardRenderer;
    }

    void run(){
        while(running){
            @SuppressWarnings("PointlessBitwiseExpression")
            long pipeHandle = CreateNamedPipe(
                PIPE_NAME,
                0x00000003 /*PIPE_ACCESS_DUPLEX*/,
                0x00000004 /*PIPE_TYPE_MESSAGE*/ | 0x00000002/*PIPE_READMODE_MESSAGE*/ | 0x00000000/*PIPE_WAIT*/,
                255/*PIPE_UNLIMITED_INSTANCES*/,
                388 /*sizeof(SimulatedCommandPacket)*/,
                388 /*sizeof(SimulatedCommandPacket)*/,
                0
            );

            if (pipeHandle == -1 /*INVALID_HANDLE_VALUE*/) {
                System.err.println("Failed to create named pipe");
                System.err.println(GetLastError());
                return;
            }

            if(!ConnectNamedPipe(pipeHandle)){
                System.err.println("Failed to connect to client");
                System.err.println(GetLastError());
                CloseHandle(pipeHandle);
                continue;
            }

            ByteBuffer commandBuffer = ByteBuffer.allocateDirect(388).order(ByteOrder.nativeOrder());
            ByteBuffer responseBuffer = ByteBuffer.allocateDirect(388).order(ByteOrder.nativeOrder());
            ByteBuffer intBuffer = ByteBuffer.allocateDirect(Integer.BYTES).order(ByteOrder.nativeOrder());

            long commandBufferAddress = getByteBufferAddress(commandBuffer);
            long responseBufferAddress = getByteBufferAddress(responseBuffer);
            long intBufferAddress = getByteBufferAddress(intBuffer);

            int readBytes;
            int writtenBytes;

            while(true){
                if(!ReadFile(
                    pipeHandle,
                    commandBufferAddress,
                    388 /*sizeof(SimulatedCommandPacket)*/,
                    intBufferAddress
                )){
                    CloseHandle(pipeHandle);
                    System.err.println("Failed to read from client");
                    System.err.println(GetLastError());
                    break;
                }
                readBytes = intBuffer.getInt(0);

                int command = commandBuffer.getInt(0);
                long size = commandBuffer.getInt(4);

                switch(SimulatedCommand.values()[command]){
                    case ack: break;
                    case nack: break;
                    case reset: handleReset(); break;
                    case setKey: handleSetKey(
                        commandBuffer.get(8),
                        commandBuffer.get(9),
                        commandBuffer.get(10),
                        commandBuffer.get(11),
                        commandBuffer.get(12)
                    ); break;
                    case resetKey: handleResetKey(
                        commandBuffer.get(8),
                        commandBuffer.get(9)
                    ); break;
                    case update: handleUpdate(
                        getBytes(commandBuffer, 8, WOOTING_RGB_ROWS * WOOTING_RGB_COLS * 3)
                    ); break;
                }

                responseBuffer.putInt(0, SimulatedCommand.ack.ordinal());
                responseBuffer.putInt(4, 4+4+4);
                responseBuffer.putInt(4+4, command);

                if(!WriteFile(
                    pipeHandle,
                    responseBufferAddress,
                    responseBuffer.getInt(4),
                    intBufferAddress
                )){
                    System.err.println("Failed to write to client");
                    System.err.println(GetLastError());
                    CloseHandle(pipeHandle);
                    break;
                }
            }
        }
    }

    private byte[] getBytes(ByteBuffer byteBuffer, int offset, int size){
        byte[] buffer = new byte[size];
        byteBuffer.position(offset);
        byteBuffer.get(buffer);
        byteBuffer.position(0);
        return buffer;
    }

    private void handleUpdate(byte[] bytes){
        System.out.println("Full keyboard update");
        renderer.keyLock(()->
            renderer.getKeys().forEach(k->{
                int startingIndex = (k.row * 21 + k.col) * 3;
                k.setColor(
                    bytes[startingIndex],
                    bytes[startingIndex + 1],
                    bytes[startingIndex + 2]
                );
            })
        );
    }

    private void handleResetKey(byte row, byte col){
        System.out.printf("Single key reset: %d %d\n", row, col);
        renderer.keyLock(()->renderer.getKey(row, col).resetColor());
    }

    private void handleSetKey(byte r, byte g, byte b, byte row, byte col){
        System.out.printf("Single key set: %d %d %02X%02X%02X\n", row, col, r, g, b);
        renderer.keyLock(()->{
            var key = renderer.getKey(row, col);
            if(key != null){
                key.setColor(r, g, b);
            }
        });
    }

    private void handleReset(){
        System.out.println("Full keyboard reset");
        renderer.keyLock(()->renderer.getKeys().forEach(Key::resetColor));
    }
}
