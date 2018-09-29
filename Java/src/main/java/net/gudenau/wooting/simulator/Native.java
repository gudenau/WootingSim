package net.gudenau.wooting.simulator;

import java.io.*;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.ByteBuffer;

public class Native{
    static{
        URL location = Native.class.getProtectionDomain().getCodeSource().getLocation();
        File file;
        try{
            file = new File(location.toURI());
        }catch(URISyntaxException ignored){
            file = new File(location.toString());
        }
        if(file.isDirectory()){
            System.load("D:\\projects\\Wooting\\WootingSimulator\\x64\\Debug\\SimulatorNatives.dll");
        }else{
            try(InputStream inputStream = Native.class.getResourceAsStream("/net/gudenau/wooting/simulator/res/natives/windows.amd64.dll");
                OutputStream outputStream = new FileOutputStream("windows.amd64.dll")){
                inputStream.transferTo(outputStream);
            }catch(IOException e){
                throw new RuntimeException(e);
            }
            System.load(new File("windows.amd64.dll").getAbsolutePath());
        }
    }

    public native static long CreateNamedPipe(
        String lpName,
        int dwOpenMode,
        int dwPipeMode,
        int nMaxInstances,
        int nOutBufferSize,
        int nInBufferSize,
        int nDefaultTimeOut
    );

    public native static boolean ConnectNamedPipe(
        long hNamedPipe
    );

    public native static boolean ReadFile(
        long hFile,
        long lpBuffer,
        int nNumberOfBytesToRead,
        long lpNumberOfBytesRead
    );

    public native static boolean WriteFile(
        long hFile,
        long lpBuffer,
        int nNumberOfBytesToWrite,
        long lpNumberOfBytesWritten
    );

    public native static boolean CloseHandle(
        long hObject
    );

    public native static int GetLastError();

    public native static long getByteBufferAddress(
        ByteBuffer buffer
    );
}
