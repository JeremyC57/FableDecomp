#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("009f0710_TEST PASS\n");
    return 0;
}