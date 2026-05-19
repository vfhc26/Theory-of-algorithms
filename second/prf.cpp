#include <iostream>
#include <vector>

// Примитивные рекурсивные функции
int Z(int x) {return 0;}
int S(int x) {return x + 1;}
int I(int x) {return x;}

int C1(int x) {return S(Z(x));}

int P(int x) {
    if (x == 0) return Z(x);
    return x - 1;
}

int sub(int x, int y) {
    if (y == 0) return x;
    return P(sub(x, y - 1));
}
int isEven(int x) {
    if (x == 0) return C1(x);
    return sub(C1(x), isEven(x - 1));
}

int main(){
    std::vector<std::pair<int, int>> test_cases = {
            {0, 1},
            {11, 0},
            {2, 1},
            {39, 0},
            {400, 1},
            {67, 0},
            {100, 1},
            {11, 0},
    };
    for (const auto& test : test_cases) {
        int x = test.first;
        int expected = test.second;
        int result = isEven(x);
        std::cout << "isEven(" << x << ") = " << result << " (expected: " << expected << ")" << std::endl;
    }
}


