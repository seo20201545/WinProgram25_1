#pragma comment(lib, "Opengl32.lib")
#include <GLFW/glfw3.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define PI 3.14159265358979323846f

void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawSegment(float x, float y, float scale, int segment) {
    float thickness = scale * 0.1f;
    float length = scale;

    switch (segment) {
    case 0: drawRect(x, y + length, length, thickness, 1, 1, 1); break;              // top
    case 1: drawRect(x + length, y + length, thickness, length, 1, 1, 1); break;     // top-right
    case 2: drawRect(x + length, y, thickness, length, 1, 1, 1); break;              // bottom-right
    case 3: drawRect(x, y - thickness, length, thickness, 1, 1, 1); break;           // bottom
    case 4: drawRect(x - thickness, y, thickness, length, 1, 1, 1); break;           // bottom-left
    case 5: drawRect(x - thickness, y + length, thickness, length, 1, 1, 1); break;  // top-left
    case 6: drawRect(x, y + length, length, thickness, 1, 1, 1); break;              // center (redraw top to simplify)
    }
}

void drawDigit(float x, float y, float scale, int num) {
    static const int segments[10][7] = {
        {1,1,1,1,1,1,0},  // 0
        {0,1,1,0,0,0,0},  // 1
        {1,1,0,1,1,0,1},  // 2
        {1,1,1,1,0,0,1},  // 3
        {0,1,1,0,0,1,1},  // 4
        {1,0,1,1,0,1,1},  // 5
        {1,0,1,1,1,1,1},  // 6
        {1,1,1,0,0,0,0},  // 7
        {1,1,1,1,1,1,1},  // 8
        {1,1,1,1,0,1,1}   // 9
    };
    for (int i = 0; i < 7; i++) {
        if (segments[num][i]) drawSegment(x, y, scale, i);
    }
}

void drawColon(float x, float y, float scale) {
    float size = scale * 0.2f;
    drawRect(x, y + scale * 0.8f, size, size, 1, 1, 1);
    drawRect(x, y + scale * 0.2f, size, size, 1, 1, 1);
}

void drawDigitalClock(int hour, int min, int sec, float startX, float startY, float scale) {
    int digits[] = {
        hour / 10, hour % 10,
        min / 10, min % 10,
        sec / 10, sec % 10
    };

    for (int i = 0; i < 6; i++) {
        float x = startX + i * (scale + 10);
        if (i == 2 || i == 4) {
            drawColon(x, startY, scale);
            x += scale * 0.4f;
        }
        drawDigit(x, startY, scale, digits[i]);
    }
}

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

        // 검은색 직사각형
        float rectX = CLOCK_CX - 75;
        float rectY = CLOCK_CY - 80 - 25;
        drawRect(rectX, rectY, 150, 50, 0, 0, 0);

        // 디지털 시계 출력
        drawDigitalClock(t->tm_hour, t->tm_min, t->tm_sec, rectX + 5, rectY + 5, 10.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
