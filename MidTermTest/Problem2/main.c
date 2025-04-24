#include <stdio.h>
#include <windows.h>

// ����ȭ��� �۾�ǥ������ �׸��� �Լ�
void DrawBG(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // �Ķ��� ����ȭ�� ��� (���� 1��, �¿� 2ĭ ���� ����)
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
    for (int y = 1; y < height - 1; y++) {
        COORD pos = { 2, y }; // ���� ���� 2ĭ
        SetConsoleCursorPosition(hConsole, pos);
        for (int x = 0; x < width - 4; x++) { // �¿� ���� ������ �ʺ�
            printf(" ");
        }
    }

    // ȸ�� �۾� ǥ���� ��� (�ϴ� 1��, ���� ���� ����)
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    COORD barPos = { 2, height - 1 }; // ���� ���� 2ĭ, �ϴ� ������ ��
    SetConsoleCursorPosition(hConsole, barPos);
    for (int x = 0; x < width - 4; x++) {
        printf(" ");
    }

    // ��� �� ���� ���󺹱� (�� ����, ���� ���)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main() {
    int width = 80;
    int height = 25;

    DrawBG(width, height);

    return 0;
}