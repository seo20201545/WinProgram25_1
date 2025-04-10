#include <windows.h>
#include <stdio.h>

int main()
{
    char letter = 'O';
    POINT mousePos;
    int isDragging = 0;
    int letterX = 10, letterY = 5;
    int offsetX = 0, offsetY = 0;

    // 콘솔 초기화
    system("cls");

    while (1)
    {
        GetCursorPos(&mousePos);

        // 콘솔 좌표 추정 (해상도 및 폰트 설정에 따라 값 조절 가능)
        int cursorX = (mousePos.x - 8) / 8;   // 예: 2560 해상도에서는 8이 더 정확한 경우 있음
        int cursorY = (mousePos.y - 31) / 16; // 콘솔 위치 보정값 (헤더 여백 고려)

        system("cls");
        printf("알파벳 '%c'을 마우스로 드래그하세요.\n", letter);
        printf("현재 위치: (%d, %d)\n", letterX, letterY);

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { letterX, letterY });
        printf("%c", letter);

        // 드래그 시작 (O 근처 오차범위 내 클릭 허용)
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !isDragging)
        {
            if (abs(cursorX - letterX) <= 2 && abs(cursorY - letterY) <= 2)
            {
                isDragging = 1;
                offsetX = cursorX - letterX;
                offsetY = cursorY - letterY;
            }
        }

        // 드래그 진행
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && isDragging)
        {
            letterX = cursorX - offsetX;
            letterY = cursorY - offsetY;
        }

        // 드래그 종료
        if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        {
            isDragging = 0;
        }

        // ESC 종료
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            printf("\n프로그램 종료\n");
            break;
        }

        Sleep(10);
    }

    return 0;
}