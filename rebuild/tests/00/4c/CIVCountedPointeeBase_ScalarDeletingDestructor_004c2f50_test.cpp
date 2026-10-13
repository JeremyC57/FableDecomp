#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("004c2f50_TEST PASS\n");
    return 0;
}