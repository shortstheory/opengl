// #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

GLfloat pointVertex[1000][2];

void drawPixel(GLfloat x, GLfloat y)
{
    GLfloat pointVertex[] = {x, y};
    glEnable(GL_POINT_SMOOTH); // make the point circular
    glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute
    glPointSize(2); // must be added before glDrawArrays is called
    glVertexPointer(2, GL_FLOAT, 0, pointVertex); // point to the vertices to be used
    glDrawArrays(GL_POINTS, 0, 1); // draw the vertixes
    glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex arrayattribute
    glDisable(GL_POINT_SMOOTH); // stop the smoothing to make the points circular
}

void drawLine(GLfloat start_x, GLfloat start_y, GLfloat end_x, GLfloat end_y)
{
    if (start_x > end_x) {
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }
    int dx = end_x - start_x;
    int dy = end_y - start_y;

    if (dy > 0) {
        int p = 2*dy - dx; //initial delta
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

int main( void )
{
    GLFWwindow *window;

    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        // bresenham(200, 100, 400, 300);
        drawLine(400, 600, 200, 200);
        drawLine(0, 400, 200, 200);

        // Swap front and back buffers
        glfwSwapBuffers( window );

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}