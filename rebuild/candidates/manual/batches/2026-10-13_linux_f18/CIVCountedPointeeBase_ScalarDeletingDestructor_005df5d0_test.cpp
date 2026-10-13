#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("005df5d0_TEST PASS\n");
    return 0;
}
