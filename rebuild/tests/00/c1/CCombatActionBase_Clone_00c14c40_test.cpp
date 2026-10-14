#include <cstdio>
static int g_c = 0; struct X { unsigned char b[0x44]; X() {} X(const X&) { ++g_c; } X* C() { return new X(*this); } };
static bool Check() { X x; X* y = x.C(); bool ok = y && g_c == 1; delete y; return ok; }

int main()
{
    if (!Check()) { std::printf("00c14c40_TEST FAIL\n"); return 1; }
    std::printf("00c14c40_TEST PASS\n");
    return 0;
}