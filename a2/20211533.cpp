#include <iostream>
#include <string>
#include <fstream>

#define MAX_TERMS 100

using namespace std;

typedef struct {
	float coef;
	int expon;
} polynomial;

// polynomial 초기화 함수
// O(1)
void initPoly(polynomial* poly) {
	int i;
	// 배열을 순회하면서 초기값을 0으로 설정함
	for (i = 0; i < MAX_TERMS; i++) {
		poly[i].coef = 0;
		poly[i].expon = 0;
	}
}

// filename에서 polynomial을 읽어 저장하는 함수
// O(n)
void readPoly(string filename, polynomial* poly, int* len) {
	ifstream fin(filename);

	float a;
	int b = 0, i = 0;

	// end of line 까지 입력
	while (!fin.eof()) {
		fin >> a >> b;
		poly[i].coef = a;
		poly[i].expon = b;
		i++;
	}

	*len = i;

	fin.close();
}

// polynomial을 출력하는 함수
// O(len)
void printPoly(polynomial* poly, int len) {

	// 배열을 순회하면서 출력
	for (int i = 0; i < len; i++) {
		if (poly[i].coef == 0) continue;
		if (i > 0) cout << " + ";
		cout << poly[i].coef << "x^" << poly[i].expon;
	}
}

/**
 * polynomial A의 모든 항에 대하여
 * polynomial B의 모든 항을 곱하고
 * polynomial C에 이를 저장하는 함수
 *
 * 최악의 경우(곱한 모든 항들의 지수가 다름) lenC는 1, 2, ... , lenA*lenB까지 증가
 * 따라서 시간 복잡도는 O(1) + O(2) + O(3) + ... + O(lenC)임
 *
 * O(lenC*(lenC+1)/2)
 * = O(lenC^2)
 * = O((lenA*lenB)^2)
 *
 * 최종 시간 복잡도(Worst Case)
 * O((lenA*lenB)^2)
 *
 */
void pmult(polynomial* A, polynomial* B, polynomial* C, int lenA, int lenB, int* lenC) {

	float a;
	int i, j, k, b;

	// lenA번 반복
	for (i = 0; i < lenA; i++) {

		if (A[i].coef == 0) continue;

		// lenB번 반복
		for (j = 0; j < lenB; j++) {

			// 계수가 0이면 건너뛰기
			if (B[j].coef == 0) continue;

			a = A[i].coef * B[j].coef;
			b = A[i].expon + B[j].expon;

			// 계수가 0이면 건너뛰기
			if (a == 0) continue;

			// 순차탐색
			// O(lenC)
			for (k = 0; k < *lenC; k++) {
				if (C[k].expon == b) {
					C[k].coef += a;
					break;
				}
			}

			// 일치하는 지수가 없으면 맨 뒤에 추가
			if (k == *lenC) {
				C[k].expon = b;
				C[k].coef = a;

				(*lenC)++;
			}
		}
	}
}

// polynomial을 지수 내림차순으로 정렬하는 함수 (bubble sort)
// O(len^2)
void sortPoly(polynomial* poly, int len) {
	int i, j, t, tt;
	// bubble sort 구현
	for (i = 0; i < len; i++) {
		for (j = 0; j < len; j++) {
			if (poly[i].expon > poly[j].expon) {
				t = poly[i].expon;
				tt = poly[i].coef;
				poly[i].expon = poly[j].expon;
				poly[i].coef = poly[j].coef;
				poly[j].expon = t;
				poly[j].coef = tt;
			}
		}
	}
}

/**
 * A.txt의 항 개수 = n
 * B.txt의 항 개수 = m
 *
 * 최종 시간 복잡도
 * O(3 + n + m + 2 * (nm)^2 + nm)
 * = O((nm)^2)
 *
 */

int main() {

	polynomial A[MAX_TERMS];
	polynomial B[MAX_TERMS];
	polynomial C[MAX_TERMS];

	int a, b, c = 0;

	// O(1)
	initPoly(A);
	initPoly(B);
	initPoly(C);

	// O(n)
	readPoly("A.txt", A, &a);
	// O(m)
	readPoly("B.txt", B, &b);

	// O((nm)^2)
	pmult(A, B, C, a, b, &c);

	// O((nm)^2)
	sortPoly(C, c);

	// O(nm)
	printPoly(C, c);

	return 0;
}