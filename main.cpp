#include "GLFW/glfw3.h"
#include "GL/glut.h"
#include <stdlib.h>
#include <iostream>
#include <valarray>

float angle = 0.6;

float alpha = 0.6;

float translate = 0;
float delTr = 0.07;

int width = 1000;
int height = 1000;

GLfloat colors[] = {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,

        1, 0, 0, alpha,
        1, 0, 0, alpha,
        1, 0, 0, alpha,

        0, 1, 0, alpha,
        0, 1, 0, alpha,
        0, 1, 0, alpha,

        0, 0, 1, alpha,
        0, 0, 1, alpha,
        0, 0, 1, alpha,

        0, 1, 1, alpha,
        0, 1, 1, alpha,
        0, 1, 1, alpha,

        1, 0, 1, alpha,
        1, 0, 1, alpha,
        1, 0, 1, alpha,

        1, 1, 0, alpha,
        1, 1, 0, alpha,
        1, 1, 0, alpha,

        0.5, 0.5, 1, alpha,
        0.5, 0.5, 1, alpha,
        0.5, 0.5, 1, alpha,

        1, 0.5, 0.5, alpha,
        1, 0.5, 0.5, alpha,
        1, 0.5, 0.5, alpha,
};

GLfloat verticles[] = {
        -1, 0, 0,
        1, 0, 0,
        0, -1, 0,
        0, 1, 0,
        0, 0, -1,
        0, 0, 1,

        -1, 0, 0,
        0, -1, 0,
        0, 0, -1,

        1, 0, 0,
        0, 1, 0,
        0, 0, -1,

        1, 0, 0,
        0, -1, 0,
        0, 0, -1,

        1, 0, 0,
        0, 1, 0,
        0, 0, 1,

        -1, 0, 0,
        0, 1, 0,
        0, 0, 1,

        -1, 0, 0,
        0, 1, 0,
        0, 0, -1,

        -1, 0, 0,
        0, -1, 0,
        0, 0, 1,

        1, 0, 0,
        0, -1, 0,
        0, 0, 1,
};

void rotate(bool left) { glRotatef(left ? -angle : angle, 0, 1, 0); }

void octReshape(bool more) { translate += more ? delTr : -delTr; if (translate < 0) translate = 0; }

static void error_callback(int error, const char* description) { fputs(description, stderr); }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT | action == GLFW_PRESS) {
        switch (key) {
            case(GLFW_KEY_ESCAPE):
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case(GLFW_KEY_A):
                rotate(true);
                break;
            case(GLFW_KEY_D):
                rotate(false);
                break;
            case(GLFW_KEY_W):
                octReshape(true);
                break;
            case(GLFW_KEY_S):
                octReshape(false);
                break;
            default:
                break;
        }
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void reshape() {
    glLoadIdentity();

    if (width <= height)
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)height/(GLfloat)width, 1.5 * (GLfloat)height/(GLfloat)width, -10.0, 10.0);
    else
        glOrtho(-1.5*(GLfloat)width/(GLfloat)height, 1.5*(GLfloat)width/(GLfloat)height, -1.5, 1.5, -10.0, 10.0);
}

int main()
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(width, height, "lab3", nullptr, nullptr);
    glfwSetKeyCallback(window, key_callback);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // переключаем текущий контекст на это окно
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height); // Делаем экран OpenGL в соответствии с размером окна

    reshape();

    // включение массивов вершины и цвета
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // задаем массивы вершин и цвета
    glVertexPointer(3, GL_FLOAT, 0, verticles);
    glColorPointer(4, GL_FLOAT, 0, colors);

    // работа с глубиной
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    // включение прозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // перемещение камеры
    gluLookAt(0.1, 0.12, 0.1,
              0, 0, 0,
              0.0, 1.0, 0.0);

    // включение света
//    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glLineWidth(3.f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_LINES, 0, 6);
        for (int i = 18; i < 89; i += 9) {
            float x = verticles[i] + verticles[i + 3] + verticles[i + 6];
            float y = verticles[i + 1] + verticles[i + 4] + verticles[i + 7];
            float z = verticles[i + 2] + verticles[i + 5] + verticles[i + 8];
            glNormal3f(x, y, z);
            glPushMatrix();
                glTranslatef(x * translate, y * translate, z * translate);
                glDrawArrays(GL_TRIANGLES, i / 3, 3);
            glPopMatrix();
        }
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDisable(GL_BLEND);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    return 0;
}