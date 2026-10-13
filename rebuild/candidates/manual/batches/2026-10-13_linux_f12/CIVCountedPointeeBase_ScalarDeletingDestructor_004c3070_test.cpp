#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("004c3070_TEST PASS\n");
    return 0;
}
