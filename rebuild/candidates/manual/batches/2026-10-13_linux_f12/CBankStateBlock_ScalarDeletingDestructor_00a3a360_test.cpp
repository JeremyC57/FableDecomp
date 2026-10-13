#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("00a3a360_TEST PASS\n");
    return 0;
}
