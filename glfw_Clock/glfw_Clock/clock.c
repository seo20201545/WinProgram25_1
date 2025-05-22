#pragma comment(lib, "Opengl32.lib")
#include <GLFW/glfw3.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846f

void drawCircle(float cx, float cy, float radiusX, float radiusY, int segments, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(cx + cosf(angle) * radiusX, cy + sinf(angle) * radiusY);
    }
    glEnd();
}

void drawPolygonLine(float x1, float y1, float x2, float y2, float width, float r, float g, float b) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float len = sqrtf(dx * dx + dy * dy);
    if (len == 0) return;

    float nx = -dy / len;
    float ny = dx / len;
    float halfWidth = width / 2.0f;

    float px1 = x1 + nx * halfWidth;
    float py1 = y1 + ny * halfWidth;
    float px2 = x1 - nx * halfWidth;
    float py2 = y1 - ny * halfWidth;
    float px3 = x2 - nx * halfWidth;
    float py3 = y2 - ny * halfWidth;
    float px4 = x2 + nx * halfWidth;
    float py4 = y2 + ny * halfWidth;

    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(px1, py1);
    glVertex2f(px2, py2);
    glVertex2f(px3, py3);
    glVertex2f(px4, py4);
    glEnd();
}

void drawClockMarks(float cx, float cy, float radius, int count, float width, float r, float g, float b, float offsetAngle) {
    for (int i = 0; i < count; i++) {
        float angle = 2.0f * PI * i / count - offsetAngle;
        float len = (i % 5 == 0) ? 35.0f : 17.5f;
        float x1 = cx + sinf(angle) * (radius - len);
        float y1 = cy + cosf(angle) * (radius - len);
        float x2 = cx + sinf(angle) * radius;
        float y2 = cy + cosf(angle) * radius;
        drawPolygonLine(x1, y1, x2, y2, width, r, g, b);
    }
}

void drawHand(float cx, float cy, float angle, float length, float width, float r, float g, float b, float offsetAngle) {
    float rad = -(angle - offsetAngle) * PI / 180.0f;
    float x = cx + cosf(rad) * length;
    float y = cy + sinf(rad) * length;
    drawPolygonLine(cx, cy, x, y, width, r, g, b);
}

int main() {
    const int WIDTH = 500, HEIGHT = 500;
    const float CLOCK_RADIUS = 200;
    const float CLOCK_CX = WIDTH / 2.0f;
    const float CLOCK_CY = HEIGHT / 2.0f;
    const float OFFSET_DEG = 90.0f;

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Clock", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        float hour = (t->tm_hour % 12) * 30.0f + t->tm_min * 0.5f;
        float minute = t->tm_min * 6.0f;
        float second = t->tm_sec * 6.0f;

        drawCircle(CLOCK_CX, CLOCK_CY, CLOCK_RADIUS, CLOCK_RADIUS, 100, 1, 1, 1);
        drawClockMarks(CLOCK_CX, CLOCK_CY, CLOCK_RADIUS, 60, 2.0f, 0, 0, 0, OFFSET_DEG * PI / 180.0f);
        drawCircle(CLOCK_CX, CLOCK_CY, 10, 10, 100, 0, 0, 0);

        drawHand(CLOCK_CX, CLOCK_CY, hour, 80, 10, 0, 0, 0, OFFSET_DEG);
        drawHand(CLOCK_CX, CLOCK_CY, minute, 150, 10, 0, 0, 0, OFFSET_DEG);

        float subCX = 118 + 50;
        float subCY = 178 + 50 + 50;
        drawCircle(subCX, subCY, 50, 50, 100, 0, 0, 0);

        for (int i = 0; i < 60; i++) {
            float angle = -2.0f * PI * i / 60 + PI / 2;
            float len = (i % 5 == 0) ? 10.0f : 5.0f;
            float x1 = subCX + cosf(angle) * (50 - len);
            float y1 = subCY + sinf(angle) * (50 - len);
            float x2 = subCX + cosf(angle) * 50;
            float y2 = subCY + sinf(angle) * 50;
            drawPolygonLine(x1, y1, x2, y2, 1, 1, 1, 1);
        }

        drawHand(subCX, subCY, second, 35, 2, 1, 1, 1, OFFSET_DEG);

        // 중심 기준 위로 80만큼 떨어진 위치에 검정색 직사각형 (150x50)
        float rectX = CLOCK_CX - 75;
        float rectY = CLOCK_CY - 80 - 25;
        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(rectX, rectY);
        glVertex2f(rectX + 150, rectY);
        glVertex2f(rectX + 150, rectY + 50);
        glVertex2f(rectX, rectY + 50);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
