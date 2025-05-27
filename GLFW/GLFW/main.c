#include <GLFW/glfw3.h>
#include <math.h>

#define WIDTH 1000
#define HEIGHT 1000

// �� �׸��� �Լ�
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

// �簢�� (��, ��) �׸���
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x - w / 2, y - h / 2);
    glVertex2f(x + w / 2, y - h / 2);
    glVertex2f(x + w / 2, y + h / 2);
    glVertex2f(x - w / 2, y + h / 2);
    glEnd();
}

// ������ �ϳ� �׸���
void drawSmiley(float cx, float cy) {
    // �� (��� ��)
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(cx, cy, 200, 100);

    // �� (���� �簢��)
    glColor3f(0.0f, 0.0f, 0.0f);
    drawRect(cx - 60, cy + 80, 20, 80); // ���� ��
    drawRect(cx + 60, cy + 80, 20, 80); // ������ ��

    // �� (���� �ݿ�)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy - 60); // �߽�
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
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1); // 2D ��ǥ�� ����

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float centerX = WIDTH / 2.0f;
        float centerY = HEIGHT / 2.0f;
        float radius = 300.0f;

        float x = centerX + radius * cosf(angle);
        float y = centerY + radius * sinf(angle);

        drawSmiley(x, y);

        angle -= 0.01f; // �ð� ���� ȸ��

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
