#include <iostream>
#include <time.h>

using namespace std;

// SWAP 함수
// a와 b를 reference로 입력받아 a와 b의 값을 서로 바꾼다.
void SWAP(char &a, char &b, int &temp) {
    temp = a;
    a = b;
    b = temp;
}

// perm 함수
// list와 시작 인덱스, 끝 인덱스를 입력받아 permutation을 수행한다.
void perm(char *list, int i, int n) {
    int j, temp;
    // 시작 인덱스와 끝 인덱스가 일치하면 정지
    if(i==n) {
        //for(j=0; j<=n; j++) cout << list[j];
        //cout << "  ";
    } else {
        // 시작 인덱스부터 끝 인덱스까지 순회하면서
        for(j=i; j<=n; j++) {
            // 시작 인덱스와 현재 인덱스의 값을 바꿈
            SWAP(list[i], list[j], temp);
            // 시작 인덱스를 1 늘리고 perm 호출
            perm(list, i+1, n);
            // 바꿨던 인덱스를 원상 복귀
            SWAP(list[i], list[j], temp);
        }
    }
}

int main() {
    // 소요 시간
    double duration;
    // 리스트
    char list[12] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    // 1~12개의 permutation 테스트
    for(int i=0;i<12;i++) {
        long repetitions = 0;
        clock_t start = clock();
        do {
            repetitions++;
            // permutation 수행
            perm(list, 0, i);
        } while(clock() - start < 1000);
        duration = (double) (clock() - start) / CLOCKS_PER_SEC;
        duration /= repetitions;

        // 반복 횟수 및 소요 시간 출력
        cout << repetitions << " " << duration << endl;
    }
    return 0;
}