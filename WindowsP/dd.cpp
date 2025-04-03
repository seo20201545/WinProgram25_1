#include <stdio.h>
#include <string.h>

typedef struct {
    int x, y;
    int width, height;
    int colorCode;
    const char* name;
} WindowBox;

void drawBox(WindowBox box) {
    printf("\033[%dm", box.colorCode);  // 배경 색상 설정

    // ┌───┐ 상단 테두리
    printf("\033[%d;%dH┌", box.y, box.x);
    for (int i = 0; i < box.width - 2; i++) printf("─");
    printf("┐");
    printf("\033[%d;%dH ", box.y, box.x + box.width);

    // 제목 영역 (3줄)
    for (int i = 1; i <= 3; i++) {
        printf("\033[%d;%dH│", box.y + i, box.x);  // 왼쪽 테두리
        if (i == 1) {
            // 가운데 줄에 제목 출력 (가운데 정렬)
            int nameLen = strlen(box.name);
            int titleX = box.x + (box.width - nameLen) / 2;
            printf("\033[%d;%dH%s", box.y + i, titleX, box.name);
        }
        else {
            // 빈 줄 출력
            printf("\033[%d;%dH", box.y + i, box.x + 1);
            for (int j = 0; j < box.width - 2; j++) printf(" ");
        }
        printf("\033[%d;%dH│", box.y + i, box.x + box.width - 1);  // 오른쪽 테두리
        printf("\033[%d;%dH ", box.y + i, box.x + box.width);
    }

    // 내용 영역 (4줄)
    for (int i = 4; i <= 6; i++) {
        printf("\033[%d;%dH│", box.y + i, box.x);
        printf("\033[%d;%dH", box.y + i, box.x + 1);
        for (int j = 0; j < box.width - 2; j++) printf(" ");
        printf("\033[%d;%dH│", box.y + i, box.x + box.width - 1);
        printf("\033[%d;%dH ", box.y + i, box.x + box.width);
    }

    // 하단 테두리
    printf("\033[%d;%dH└", box.y + box.height - 1, box.x);
    for (int i = 0; i < box.width - 2; i++) printf("─");
    printf("┘");
    printf("\033[%d;%dH ", box.y + box.height - 1, box.x + box.width);

    // 초기화
    printf("\033[0m");
}

int main() {
    int width = 100;
    int height = 30;

    // 전체 배경 (초록색)
    for (int i = 0; i < height; i++) {
        printf("\033[42m");
        for (int j = 0; j < width; j++) {
            printf(" ");
        }
        printf("\033[0m\n");
    }

    // 박스 정의 (20x7 고정)
    WindowBox blueBox = { 10, 3, 20, 7, 44, "      파란창      " };
    WindowBox redBox = { 15, 5, 20, 7, 41, "      빨간창      " };

    // 박스 출력
    drawBox(blueBox);
    drawBox(redBox);

    // 디버깅 메시지 아래로 유도
    printf("\033[%d;%dH", height - 1, 1);
    printf("\033[0m\n");

    return 0;
}