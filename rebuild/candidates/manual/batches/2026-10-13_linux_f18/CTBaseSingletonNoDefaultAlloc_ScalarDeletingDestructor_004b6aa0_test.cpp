#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("004b6aa0_TEST PASS\n");
    return 0;
}
