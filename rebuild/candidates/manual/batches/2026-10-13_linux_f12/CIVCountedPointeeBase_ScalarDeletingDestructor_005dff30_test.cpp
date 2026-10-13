#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("005dff30_TEST PASS\n");
    return 0;
}
