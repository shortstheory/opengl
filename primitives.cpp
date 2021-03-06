#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <tuple>
#include <unistd.h>

#define PI 3.14159265
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void drawPixel(GLfloat x, GLfloat y)
{
    GLfloat pointVertex[] = {x, y};

    GLfloat red = 0;
    GLfloat green = 1;
    GLfloat blue =0;
    GLfloat color_vector[] = {red, green, blue};

    glColorPointer(3, GL_FLOAT, 0, color_vector);

    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(2);
    glVertexPointer(2, GL_FLOAT, 0, pointVertex);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_POINT_SMOOTH);
}

void drawLine(GLfloat start_x, GLfloat start_y, GLfloat end_x, GLfloat end_y)
{
    if (start_x > end_x) {
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }
    int dx = end_x - start_x;
    int dy = end_y - start_y;

    //when line is steeper than 1
    if (abs(dy) > abs(dx)) {
        if (dy > 0) { //when line has m>1 && m<=infinity
            int p = -2*dx + dy; //initial delta
            int northDelta = -2*dx;
            int northEastDelta = 2*dy - 2*dx;
            for (int x = start_x, y = start_y; y<= end_y; y++) {
                if (p > 0) {
                    p = p + northDelta;
                } else {
                    p = p + northEastDelta;
                    x++;
                }
                drawPixel(x, y);
            }
        } else { //when it spills over to second quadrant but still has abs(m) > 1
            int p = 2*dx - dy; //initial delta
            int southDelta = 2*dx;
            int southEastDelta = 2*(dy + dx);
            for (int x = start_x, y = start_y; y >= end_y; y--) {
                if (p < 0) {
                    p = p + southDelta;
                } else {
                    p = p + southEastDelta;
                    x++;
                }
                drawPixel(x, y);
            }
        }
    } else {
        if (dy > 0) {
            int p = 2*dy - dx;
            int eastDelta = 2*dy;
            int northEastDelta = 2*(dy - dx);
            for (int x = start_x, y = start_y; x<= end_x; x++) {
                if (p < 0) {
                    p = p + eastDelta;
                } else {
                    p = p + northEastDelta;
                    y++;
                }
                drawPixel(x, y);
            }
        } else {
            int p = 2*dy + dx; //initial delta
            int eastDelta = 2*dy;
            int southEastDelta = 2*(dy + dx);
            for (int x = start_x, y = start_y; x<= end_x; x++) {
                if (p > 0) {
                    p = p + eastDelta;
                } else {
                    p = p + southEastDelta;
                    y--;
                }
                drawPixel(x, y);
            }
        }
    }
}

void drawCirclePixel(int x, int y, int origin_x, int origin_y)
{
    GLfloat _x = x;
    GLfloat _y = y;

    GLfloat _ox = origin_x;
    GLfloat _oy = origin_y;
    // draw pixel 8 times for covering symmetry of the circle
    drawPixel(_x+_ox, _y+_oy);
    drawPixel(_x*-1+_ox, _y+_oy);
    drawPixel(_x+_ox, _y*-1+_oy);
    drawPixel(_x*-1+_ox, _y*-1+_oy);
    drawPixel(_y+_ox, _x+_oy);
    drawPixel(_y*-1+_ox, _x+_oy);
    drawPixel(_y+_ox, _x*-1+_oy);
    drawPixel(_y*-1+_ox, _x*-1+_oy);

}

void drawCircle(int origin_x, int origin_y, int radius)
{
    int x = 0;
    int y = radius;

    int d = 1 - radius;
    int deltaE = 3;
    int deltaSE = -2*radius + 5;
    drawCirclePixel(x, y, origin_x, origin_y);

    while(y > x) {
        if (d < 0) { //east pixel
            d += deltaE;
            deltaE += 2;
            deltaSE += 2;
        } else { //south-east pixel
            d += deltaSE;
            deltaE += 2;
            deltaSE += 4;
            y--;
        }
        x++;
        drawCirclePixel(x, y, origin_x, origin_y);
    }
}

void demoPrimitiveDrawing()
{
    float x = 0; float y = 0;
    float radius = 200;
    for (int deg = 0; deg < 360; deg+=10) {
        x = radius*cos(deg*PI/180);
        y = radius*sin(deg*PI/180);
        drawLine(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, x+SCREEN_WIDTH/2, y+SCREEN_HEIGHT/2);
    }
    drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 200);
    drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 220);
}

int main()
{
    GLFWwindow *window;
    // Initialize the library
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "IS F311 Computer Graphics Assignment", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        demoPrimitiveDrawing();
        glfwSwapBuffers(window);
        sleep(1);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
