#include <iostream>
#include <string>
#include <fstream>

#define MAX_TERMS 100

using namespace std;

typedef struct poly_node *poly_pointer;
typedef struct poly_node {
    float coef;
    int expon;
    poly_pointer link;
};
poly_pointer A, B, C;

// filename에서 polynomial을 읽어 저장하는 함수
// O(n)
void readPoly(string filename, poly_pointer& poly) {
	ifstream fin(filename);

	float a;
	int b = 0;

    poly_pointer prev = NULL;

	// end of line 까지 입력
	while (!fin.eof()) {
		fin >> a >> b;
        
        poly_pointer node = new poly_node;
		node->coef = a;
		node->expon = b;
        node->link = NULL;

        if(prev) {
            prev->link = node;
        } else {
            poly = node;
        }

        prev = node;
	}

	fin.close();
}

// polynomial을 출력하는 함수
// O(n)
void printPoly(poly_pointer poly) {

	// 배열을 순회하면서 출력
    poly_pointer cur = poly;

    while(cur) {

        cout << cur->coef << "x^" << cur->expon;

        if(cur->link) {
            cout << " + ";
        }

        cur = cur->link;

    }
}

/**
 * polynomial A(n)의 모든 항에 대하여
 * polynomial B(m)의 모든 항을 곱하고
 * polynomial C에 이를 저장하는 함수
 *
 * 최악의 경우(곱한 모든 항들의 지수가 다름) 1, 2, ... , n*m번 순차 탐색
 * 따라서 시간 복잡도는 O(1 + 2 + ... + nm)임
 *
 * O(nm*(nm+1)/2)
 * = O(nm^2)
 * = O((nm)^2)
 *
 * 최종 시간 복잡도(Worst Case)
 * O((nm)^2)
 *
 */
void pmult(poly_pointer A, poly_pointer B, poly_pointer& C) {

	float a;
	int b;

    poly_pointer curA = A;
    poly_pointer curB = NULL;
    poly_pointer curC = NULL;

    // n번 반복
    while(curA) {

        curB = B;

        // m번 반복
        while(curB) {

            a = curA->coef * curB->coef;
            b = curA->expon + curB->expon;

            curC = C;

            int flag = 0;

            // 순차 탐색: 1, 2, ..., n*m
            while(curC) {

                if(curC->expon == b) {

                    curC->coef += a;
                    flag = 1;

                    break;

                }

                if(curC->link) {
                    curC = curC->link;
                } else {
                    break;
                }

            }

            // 일치하는 지수가 없으면 맨 뒤에 추가
            if(!flag) {

                poly_pointer node = new poly_node;
                node->coef = a;
                node->expon = b;
                node->link = NULL;

                if(curC) {
                    curC->link = node;
                } else {
                    C = node;
                }

            }

            curB = curB->link;

        }

        curA = curA->link;

    }

}

// polynomial을 지수 내림차순으로 정렬하는 함수 (bubble sort)
// O(n^2)
void sortPoly(poly_pointer poly) {
	float ct;
    int et;
	// bubble sort 구현
    poly_pointer cur1 = poly;
    poly_pointer cur2 = NULL;

    while(cur1) {

        cur2 = poly;

        while(cur2) {

            if(cur1->expon > cur2->expon) {

                et = cur1->expon;
                ct = cur1->coef;

                cur1->expon = cur2->expon;
                cur1->coef = cur2->coef;

                cur2->expon = et;
                cur2->coef = ct;

            }


            cur2 = cur2->link;

        }

        cur1 = cur1->link;

    }
}

// polynomial 동적 할당 해제 함수
// O(n)
void freePoly(poly_pointer poly) {

    poly_pointer cur = poly;
    poly_pointer next = NULL;

    while(cur) {

        next = cur->link;

        delete cur;

        cur = next;

    }

}

/**
 * A.txt의 항 개수 = n
 * B.txt의 항 개수 = m
 *
 * 최종 시간 복잡도
 * O(2n + 2m + 2nm + 2 * (nm)^2)
 * = O((nm)^2)
 *
 */

int main() {

	// O(n)
	readPoly("A.txt", A);
	// O(m)
	readPoly("B.txt", B);

	// O((nm)^2)
	pmult(A, B, C);

	// O((nm)^2)
	sortPoly(C);

	// O(nm)
	printPoly(C);

    // O(n)
    freePoly(A);

    // O(m)
    freePoly(B);

    // O(nm)
    freePoly(C);

	return 0;
}