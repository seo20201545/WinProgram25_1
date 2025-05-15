#pragma comment(lib, "Opengl32.lib")
#include <GLFW/glfw3.h>
#include <math.h>

#define PI 3.14159265358979323846

void drawEllipse(float cx, float cy, float rx, float ry, int segments, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        float x = cosf(angle) * rx;
        float y = sinf(angle) * ry;
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawEllipseOutline(float cx, float cy, float rx, float ry, int segments, float r, float g, float b, float thickness) {
    glLineWidth(thickness);
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        float x = cosf(angle) * rx;
        float y = sinf(angle) * ry;
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawWhisker(float x1, float y1, float x2, float y2) {
    glLineWidth(2.0f);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

int main() {
    const int WIDTH = 537;
    const int HEIGHT = 445;

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bonobono", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, WIDTH, HEIGHT, WIDTH, HEIGHT);

    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float cx = WIDTH / 2.0f;
        float cy = HEIGHT / 2.0f;

        // ¾ó±¼ ¾Æ¿ô¶óÀÎ + ³»ºÎ
        drawEllipseOutline(cx, cy, 215.0f, 192.5f, 100, 1, 1, 1, 2.0f);
        drawEllipse(cx, cy, 213.0f, 190.5f, 100, 0.0f, 112.0f / 255.0f, 192.0f / 255.0f);

        // ´«
        drawEllipse(89.0f, HEIGHT - 177.5f, 16.0f, 15.5f, 100, 0, 0, 0); // ¿ÞÂÊ´« at (73,162)
        drawEllipse(428.0f, HEIGHT - 184.5f, 16.0f, 15.5f, 100, 0, 0, 0); // ¿À¸¥ÂÊ´« at (412,169)

        // ÁÖµÕÀÌ (Å©±â 83x75)
        drawEllipse(192.0f + 41.5f, HEIGHT - (251.0f + 37.5f), 41.5f, 37.5f, 100, 1, 1, 1); // ¿ÞÂÊ
        drawEllipse(275.0f + 41.5f, HEIGHT - (251.0f + 37.5f), 41.5f, 37.5f, 100, 1, 1, 1); // ¿À¸¥ÂÊ

        // ÁÖµÕÀÌ »çÀÌ °ø¹é ÇÏ¾á»ö Å¸¿øÀ¸·Î Ã¤¿ò
        drawEllipse(268.0f + 7.5f, HEIGHT - (252.0f + 7.5f), 30.0f, 30.0f, 100, 1, 1, 1);

        // ÄÚ (ÁÂÇ¥ 242, 217 ¡æ Áß½É (242 + 66/2, 217 + 55/2))
        drawEllipse(242.0f + 33.0f, HEIGHT - (217.0f + 27.5f), 33.0f, 27.5f, 100, 0, 0, 0);

        // ¼ö¿° ¿ÞÂÊ
        drawWhisker(cx - 35, cy - 40, cx - 90, cy - 35);
        drawWhisker(cx - 35, cy - 50, cx - 90, cy - 50);
        drawWhisker(cx - 35, cy - 60, cx - 90, cy - 65);

        // ¼ö¿° ¿À¸¥ÂÊ
        drawWhisker(cx + 35, cy - 40, cx + 90, cy - 35);
        drawWhisker(cx + 35, cy - 50, cx + 90, cy - 50);
        drawWhisker(cx + 35, cy - 60, cx + 90, cy - 65);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
