#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("007b3e50_TEST PASS\n");
    return 0;
}
