#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("00864e00_TEST PASS\n");
    return 0;
}
