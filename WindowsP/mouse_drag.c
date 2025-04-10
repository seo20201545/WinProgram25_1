#include <windows.h>
#include <stdio.h>

int main()
{
    char letter = 'O';
    POINT mousePos;
    int isDragging = 0;
    int letterX = 10, letterY = 5;
    int offsetX = 0, offsetY = 0;

    // �ܼ� �ʱ�ȭ
    system("cls");

    while (1)
    {
        GetCursorPos(&mousePos);

        // �ܼ� ��ǥ ���� (�ػ� �� ��Ʈ ������ ���� �� ���� ����)
        int cursorX = (mousePos.x - 8) / 8;   // ��: 2560 �ػ󵵿����� 8�� �� ��Ȯ�� ��� ����
        int cursorY = (mousePos.y - 31) / 16; // �ܼ� ��ġ ������ (��� ���� ���)

        system("cls");
        printf("���ĺ� '%c'�� ���콺�� �巡���ϼ���.\n", letter);
        printf("���� ��ġ: (%d, %d)\n", letterX, letterY);

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { letterX, letterY });
        printf("%c", letter);

        // �巡�� ���� (O ��ó �������� �� Ŭ�� ���)
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !isDragging)
        {
            if (abs(cursorX - letterX) <= 2 && abs(cursorY - letterY) <= 2)
            {
                isDragging = 1;
                offsetX = cursorX - letterX;
                offsetY = cursorY - letterY;
            }
        }

        // �巡�� ����
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && isDragging)
        {
            letterX = cursorX - offsetX;
            letterY = cursorY - offsetY;
        }

        // �巡�� ����
        if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        {
            isDragging = 0;
        }

        // ESC ����
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            printf("\n���α׷� ����\n");
            break;
        }

        Sleep(10);
    }

    return 0;
}