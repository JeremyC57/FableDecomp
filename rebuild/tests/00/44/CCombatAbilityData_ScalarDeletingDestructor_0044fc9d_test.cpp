#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("0044fc9d_TEST PASS\n");
    return 0;
}