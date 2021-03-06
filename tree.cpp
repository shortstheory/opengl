/** @file */
#include "turtle.h"
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 800

/// animate the drawing of each branch
const bool animate = false;

string generatedString;

/// Draw pattern given the generated l system string and other graphics parameters
/// Parameters: string sentence : Generated l system string
/// int turnAngle : Interpret + and - with this angle
/// GLFWwindow * window: Pointer to window (for animation)

void drawPattern(string sentence, int turnAngle, int season, GLFWwindow* window)
{
    Turtle turtle;
    float angle = (PI / 180) * turnAngle;

    turtle.translate(WIDTH/2, 0);

    int draw_num = 0;
    int trunk = 1; // To keep track when to stop tapering and start branch contraction
    for (int i = 0; i < sentence.length(); i++) {
        char current = sentence[i];


        if (current == 'F') {
            turtle.draw();
            if (draw_num++ % 5 == 0 && trunk) {
                turtle.setThickness(-1); // -1 corresponds to taper
            }
        } else if (current == '+') {
            turtle.rotate(-angle);
        } else if (current == '-') {
            turtle.rotate(angle);
        } else if (current == '[') {
            if (animate) {
                glfwSwapBuffers(window);
                usleep(0.001 * 1e6);
            }
            turtle.saveState();
            turtle.setThickness(0); // 0 corresponds to branch contraction
        } else if (current == ']') {
            turtle.restoreState();
            turtle.setThickness(0); // 0 corresponds to branch contraction
        } else if (current == 'X' && sentence[i + 1] != '[') {
            trunk = 0; // Stop tapering
            turtle.drawLeaf(season);
        }
    }
}


/// Given an axiom, use defined production rules to recursively generate a string.
/// Stores generated string in global variable `generatedString`
/// Implementation of an l system using context-free grammar.
/// Parameters:
/// string sentence : String generated until preveious recursive step (initially the axiom)
/// int depth : Depth of recursion for generating string

void generateString(string sentence, int depth)
{
    if(depth == 0) {
        generatedString = sentence;
        return;
    }

    string nextSentence;

    for (int i = 0; i < sentence.length(); i++) {
        if (sentence[i] == 'X') {
            // nextSentence += "F-[[X]+X]+F[-FX]+X";
            int choice = rand() % 4;
            if (choice == 0)
                nextSentence += "F-[[X]+X]+F[[X]+X]-X";
            else if (choice == 1)
                nextSentence += "F-[[X]+X]+F[-FX]+X";
            else if (choice == 2)
                nextSentence += "F[+X][-X]FX";
            else if (choice == 3)
                nextSentence += "F[+X]F[-X]+X";
            continue;
        }
        if (sentence[i] == 'F') {
            nextSentence += "FF";
            continue;
        }
        nextSentence += sentence[i];
    }
    generateString(nextSentence, depth - 1);
}


int main()
{
    const int generations = 7;

    srand(time(NULL));

    // OpenGL initialisation code

    if(glfwInit() == false) {
        fprintf(stderr, "Error initializing GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "IS F311 Computer Graphics Project", NULL, NULL);

    if(!window) {
        glfwTerminate();
        fprintf(stderr, "Error while creating a window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);
    glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    string sentence = "X";
    int depth = 1;
    Turtle turtle;
    int turnAngle = 15 + rand() % 20;
    int season = 0;
    // Render loop for the OpenGL window. The scene is redrawn on every refresh.

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        if (depth < generations) {
            generateString(sentence, depth++);
        }
        season = (season + 1) % 3;
        drawPattern(generatedString, turnAngle, season, window);

        glfwSwapBuffers(window);
        glfwPollEvents();
        // usleep(0.25 * 1e6);
        sleep(1);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
