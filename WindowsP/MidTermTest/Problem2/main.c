#include <stdio.h>
#include <windows.h>

// 바탕화면과 작업표시줄을 그리는 함수
void DrawBG(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 파란색 바탕화면 출력 (상하 1줄, 좌우 2칸 여백 제외)
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
    for (int y = 1; y < height - 1; y++) {
        COORD pos = { 2, y }; // 좌측 여백 2칸
        SetConsoleCursorPosition(hConsole, pos);
        for (int x = 0; x < width - 4; x++) { // 좌우 여백 제외한 너비
            printf(" ");
        }
    }

    // 회색 작업 표시줄 출력 (하단 1줄, 여백 동일 적용)
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    COORD barPos = { 2, height - 1 }; // 좌측 여백 2칸, 하단 마지막 줄
    SetConsoleCursorPosition(hConsole, barPos);
    for (int x = 0; x < width - 4; x++) {
        printf(" ");
    }

    // 출력 후 색상 원상복구 (흰 글자, 검은 배경)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main() {
    int width = 80;
    int height = 25;

    DrawBG(width, height);

    return 0;
}