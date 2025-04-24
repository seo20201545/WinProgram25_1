#include <stdio.h>

// 점수 판별 함수
int PassOrFail(int score) {
    if (score >= 50) {
        return 1;
    }
    else {
        return 0;
    }
}

int main() {
    int expectedAverage = 40;   // 수업의 예상 평균 (사용은 참고용)
    int myScore;                // 본인이 입력할 예상 점수

    printf("본인의 예상 점수를 입력하세요: ");
    scanf_s("%d", &myScore);

    int result = PassOrFail(myScore);

    if (result == 1) {
        printf("재시험 없습니다!\n");
    }
    else {
        printf("우리는 망했다… 재시험이다…\n");
    }

    return 0;
}