#include <GLFW/glfw3.h>
#include <math.h>

// 창 크기 설정 (픽셀 단위)
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// 도형 분할 수 (원/입 등 곡선 표현 정밀도)
#define NUM_SEGMENTS 360

// 파이 상수
#define PI 3.1415926535f

// 오른쪽 눈 상태: 0 = 세로선, 1 = '<' 모양
int rightEyeIsArrow = 0;

// --------------------- 원(얼굴) 그리기 ---------------------
void drawCircle(float cx, float cy, float radius) {
    glColor3f(1.0f, 1.0f, 0.0f); // 노란색
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // 중심점

    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float angle = 2.0f * PI * i / NUM_SEGMENTS;
        float x = cx + cosf(angle) * radius;
        float y = cy + sinf(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}

// --------------------- 눈 그리기 ---------------------
void drawEyes(float cx, float cy, float faceRadius) {
    float eyeOffsetX = faceRadius * 0.4f;      // 얼굴 중심 기준 눈 좌우 위치
    float eyeTopY = cy + faceRadius * 0.4f;    // 눈 윗부분 Y 좌표
    float eyeHeight = faceRadius * 0.3f;      // 눈 길이 (수직 방향)
    float eyeBottomY = eyeTopY - eyeHeight;    // 눈 아랫부분 Y 좌표
    float eyeCenterY = (eyeTopY + eyeBottomY) / 2.0f; // 눈 중심 Y 좌표

    float arrowWidth = 5.0f; // '<' 모양 눈 가로폭

    glColor3f(0.0f, 0.0f, 0.0f); // 검은색
    glBegin(GL_LINES);

    // 왼쪽 눈 (세로선)
    glVertex2f(cx - eyeOffsetX, eyeTopY);
    glVertex2f(cx - eyeOffsetX, eyeBottomY);

    // 오른쪽 눈
    if (rightEyeIsArrow) {
        // 오른쪽 눈을 '<' 모양으로 그림 (↙ ↗)
        glVertex2f(cx + eyeOffsetX + arrowWidth, eyeTopY);      // 대각선 왼아래로
        glVertex2f(cx + eyeOffsetX - arrowWidth, eyeCenterY);

        glVertex2f(cx + eyeOffsetX - arrowWidth, eyeCenterY);   // 대각선 왼위로
        glVertex2f(cx + eyeOffsetX + arrowWidth, eyeBottomY);
    }
    else {
        // 기본: 오른쪽 눈도 세로선
        glVertex2f(cx + eyeOffsetX, eyeTopY);
        glVertex2f(cx + eyeOffsetX, eyeBottomY);
    }

    glEnd();
}

// --------------------- 웃는 입 그리기 ---------------------
void drawSmile(float cx, float cy, float radius) {
    float smileRadius = radius * 0.5f;          // 입 반지름 (얼굴 반지름의 절반)
    float smileCenterY = cy - radius * 0.3f;    // 입 중심 Y 좌표 (얼굴 중심보다 아래)

    glColor3f(0.0f, 0.0f, 0.0f); // 검은색
    glBegin(GL_LINE_STRIP);     // 선 연결로 반원 구성

    for (int i = 0; i <= NUM_SEGMENTS / 2; ++i) {
        float angle = PI * i / (NUM_SEGMENTS / 2); // 0~π
        float x = cx + cosf(angle) * smileRadius;
        float y = smileCenterY - sinf(angle) * smileRadius; // 아래 방향으로 반원
        glVertex2f(x, y);
    }
    glEnd();
}

// --------------------- 키보드 입력 처리 ---------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_W) {
            rightEyeIsArrow = 1; // W/w 키 입력 시 오른쪽 눈을 '<' 모양으로 설정
        }
    }
}

// --------------------- 메인 함수 ---------------------
int main() {
    if (!glfwInit())
        return -1;

    // 고정 크기 창 생성
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Smiley Face", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback); // 키 입력 콜백 등록

    // 픽셀 기준 좌표계 설정
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1); // 2D 좌표계
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 렌더링 루프
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // 얼굴 중심 및 크기
        float faceCenterX = 400.0f;  // X 중심
        float faceCenterY = 300.0f;  // Y 중심
        float faceRadius = 100.0f;  // 반지름

        drawCircle(faceCenterX, faceCenterY, faceRadius);   // 얼굴
        drawEyes(faceCenterX, faceCenterY, faceRadius);     // 눈
        drawSmile(faceCenterX, faceCenterY, faceRadius);    // 입

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
