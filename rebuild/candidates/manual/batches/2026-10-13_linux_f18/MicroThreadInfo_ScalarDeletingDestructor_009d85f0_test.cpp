#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("009d85f0_TEST PASS\n");
    return 0;
}
