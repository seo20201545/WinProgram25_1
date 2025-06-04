#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h> // bool 타입 사용을 위한 헤더

// -----------------------------
// 창 크기 및 원 표현 설정
// -----------------------------
#define WINDOW_WIDTH 800             // 창 가로 픽셀 수
#define WINDOW_HEIGHT 800            // 창 세로 픽셀 수
#define NUM_SEGMENTS 360             // 원 둘레의 삼각형 분할 수 (클수록 원에 가까움)
#define PI 3.1415926535f             // 원주율

// -----------------------------
// 회전 각도 상태 변수 (시간에 따라 누적)
// -----------------------------
float blueOrbitAngle = 0.0f;         // 파란 원의 공전 각도
float blueSelfAngle = 0.0f;          // 파란 원의 자전 각도
float grayOrbitAngle = 0.0f;         // 회색 원의 공전 각도
float graySelfAngle = 0.0f;          // 회색 원의 자전 각도

// -----------------------------
// 회전 속도 (한 프레임당 각도 증가량)
// -----------------------------
float orbitSpeed = 0.02f;            // 공전 속도 (rad/frame)
float selfSpeed = 0.1f;              // 자전 속도 (rad/frame)

// -----------------------------
// 시간 방향 제어 변수
// -1: 시간 감소 (역방향)
//  0: 정지
// +1: 시간 증가 (정방향)
// -----------------------------
int timeDirection = 0;

// -----------------------------
// 오른쪽 눈을 '<' 형태로 그릴지 여부
// -----------------------------
bool isRightEyeAngled = false;

// -----------------------------
// 원 그리기 함수 정의
// cx, cy: 중심 좌표 / radius: 반지름 / r,g,b: 색상 값
// -----------------------------
void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float angle = 2.0f * PI * i / NUM_SEGMENTS;
        float x = cx + cosf(angle) * radius;
        float y = cy + sinf(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}

// -----------------------------
// 삼각형 그리기 함수 정의 (정삼각형)
// cx, cy: 중심 좌표 / size: 한 변 길이 / r,g,b: 색상 값
// -----------------------------
void drawTriangle(float cx, float cy, float size, float r, float g, float b) {
    float height = size * sqrtf(3.0f) / 2.0f; // 정삼각형 높이
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(cx, cy + 2.0f * height / 3.0f); // 꼭대기 점
    glVertex2f(cx - size / 2.0f, cy - height / 3.0f); // 좌측 하단
    glVertex2f(cx + size / 2.0f, cy - height / 3.0f); // 우측 하단
    glEnd();
}

// -----------------------------
// 별 그리기 함수 정의 (5각 별)
// cx, cy: 중심 좌표 / radius: 외곽 반지름 / r,g,b: 색상 값
// -----------------------------
void drawStar(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // 중심점
    for (int i = 0; i <= 10; ++i) {
        float angle = PI * i / 5.0f; // 36도 간격
        float len = (i % 2 == 0) ? radius : radius * 0.5f; // 외곽/내부 길이 번갈아 설정
        float x = cx + cosf(angle) * len;
        float y = cy + sinf(angle) * len;
        glVertex2f(x, y);
    }
    glEnd();
}

// -----------------------------
// 사각형 그리기 함수 정의 (정사각형)
// cx, cy: 중심 좌표 / size: 변 길이 / r,g,b: 색상 값
// -----------------------------
void drawRectangle(float cx, float cy, float size, float r, float g, float b) {
    float half = size / 2.0f; // 중심에서 각 방향의 절반 길이
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(cx - half, cy - half); // 좌하단
    glVertex2f(cx + half, cy - half); // 우하단
    glVertex2f(cx + half, cy + half); // 우상단
    glVertex2f(cx - half, cy + half); // 좌상단
    glEnd();
}

