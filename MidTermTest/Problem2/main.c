#include <stdio.h>
#include <windows.h>
#include <conio.h>

// ANSI 색상 코드 → 콘솔 색상 매핑
WORD getColorAttribute(int color) {
    switch (color) {
    case 0: return 0;
    case 1: return BACKGROUND_BLUE;
    case 2: return BACKGROUND_GREEN;
    case 3: return BACKGROUND_GREEN | BACKGROUND_BLUE;
    case 4: return BACKGROUND_RED;
    case 5: return BACKGROUND_RED | BACKGROUND_BLUE;
    case 6: return BACKGROUND_RED | BACKGROUND_GREEN; // 갈색 (brown)
    case 7: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
    default: return 0;
    }
}

// 배경화면 + 작업표시줄 출력
void DrawBG(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
    for (int y = 1; y < height - 1; y++) {
        COORD pos = { 2, y };
        SetConsoleCursorPosition(hConsole, pos);
        for (int x = 0; x < width - 4; x++) {
            printf(" ");
        }
    }

    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    COORD pos = { 2, height - 1 };
    SetConsoleCursorPosition(hConsole, pos);
    for (int x = 0; x < width - 4; x++) {
        printf(" ");
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// 윈도우 창 출력
void drawConsoleWindow(int startX, int startY, int width, int height, const char* title, int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD bgColor = getColorAttribute(color);
    WORD borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | bgColor;

    // 내부 채우기
    SetConsoleTextAttribute(hConsole, bgColor);
    for (int y = 0; y < height; y++) {
        COORD pos = { startX, startY + y };
        SetConsoleCursorPosition(hConsole, pos);
        for (int x = 0; x < width; x++) {
            printf(" ");
        }
    }

    // 테두리: 가로(-), 세로(|), 모서리(+)
    SetConsoleTextAttribute(hConsole, borderColor);
    for (int x = 0; x < width; x++) {
        SetConsoleCursorPosition(hConsole, (COORD) { startX + x, startY });
        printf("-");
        SetConsoleCursorPosition(hConsole, (COORD) { startX + x, startY + height - 1 });
        printf("-");
    }
    for (int y = 1; y < height - 1; y++) {
        SetConsoleCursorPosition(hConsole, (COORD) { startX, startY + y });
        printf("|");
        SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 1, startY + y });
        printf("|");
    }

    // 모서리
    SetConsoleCursorPosition(hConsole, (COORD) { startX, startY }); printf("+");
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 1, startY }); printf("+");
    SetConsoleCursorPosition(hConsole, (COORD) { startX, startY + height - 1 }); printf("+");
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 1, startY + height - 1 }); printf("+");

    // 제목 한 줄 아래로 출력
    SetConsoleCursorPosition(hConsole, (COORD) { startX + 1, startY + 1 });
    SetConsoleTextAttribute(hConsole, borderColor);
    printf("%s", title);

    // X 버튼 (한 줄 아래로 위치 이동)
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 4, startY + 1 });
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("X");

    // X 오른쪽 한 칸 채우기 (배경색 동일하게)
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 3, startY + 1 });
    SetConsoleTextAttribute(hConsole, bgColor);
    printf(" ");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main() {
    int width = 80;
    int height = 25;

    system("cls");

    // 배경화면 출력
    DrawBG(width, height);

    // 노란 윈도우 (갈색)
    drawConsoleWindow(15, 4, 50, 15, "나의 멋진 윈도우", 6);

    // 초록 윈도우
    drawConsoleWindow(10, 10, 25, 10, "초록 윈도우", 2);

    // 보라 윈도우
    drawConsoleWindow(30, 6, 30, 12, "보라 윈도우", 5);

    // 디버깅 메시지가 아래쪽에 나오도록 커서 수동 이동
    COORD cursor;
    cursor.X = 0;
    cursor.Y = height;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

    return 0;
}
