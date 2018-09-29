package net.gudenau.wooting.simulator;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import javax.imageio.ImageIO;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryUtil;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;

class KeyboardRenderer{
    private final List<Key> keys;

    KeyboardRenderer(){
        List<Key> keyList = new ArrayList<>();

        // Escape
        keyList.add(new Key(0, 0));
        // F1 though Mode
        for(int x = 2; x < 21; x++){
            keyList.add(new Key(x, 0));
        }
        // Tilde though num 9
        for(int y = 1; y < 3; y++){
            for(int x = 0; x < 20; x++){
                keyList.add(new Key(x, y));
            }
        }
        // Num -
        keyList.add(new Key(20, 1));
        // Num +
        keyList.add(new Key(20, 2, 1, 2, 2, 20));
        // Num return
        keyList.add(new Key(20, 4, 1, 2, 4, 20));
        // Caps though /
        for(int y = 3; y < 5; y++){
            for(int x = 0; x < 12; x++){
                keyList.add(new Key(x, y));
            }
        }
        // ISO
        keyList.add(new Key(12, 3));
        // Return
        keyList.add(new Key(13, 3));
        // Right shift
        keyList.add(new Key(12, 4, 2, 1, 4, 13));
        // Up arrow
        keyList.add(new Key(15, 4));
        // Num 4 though num 3
        for(int y = 3; y < 5; y++){
            for(int x = 17; x < 20; x++){
                keyList.add(new Key(x, y));
            }
        }
        // Left ctrl though left alt
        for(int x = 0; x < 4; x++){
            keyList.add(new Key(x, 5));
        }
        // Space
        keyList.add(new Key(3, 5, 7, 1, 5, 6));
        // Right alt though right arrow
        for(int x = 10; x < 17; x++){
            keyList.add(new Key(x, 5));
        }
        // Num 0
        keyList.add(new Key(17, 5, 2, 1, 5, 18));
        // Num .
        keyList.add(new Key(19, 5));

        keys = Collections.unmodifiableList(keyList);
    }

    void run(){
        glfwInit();

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        long window = glfwCreateWindow(1344, 384, "Wooting Simulator", 0, 0);
        glfwSwapInterval(1);
        glfwShowWindow(window);

        glfwMakeContextCurrent(window);
        GL.createCapabilities();

        glViewport(0, 0, 1344, 384);
        glOrtho(0, 1344, 384, 0, -1, 1);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        int texture = loadImage();

        glClearColor(0, 0, 0, 1);

        while(!glfwWindowShouldClose(window)){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDisable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
            synchronized(keys){
                for(Key key : keys){
                    int color = key.color;
                    float r = ((color >>> 16) & 0xFF) / 255F;
                    float g = ((color >>> 8) & 0xFF) / 255F;
                    float b = (color & 0xFF) / 255F;
                    glColor3f(r, g, b);

                    int x = key.x * 64;
                    int y = key.y * 64;
                    int x2 = (key.x + key.w) * 64;
                    int y2 = (key.y + key.h) * 64;
                    glVertex2f(x, y);
                    glVertex2f(x2, y);
                    glVertex2f(x2, y2);
                    glVertex2f(x, y2);
                }
            }
            glEnd();

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(0, 0);
            glTexCoord2f(0.65625f, 0);
            glVertex2f(1344, 0);
            glTexCoord2f(0.65625f, 0.75f);
            glVertex2f(1344, 384);
            glTexCoord2f(0, 0.75f);
            glVertex2f(0, 384);
            glEnd();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    List<Key> getKeys(){
        return keys;
    }

    Key getKey(int row, int col){
        return keys.stream().filter(k->k.row == row && k.col == col).findAny().orElse(null);
    }

    void keyLock(Runnable runnable){
        synchronized(keys){
            runnable.run();
        }
    }

    private int loadImage(){
        BufferedImage image;
        try{
            image = ImageIO.read(getClass().getResourceAsStream("/net/gudenau/wooting/simulator/res/keys.png"));
        }catch(IOException e){
            throw new RuntimeException(e);
        }

        int width = image.getWidth();
        int height = image.getHeight();
        int[] pixels = new int[width * height];
        image.getRGB(0, 0, width, height, pixels, 0, width);
        ByteBuffer pixelBuffer = MemoryUtil.memAlloc(pixels.length * 4);
        for(int pixel : pixels){
            pixelBuffer.putInt(pixel);
        }
        pixelBuffer.position(0);

        int texture = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixelBuffer
        );
        return texture;
    }
}
