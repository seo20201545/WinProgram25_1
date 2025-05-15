#pragma comment(lib, "Opengl32.lib")
#include <GLFW/glfw3.h>
#include <math.h>

#define PI 3.14159265358979323846

// 타원 그리기
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

// 테두리 타원 그리기
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

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(537, 445, "보노보노 얼굴 - 왼쪽 눈까지", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 픽셀 단위 좌표 설정 (좌하단 0,0 ~ 우상단 537,445)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 537, 0, 445, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // 중심 좌표
        float centerX = 537 / 2.0f;
        float centerY = 445 / 2.0f;

        // 얼굴 (파란 원 + 아웃라인)
        float ellipseW = 430.0f;
        float ellipseH = 385.0f;

        // 1. 검은 아웃라인
        drawEllipseOutline(centerX, centerY, ellipseW / 2.0f, ellipseH / 2.0f, 100, 0, 0, 0, 2.0f);

        // 2. 파란 원 내부 (조금 작게)
        drawEllipse(centerX, centerY, (ellipseW - 4) / 2.0f, (ellipseH - 4) / 2.0f, 100,
            0.0f, 112.0f / 255.0f, 192.0f / 255.0f);  // #0070C0

        // 3. 왼쪽 눈 (검정 타원)
        drawEllipse(89.0f, 177.5f, 16.0f, 15.5f, 100, 0.0f, 0.0f, 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
