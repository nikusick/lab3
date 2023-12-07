#define STB_IMAGE_IMPLEMENTATION
#include "GLFW/glfw3.h"
#include "GL/glut.h"
#include <stdlib.h>
#include <iostream>
#include "stb_image.h"
#include "coordinates.h"

struct Parameters {
    bool light = false;
    bool blending = true;
    bool textures = true;
};

Parameters params;

void rotate(bool left) { glRotatef(left ? -angle : angle, 0, 1, 0); }

void lRotate(bool left) {
    lightAngle += (left ? -0.5f: 0.5f);
//    if (lightAngle < 0 || lightAngle >= M_PI * 2) lightAngle = 0;
}

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
            case(GLFW_KEY_LEFT):
                lRotate(true);
                break;
            case(GLFW_KEY_RIGHT):
                lRotate(false);
                break;
            case(GLFW_KEY_W):
                octReshape(true);
                break;
            case(GLFW_KEY_S):
                octReshape(false);
                break;
            case(GLFW_KEY_1):
                params.light = !params.light;
                break;
            case(GLFW_KEY_2):
                params.textures = !params.textures;
                break;
            case(GLFW_KEY_3):
                params.blending = !params.blending;
                break;
            default:
                break;
        }
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void loadImage(const char* path) {
    int w, h, nrChannels;
    unsigned char* data = stbi_load(path, &w, &h, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &texture);
        stbi_set_flip_vertically_on_load(GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    else std::cout << "Failed" << std::endl;
}

void enable() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glLineWidth(3.f);

    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    if (params.textures) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        loadImage("../images/flowers.png");
        glEnable(GL_TEXTURE_2D);
    }
    else glEnableClientState(GL_COLOR_ARRAY);
    if (params.blending) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        GLfloat shininess[] = {25.f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
    if (params.light) {
        glEnable(GL_LIGHTING);
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
    }
}

void disable() {
    glDisable(GL_BLEND);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
}

void addLight() {
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();

    glPushMatrix();
        glRotatef(lightAngle, 0, 1, 0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        GLfloat amb[] = {0.3f, 0.3f, 0.3f, 1};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

        glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
        glColor3f(1.0f, 1.0f, 1.0f);
        gluSphere(quadObj, 0.1, 20, 20);
    glPopMatrix();
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

    glLoadIdentity();

    if (width <= height)
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)height/(GLfloat)width, 1.5 * (GLfloat)height/(GLfloat)width, -10.0, 10.0);
    else
        glOrtho(-1.5*(GLfloat)width/(GLfloat)height, 1.5*(GLfloat)width/(GLfloat)height, -1.5, 1.5, -10.0, 10.0);

    // задаем массивы вершин, цвета и текстур
    glVertexPointer(3, GL_FLOAT, 0, verticles);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    // перемещение камеры
    gluLookAt(0.1, 0.12, 0.1,
              0, 0, 0,
              0.0, 1.0, 0.0);


    while (!glfwWindowShouldClose(window)) {
        enable();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_LINES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, texture);
        for (int i = 18; i < 89; i += 9) {
            float x = verticles[i] + verticles[i + 3] + verticles[i + 6];
            float y = verticles[i + 1] + verticles[i + 4] + verticles[i + 7];
            float z = verticles[i + 2] + verticles[i + 5] + verticles[i + 8];
            glPushMatrix();
                glTranslatef(x * translate, y * translate, z * translate);
                glDrawArrays(GL_TRIANGLES, i / 3, 3);
            glPopMatrix();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        if (params.light) addLight();
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
        disable();
    }
    return 0;
}