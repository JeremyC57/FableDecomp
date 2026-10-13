#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("007c28a0_TEST PASS\n");
    return 0;
}
