#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("00447a20_TEST PASS\n");
    return 0;
}
