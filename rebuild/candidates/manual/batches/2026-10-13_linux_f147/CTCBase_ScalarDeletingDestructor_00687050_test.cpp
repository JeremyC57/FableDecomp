#include <cstdio>
static int g_d = 0; struct Base { virtual void v00() {} ~Base() { ++g_d; } };
struct X : Base { virtual ~X() { } };
int main()
{
    X* x = new X; delete x; if (g_d != 1) { std::printf("00687050_TEST FAIL\n"); return 1; }

    std::printf("00687050_TEST PASS\n");
    return 0;
}
