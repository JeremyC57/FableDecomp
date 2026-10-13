#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("00a61f70_TEST PASS\n");
    return 0;
}
