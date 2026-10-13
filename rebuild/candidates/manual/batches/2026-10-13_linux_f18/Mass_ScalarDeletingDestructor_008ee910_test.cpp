#include <cstdio>
struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;

    std::printf("008ee910_TEST PASS\n");
    return 0;
}
