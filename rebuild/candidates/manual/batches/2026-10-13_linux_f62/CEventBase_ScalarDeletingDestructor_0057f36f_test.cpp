#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("0057f36f_TEST PASS\n");
    return 0;
}
