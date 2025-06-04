#include <GLFW/glfw3.h>
#include <math.h>

// -----------------------------
// 화면 설정
// -----------------------------
#define WINDOW_WIDTH 800               // 창 가로 크기
#define WINDOW_HEIGHT 800              // 창 세로 크기
#define NUM_SEGMENTS 360               // 원을 구성할 삼각형 분할 수
#define PI 3.1415926535f               // 원주율

// -----------------------------
// 회전 각도 상태값 (시간에 따라 누적됨)
// -----------------------------
float blueOrbitAngle = 0.0f;           // 파란 원의 공전 각도
float blueSelfAngle = 0.0f;           // 파란 원의 자전 각도
float grayOrbitAngle = 0.0f;           // 회색 원의 공전 각도
float graySelfAngle = 0.0f;           // 회색 원의 자전 각도

// -----------------------------
// 회전 속도 설정
// -----------------------------
float orbitSpeed = 0.02f;              // 공전 속도
float selfSpeed = 0.1f;               // 자전 속도

// -----------------------------
// 시간 진행 방향 (-1: 감소, 0: 정지, 1: 증가)
// -----------------------------
int timeDirection = 0;

// -----------------------------
// 일반 원 그리기 함수
// cx, cy: 중심 좌표
// radius: 반지름
// r, g, b: 색상 (0~1)
// -----------------------------
void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);                // 색상 설정
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);                // 중심점

    // 원 둘레 점들 계산
    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float angle = 2.0f * PI * i / NUM_SEGMENTS;
        float x = cx + cosf(angle) * radius;
        float y = cy + sinf(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}

// -----------------------------
// 키보드 입력 콜백 함수
// →: 시간 진행 (정방향)
// ←: 시간 감소 (역방향)
// -----------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_RIGHT) {
            timeDirection = 1; // 시간 증가
        }
        else if (key == GLFW_KEY_LEFT) {
            timeDirection = -1; // 시간 감소
        }
    }
}

int main() {
    if (!glfwInit())
        return -1;

    // GLFW 창 생성
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Orbit & Spin Control", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback); // 키 콜백 등록

    // 좌표계 설정: 정규화 좌표계 (-1.0 ~ 1.0)
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);  // 2D 평면 좌표계
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // 배경 지우기

        // ============================================
        // 1. 노란색 원: 기준 중심 원 (고정)
        // ============================================
        drawCircle(0.0f, 0.0f, 0.2f, 1.0f, 1.0f, 0.0f); // 중심: (0,0), 반지름: 0.2

        // ============================================
        // 2. 파란색 원
        // - 공전 반지름: 0.6 (노란 원 기준)
        // - 자전 반지름: 0.1
        // - 공전 방향: 시계
        // - 자전 방향: 반시계
        // ============================================
        float blueOrbitRadius = 0.6f;
        float blueSelfRadius = 0.1f;

        // 공전 위치 계산 (시계방향이므로 -각도)
        float blueCenterX = cosf(-blueOrbitAngle) * blueOrbitRadius;
        float blueCenterY = sinf(-blueOrbitAngle) * blueOrbitRadius;

        glPushMatrix();
        glTranslatef(blueCenterX, blueCenterY, 0.0f);                      // 공전 위치로 이동
        glRotatef(+blueSelfAngle * 180.0f / PI, 0.0f, 0.0f, 1.0f);         // 자전 (반시계)
        drawCircle(0.0f, 0.0f, blueSelfRadius, 0.0f, 0.0f, 1.0f);          // 파란색 원
        glPopMatrix();

        // ============================================
        // 3. 회색 원
        // - 공전 반지름: 0.2 (파란 원 기준)
        // - 자전 반지름: 0.05
        // - 공전 방향: 시계
        // - 자전 방향: 반시계
        // ============================================
        float grayOrbitRadius = 0.2f;
        float graySelfRadius = 0.05f;

        // 회색 원의 파란 원 기준 위치
        float grayOffsetX = cosf(-grayOrbitAngle) * grayOrbitRadius;
        float grayOffsetY = sinf(-grayOrbitAngle) * grayOrbitRadius;

        glPushMatrix();
        glTranslatef(blueCenterX + grayOffsetX, blueCenterY + grayOffsetY, 0.0f); // 공전 위치
        glRotatef(+graySelfAngle * 180.0f / PI, 0.0f, 0.0f, 1.0f);                // 자전 (반시계)
        drawCircle(0.0f, 0.0f, graySelfRadius, 0.5f, 0.5f, 0.5f);                 // 회색 원
        glPopMatrix();

        // ============================================
        // 4. 시간에 따른 각도 누적
        // ←, → 키 입력 시만 증가/감소
        // ============================================
        if (timeDirection != 0) {
            blueOrbitAngle += orbitSpeed * timeDirection;
            blueSelfAngle += selfSpeed * timeDirection;

            grayOrbitAngle += orbitSpeed * 2.0f * timeDirection; // 더 빠른 공전
            graySelfAngle += selfSpeed * 2.0f * timeDirection; // 더 빠른 자전
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 종료 처리
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
