#include <stdio.h>
#include <string.h>

typedef struct
{
    int x, y;
    int width, height;
    int colorCode;
    const char* name;
} WindowBox;

void drawBox(WindowBox box)
{
    printf("\033[%dm", box.colorCode);

    printf("\033[%d;%dH┌", box.y, box.x);
    for (int i = 0; i < box.width - 2; i++) printf("─");
    printf("┐");
    printf("\033[%d;%dH ", box.y, box.x + box.width);

    for (int i = 1; i <= 3; i++)
    {
        printf("\033[%d;%dH│", box.y + i, box.x);
        if (i == 1)
        {
            int nameLen = strlen(box.name);
            int titleX = box.x + (box.width - nameLen) / 2;
            printf("\033[%d;%dH%s", box.y + i, titleX, box.name);
        }
        else if (i == 2)
        {
            printf("\033[%d;%dH", box.y + i, box.x + 1);
            for (int j = 0; j < box.width - 2; j++) printf("─");
        }
        else
        {
            printf("\033[%d;%dH", box.y + i, box.x + 1);
            for (int j = 0; j < box.width - 2; j++) printf(" ");
        }

        printf("\033[%d;%dH│", box.y + i, box.x + box.width - 1);
        printf("\033[%d;%dH ", box.y + i, box.x + box.width);
    }

    for (int i = 4; i <= 6; i++)
    {
        printf("\033[%d;%dH│", box.y + i, box.x);
        printf("\033[%d;%dH", box.y + i, box.x + 1);
        for (int j = 0; j < box.width - 2; j++) printf(" ");
        printf("\033[%d;%dH│", box.y + i, box.x + box.width - 1);
        printf("\033[%d;%dH ", box.y + i, box.x + box.width);
    }

    printf("\033[%d;%dH└", box.y + box.height - 1, box.x);
    for (int i = 0; i < box.width - 2; i++) printf("─");
    printf("┘");
    printf("\033[%d;%dH ", box.y + box.height - 1, box.x + box.width);

    printf("\033[0m");
}

int main()
{
    int width = 100;
    int height = 30;

    for (int i = 0; i < height; i++)
    {
        printf("\033[42m");
        for (int j = 0; j < width; j++)
        {
            printf(" ");
        }
        printf("\033[0m\n");
    }

    WindowBox blueBox = { 10, 3, 20, 7, 44, "      고등어      " };
    WindowBox redBox = { 15, 5, 20, 7, 41, "       연어       " };

    drawBox(blueBox);
    drawBox(redBox);

    printf("\033[%d;%dH", height - 1, 1);
    printf("\033[0m\n");

    return 0;
}