void drawEyes(float cx, float cy, float faceRadius) {
    float eyeOffsetX = faceRadius * 0.4f;
    float eyeTopY = cy + faceRadius * 0.4f;
    float eyeHeight = faceRadius * 0.3f;
    float eyeBottomY = eyeTopY - eyeHeight;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    // 왼쪽 눈 (항상 직선)
    glVertex2f(cx - eyeOffsetX, eyeTopY);
    glVertex2f(cx - eyeOffsetX, eyeBottomY);
    // 오른쪽 눈
    if (!isRightEyeAngled) {    // 일반 직선 눈
        glVertex2f(cx + eyeOffsetX, eyeTopY);
        glVertex2f(cx + eyeOffsetX, eyeBottomY);
    }
    else {  // '<' 모양 눈
        float offsetX = faceRadius * 0.06f; // 가로 기울기 크기 증가
        float centerY = (eyeTopY + eyeBottomY) / 2.0f;
        glVertex2f(cx + eyeOffsetX + offsetX, eyeTopY);
        glVertex2f(cx + eyeOffsetX, centerY);
        glVertex2f(cx + eyeOffsetX, centerY);
        glVertex2f(cx + eyeOffsetX + offsetX, eyeBottomY);
    }
    glEnd();
}
// --------------------- 웃는 입 그리기 ---------------------
void drawSmile(float cx, float cy, float radius) {
    float smileRadius = radius * 0.5f;          // 입 반지름 (얼굴 반지름의 절반)
    float smileCenterY = cy - radius * 0.3f;    // 입 중심 Y 좌표 (얼굴 중심보다 아래)

    glColor3f(0.0f, 0.0f, 0.0f);    // 검은색
    glBegin(GL_LINE_STRIP);         // 선 연결로 반원 구성

    for (int i = 0; i <= NUM_SEGMENTS / 2; ++i) {
        float angle = PI * i / (NUM_SEGMENTS / 2);  // 0~π
        float x = cx + cosf(angle) * smileRadius;
        float y = smileCenterY - sinf(angle) * smileRadius; // 아래 방향으로 반원
        glVertex2f(x, y);
    }
    glEnd();
}

// -----------------------------
// 키 입력 처리 콜백 함수 정의
// - 좌우 방향키로 시간 흐름 제어
// - W 키로 오른쪽 눈 '<' 형태 토글
// -----------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_RIGHT)
            timeDirection = 1;   // 오른쪽 방향키 누르면 시간 증가
        else if (key == GLFW_KEY_LEFT)
            timeDirection = -1;  // 왼쪽 방향키 누르면 시간 감소
        else if (key == GLFW_KEY_W)
            isRightEyeAngled = !isRightEyeAngled; // W 입력 시 오른 쪽 눈 모양 윙크
    }
}

int main() {
    if (!glfwInit()) return -1;

    // GLFW 창 생성
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Smiley Orbit System", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // 좌표계 설정: 정규화 좌표계 (-1.0 ~ 1.0), 화면 크기 바뀌어도 정비율 유지
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // 2D 평면 좌표계
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // 배경 지우기


        // ============================================
        // 1. 노란색 원: 기준 중심 원 (고정)
        // ============================================
        float faceX = 0.0f; //가로 좌표
        float faceY = 0.0f; //세로 좌표
        float faceRadius = 0.2f; //반지름

        drawCircle(faceX, faceY, faceRadius, 1.0f, 1.0f, 0.0f);
        drawEyes(faceX, faceY, faceRadius);
        drawSmile(faceX, faceY, faceRadius);

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
        float blueX = cosf(-blueOrbitAngle) * blueOrbitRadius;
        float blueY = sinf(-blueOrbitAngle) * blueOrbitRadius;

        glPushMatrix();
        glTranslatef(blueX, blueY, 0.0f); // 파란 원 공전 위치
        glRotatef(+blueSelfAngle * 180.0f / PI, 0.0f, 0.0f, 1.0f); // 자전 (반시계)
        drawCircle(0.0f, 0.0f, blueSelfRadius, 0.0f, 0.0f, 1.0f);  //파란색 원
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
        glTranslatef(blueX + grayOffsetX, blueY + grayOffsetY, 0.0f); // 회색 원 공전 위치
        glRotatef(+graySelfAngle * 180.0f / PI, 0.0f, 0.0f, 1.0f);    // 자전
        drawCircle(0.0f, 0.0f, graySelfRadius, 0.5f, 0.5f, 0.5f);     //회색 원
        glPopMatrix();


        // ============================================
        // 4. 시간에 따른 각도 누적
        // ←, → 키 입력 시만 증가/감소
        // ============================================
        if (timeDirection != 0) {
            blueOrbitAngle += orbitSpeed * timeDirection;
            blueSelfAngle += selfSpeed * timeDirection;
            grayOrbitAngle += orbitSpeed * 2.0f * timeDirection; //공전 속도
            graySelfAngle += selfSpeed * 2.0f * timeDirection;   //자전 속도
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //종료 처리
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
