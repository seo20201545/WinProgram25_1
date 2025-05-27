#include <GLFW/glfw3.h>
#include <math.h>

#define WIDTH 1000
#define HEIGHT 1000

// 원 그리기 함수
void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * 3.1415926f * i / segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// 사각형 (눈, 입) 그리기
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x - w / 2, y - h / 2);
    glVertex2f(x + w / 2, y - h / 2);
    glVertex2f(x + w / 2, y + h / 2);
    glVertex2f(x - w / 2, y + h / 2);
    glEnd();
}

// 스마일 하나 그리기
void drawSmiley(float cx, float cy) {
    // 얼굴 (노란 원)
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(cx, cy, 200, 100);

    // 눈 (검은 사각형)
    glColor3f(0.0f, 0.0f, 0.0f);
    drawRect(cx - 60, cy + 80, 20, 80); // 왼쪽 눈
    drawRect(cx + 60, cy + 80, 20, 80); // 오른쪽 눈

    // 입 (검은 반원)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy - 60); // 중심
    for (int i = 0; i <= 50; i++) {
        float theta = 3.1415926f * i / 50;
        float x = 100 * cosf(theta);
        float y = 60 * sinf(theta);
        glVertex2f(cx + x, cy - 60 + y);
    }
    glEnd();
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rotating Smiley", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1); // 2D 좌표계 설정

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float centerX = WIDTH / 2.0f;
        float centerY = HEIGHT / 2.0f;
        float radius = 300.0f;

        float x = centerX + radius * cosf(angle);
        float y = centerY + radius * sinf(angle);

        drawSmiley(x, y);

        angle -= 0.01f; // 시계 방향 회전

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
