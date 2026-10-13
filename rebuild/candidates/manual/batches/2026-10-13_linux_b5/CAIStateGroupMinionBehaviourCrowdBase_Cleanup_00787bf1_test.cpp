#include <cstdio>
static int g_b = 0; struct G { virtual void v0() {} virtual void Base() { ++g_b; } bool f, c; void Cleanup() { c = false; f = true; Base(); } };
int main()
{
    G g; g.f = false; g.c = true; g.Cleanup(); if (!g.f || g.c || g_b != 1) { std::printf("00787bf1_TEST FAIL\n"); return 1; }

    std::printf("00787bf1_TEST PASS\n");
    return 0;
}
