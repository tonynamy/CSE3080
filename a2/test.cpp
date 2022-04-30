#include <iostream>
#include <random>

int main() {

    // 시드값을 얻기 위한 random_device 생성.
    std::random_device rd;

    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
    std::mt19937 gen(rd());

    // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
    std::uniform_int_distribution<int> dis(0, 99);

    int tc = 1;

    while(tc--) {

        int c=0;
        int cc=0;

        int a = dis(gen);
        int b = dis(gen);

        for(int i=0;i<a;i++) {
            for(int j=0;j<b;j++) {
                for(int k=0;k<i*b+j+1;k++) {
                    c++;
                }
            }

        }

        std::cout << a << " " << b << std::endl;
        std::cout << c << " " << (a*b)*((a*b)+1)/2 << std::endl;


    }


}