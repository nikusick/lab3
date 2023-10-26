#include "GLFW/glfw3.h"
#include "GL/glut.h"
#include <stdlib.h>
#include <iostream>
#include <valarray>

float angle = 0.6;

float alpha = 0.59;

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

GLfloat vert[] = {
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

void octReshape(bool more) {
    float x, y, z;
    for (int i = 18; i < 89; i+=3) {
        x = vert[i];
        y = vert[i + 1];
        z = vert[i + 2];
        if (x > 0) vert[i] += more ? 0.1f: -0.1f;
        else if (x < 0) vert[i] -= (more ? 0.1f: -0.1f);
        if (y > 0) vert[i + 1] += (more ? 0.1f: -0.1f);
        else if (y < 0) vert[i + 1] -= (more ? 0.1f: -0.1f);
        if (z > 0) vert[i + 2] += (more ? 0.1f: -0.1f);
        else if (z < 0) vert[i + 2] -= (more ? 0.1f: -0.1f);
    }
    glVertexPointer(3, GL_FLOAT, 0, vert);
}

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

void reshape(int width, int height) {
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
    window = glfwCreateWindow(800, 900, "lab3", nullptr, nullptr);
    glfwSetKeyCallback(window, key_callback);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // переключаем текущий контекст на это окно
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 900); // Делаем экран OpenGL в соответствии с размером окна

    glMatrixMode(GL_PROJECTION);

    reshape(800, 900);

    glLineWidth(3.f);
    // включение массивов вершины и цвета
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // указываем размер этих массивов (сколько элементов будет прочитано за один проход)
    glVertexPointer(3, GL_FLOAT, 0, vert); //3 - потому что 3 координаты для каждой вершины
    glColorPointer(4, GL_FLOAT, 0, colors);
    gluLookAt(0.1, 0.12, 0.1, // Положение глаз, взгляд "из"
              0, 0, 0, // Цель, взгляд "на"
              0.0, 1.0, 0.0); // Пока игнорируем
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_LINES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 24);
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDisable(GL_BLEND);
    return 0;
}