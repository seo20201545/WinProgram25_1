#include <stdio.h>

// ���� �Ǻ� �Լ�
int PassOrFail(int score) {
    if (score >= 50) {
        return 1;
    }
    else {
        return 0;
    }
}

int main() {
    int expectedAverage = 40;   // ������ ���� ��� (����� �����)
    int myScore;                // ������ �Է��� ���� ����

    printf("������ ���� ������ �Է��ϼ���: ");
    scanf_s("%d", &myScore);

    int result = PassOrFail(myScore);

    if (result == 1) {
        printf("����� �����ϴ�!\n");
    }
    else {
        printf("�츮�� ���ߴ١� ������̴١�\n");
    }

    return 0;
}