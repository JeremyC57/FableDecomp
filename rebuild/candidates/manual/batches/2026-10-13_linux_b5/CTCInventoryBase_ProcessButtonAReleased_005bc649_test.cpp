#include <cstdio>
static int g_c = 0; struct I { virtual void v0() {} virtual void Close(bool) { ++g_c; } void* sel;
    void P() { if (!sel) Close(true); } };
int main()
{
    I i; i.sel = 0; i.P(); i.sel = &i; i.P(); if (g_c != 1) { std::printf("005bc649_TEST FAIL\n"); return 1; }

    std::printf("005bc649_TEST PASS\n");
    return 0;
}
