#include <cstdio>
static int g_m = 0; struct M { M() { ++g_m; } };
struct D { unsigned char pad[8]; M m; D() : m() {} };
int main()
{
    D d; if (g_m != 1) { std::printf("0045368c_TEST FAIL\n"); return 1; }

    std::printf("0045368c_TEST PASS\n");
    return 0;
}
