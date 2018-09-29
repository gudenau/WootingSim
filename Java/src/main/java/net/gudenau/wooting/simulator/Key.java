package net.gudenau.wooting.simulator;

class Key{
    // Physical location and sizes
    final int x;
    final int y;
    final int w;
    final int h;

    // Logical location
    final int row;
    final int col;

    private final int originalColor = 0xFFFFFF;
    volatile int color = originalColor;

    Key(int x, int y){
        this(x, y, 1, 1, y, x);
    }

    Key(int x, int y, int w, int h, int row, int col){
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
        this.row = row;
        this.col = col;
    }

    void setColor(byte r, byte g, byte b){
        setColor(((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF));
    }

    void setColor(int color){
        this.color = color;
    }

    void resetColor(){
        this.color = originalColor;
    }
}
