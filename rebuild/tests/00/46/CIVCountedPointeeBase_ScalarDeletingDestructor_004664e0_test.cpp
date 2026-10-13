#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("004664e0_TEST PASS\n");
    return 0;
}