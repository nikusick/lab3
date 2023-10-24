#include "GLFW/glfw3.h"
#include "GL/glut.h"
#include <stdlib.h>
#include <iostream>
#include <valarray>

float angle = M_PI;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

GLubyte a1[] = { 0, 2, 4 };
GLubyte a2[] = { 1, 3, 4 };
GLubyte a3[] = { 1, 2, 4 };
GLubyte a4[] = { 1, 3, 5 };
GLubyte a5[] = { 0, 3, 5 };
GLubyte a6[] = { 0, 3, 4 };
GLubyte a7[] = { 0, 2, 5 };
GLubyte a8[] = { 1, 2, 5 };

static GLint vertices[] = {
    -1, 0, 0,
    1, 0, 0,
    0, -1, 0,
    0, 1, 0,
    0, 0, -1,
    0, 0, 1
};

void rotate(bool left) {
    if (left) {
        if (angle <= 0.001)  angle = M_PI * 2;
        angle -= 0.001;
        gluLookAt(std::cos(angle) - std::cos(angle + 0.001), 0, std::sin(angle) - std::sin(angle + 0.001), // Положение глаз, взгляд "из"
                  0, 0, 0, // Цель, взгляд "на"
                  0.0, 1.0, 0.0); // Пока игнорируем
    }
    else {
        if (angle >= M_PI * 2) angle = 0;
        angle += 0.001;
        gluLookAt(std::cos(angle) - std::cos(angle - 0.001), 0, std::sin(angle) - std::sin(angle - 0.001), // Положение глаз, взгляд "из"
                  0, 0, 0, // Цель, взгляд "на"
                  0.0, 1.0, 0.0); // Пока игнорируем
    }

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
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
            default:
                break;
        }
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    int width = 800;
    int height = 900;
    window = glfwCreateWindow(width, height, "lab3", nullptr, nullptr);
    glfwSetKeyCallback(window, key_callback);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // переключаем текущий контекст на это окно
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height); // Делаем экран OpenGL в соответствии с размером окна

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if (width <= height)

        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)height/(GLfloat)width, 1.5 * (GLfloat)height/(GLfloat)width, -10.0, 10.0);

    else
        glOrtho(-1.5*(GLfloat)width/(GLfloat)height, 1.5*(GLfloat)width/(GLfloat)height, -1.5, 1.5, -10.0, 10.0);
    glLineWidth(3.f);
    //включение массивов вершины и цвета
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//    glEnableClientState(GL_COLOR_ARRAY);
    //указываем размер этих массивов (сколько элементов будет прочитано за один проход)
    glVertexPointer(3, GL_INT, 0, vertices); //3 - потому что 3 координаты для каждой вершины
    gluLookAt(std::cos(angle), 0.15, std::sin(angle), // Положение глаз, взгляд "из"
              0, 0, 0, // Цель, взгляд "на"
              0.0, 1.0, 0.0); // Пока игнорируем
    int alpha = 150;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_LINES, 0, 6);
        glColor4ub(255, 0, 0, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a1);
        glColor4ub(0, 255, 0, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a2);
        glColor4ub(0, 0, 255, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a3);
        glColor4ub(0, 255, 255, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a4);
        glColor4ub(255, 0, 255, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a5);
        glColor4ub(25, 25, 25, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a6);
        glColor4ub(25, 75, 25, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a7);
        glColor4ub(25, 25, 75, alpha);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, a8);
        glColor3ub(255, 255, 255);
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
        rotate(true);
        _sleep(1000);
    }
    glDisable(GL_BLEND);
    return 0;
}