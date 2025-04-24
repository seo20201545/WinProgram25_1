#include <stdio.h>
#include <windows.h>
#include <conio.h>

// ANSI ���� �ڵ� �� �ܼ� ���� ����
WORD getColorAttribute(int color) {
    switch (color) {
    case 0: return 0;
    case 1: return BACKGROUND_BLUE;
    case 2: return BACKGROUND_GREEN;
    case 3: return BACKGROUND_GREEN | BACKGROUND_BLUE;
    case 4: return BACKGROUND_RED;
    case 5: return BACKGROUND_RED | BACKGROUND_BLUE;
    case 6: return BACKGROUND_RED | BACKGROUND_GREEN; // ���� (brown)
    case 7: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
    default: return 0;
    }
}

// ���ȭ�� + �۾�ǥ���� ���
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

// ������ â ���
void drawConsoleWindow(int startX, int startY, int width, int height, const char* title, int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD bgColor = getColorAttribute(color);
    WORD borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | bgColor;

    // ���� ä���
    SetConsoleTextAttribute(hConsole, bgColor);
    for (int y = 0; y < height; y++) {
        COORD pos = { startX, startY + y };
        SetConsoleCursorPosition(hConsole, pos);
        for (int x = 0; x < width; x++) {
            printf(" ");
        }
    }

    // �׵θ�: ����(-), ����(|), �𼭸�(+)
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

    // �𼭸�
    SetConsoleCursorPosition(hConsole, (COORD) { startX, startY }); printf("+");
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 1, startY }); printf("+");
    SetConsoleCursorPosition(hConsole, (COORD) { startX, startY + height - 1 }); printf("+");
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 1, startY + height - 1 }); printf("+");

    // ���� �� �� �Ʒ��� ���
    SetConsoleCursorPosition(hConsole, (COORD) { startX + 1, startY + 1 });
    SetConsoleTextAttribute(hConsole, borderColor);
    printf("%s", title);

    // X ��ư (�� �� �Ʒ��� ��ġ �̵�)
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 4, startY + 1 });
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("X");

    // X ������ �� ĭ ä��� (���� �����ϰ�)
    SetConsoleCursorPosition(hConsole, (COORD) { startX + width - 3, startY + 1 });
    SetConsoleTextAttribute(hConsole, bgColor);
    printf(" ");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main() {
    int width = 80;
    int height = 25;

    system("cls");

    // ���ȭ�� ���
    DrawBG(width, height);

    // ��� ������ (����)
    drawConsoleWindow(15, 4, 50, 15, "���� ���� ������", 6);

    // �ʷ� ������
    drawConsoleWindow(10, 10, 25, 10, "�ʷ� ������", 2);

    // ���� ������
    drawConsoleWindow(30, 6, 30, 12, "���� ������", 5);

    // ����� �޽����� �Ʒ��ʿ� �������� Ŀ�� ���� �̵�
    COORD cursor;
    cursor.X = 0;
    cursor.Y = height;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

    return 0;
}
