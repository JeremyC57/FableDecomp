#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("0041c8c5_TEST PASS\n");
    return 0;
}