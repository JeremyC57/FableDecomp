#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("005508b0_TEST PASS\n");
    return 0;
}