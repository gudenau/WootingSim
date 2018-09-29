package net.gudenau.wooting.simulator;

public class WootingSimulator{
    private static Thread pipeThread;

    public static void main(String[] arguments){
        KeyboardRenderer keyboardRenderer = new KeyboardRenderer();

        pipeThread = new Thread(new PipeServer(keyboardRenderer)::run, "Pipe server");
        pipeThread.setDaemon(true);
        pipeThread.start();

        keyboardRenderer.run();
    }
}
